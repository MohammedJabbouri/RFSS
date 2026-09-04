#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CONFIG_INITIAL_CAPACITY 32
#define CONFIG_LINE_MAX 512

typedef struct {
    char *section;
    char *key;
    char *value;
} config_entry_t;

struct config {
    config_entry_t *entries;
    size_t count;
    size_t capacity;
};

static char *dupstr(const char *s) {
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    if (copy) memcpy(copy, s, len);
    return copy;
}

static char *trim(char *s) {
    while (isspace((unsigned char)*s)) s++;
    if (*s == '\0') return s;
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return s;
}

static void strip_comment(char *s) {
    for (char *p = s; *p; p++) {
        if (*p == '#' || *p == ';') { *p = '\0'; return; }
    }
}

static int config_push(config_t *cfg, const char *section, const char *key, const char *value) {
    if (cfg->count == cfg->capacity) {
        size_t new_cap = cfg->capacity * 2;
        config_entry_t *grown = realloc(cfg->entries, new_cap * sizeof(config_entry_t));
        if (!grown) return -1;
        cfg->entries  = grown;
        cfg->capacity = new_cap;
    }

    config_entry_t *e = &cfg->entries[cfg->count];
    e->section = dupstr(section);
    e->key     = dupstr(key);
    e->value   = dupstr(value);
    if (!e->section || !e->key || !e->value) {
        free(e->section); free(e->key); free(e->value);
        return -1;
    }

    cfg->count++;
    return 0;
}

config_t *config_load(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return NULL;

    config_t *cfg = malloc(sizeof(config_t));
    if (!cfg) { fclose(f); return NULL; }

    cfg->entries = malloc(CONFIG_INITIAL_CAPACITY * sizeof(config_entry_t));
    if (!cfg->entries) { free(cfg); fclose(f); return NULL; }
    cfg->count    = 0;
    cfg->capacity = CONFIG_INITIAL_CAPACITY;

    char line[CONFIG_LINE_MAX];
    char section[CONFIG_LINE_MAX] = "";
    int line_no = 0;

    while (fgets(line, sizeof(line), f)) {
        line_no++;
        strip_comment(line);
        char *trimmed = trim(line);
        if (*trimmed == '\0') continue;   /* blank or comment-only line */

        size_t len = strlen(trimmed);
        if (trimmed[0] == '[' && trimmed[len - 1] == ']') {
            trimmed[len - 1] = '\0';
            char *name = trim(trimmed + 1);
            strncpy(section, name, sizeof(section) - 1);
            section[sizeof(section) - 1] = '\0';
            continue;
        }

        char *eq = strchr(trimmed, '=');
        if (!eq) {
            fprintf(stderr, "config: %s:%d: expected 'key = value', skip: %s\n", path, line_no, trimmed);
            continue;
        }
        *eq = '\0';
        char *key   = trim(trimmed);
        char *value = trim(eq + 1);

        if (config_push(cfg, section, key, value) != 0) {
            config_destroy(cfg);
            fclose(f);
            return NULL;
        }
    }

    fclose(f);
    return cfg;
}

void config_destroy(config_t *cfg) {
    if (!cfg) return;
    for (size_t i = 0; i < cfg->count; i++) {
        free(cfg->entries[i].section);
        free(cfg->entries[i].key);
        free(cfg->entries[i].value);
    }
    free(cfg->entries);
    free(cfg);
}

const char *config_get(const config_t *cfg, const char *section, const char *key) {
    if (!cfg) return NULL;
    for (size_t i = 0; i < cfg->count; i++) {
        if (strcmp(cfg->entries[i].section, section) == 0 &&
            strcmp(cfg->entries[i].key, key) == 0) {
            return cfg->entries[i].value;
        }
    }
    return NULL;
}

double config_get_double(const config_t *cfg, const char *section, const char *key, double default_value) {
    const char *v = config_get(cfg, section, key);
    if (!v) return default_value;
    char *end;
    double parsed = strtod(v, &end);
    return (end == v) ? default_value : parsed;
}

long config_get_long(const config_t *cfg, const char *section, const char *key, long default_value) {
    const char *v = config_get(cfg, section, key);
    if (!v) return default_value;
    char *end;
    long parsed = strtol(v, &end, 10);
    return (end == v) ? default_value : parsed;
}

// CASE SENS

int config_get_bool(const config_t *cfg, const char *section, const char *key, int default_value) {
    const char *v = config_get(cfg, section, key);
    if (!v) return default_value;
    if (!strcmp(v, "true") || !strcmp(v, "1") || !strcmp(v, "yes") || !strcmp(v, "on"))  return 1;
    if (!strcmp(v, "false") || !strcmp(v, "0") || !strcmp(v, "no") || !strcmp(v, "off")) return 0;
    return default_value;
}