#ifndef ENGINE_CLI_CONFIG_H
#define ENGINE_CLI_CONFIG_H

typedef struct config config_t;

config_t *config_load(const char *path);

void config_destroy(config_t *cfg);

const char *config_get(const config_t *cfg, const char *section, const char *key);

double config_get_double(const config_t *cfg, const char *section, const char *key, double default_value);
long   config_get_long(const config_t *cfg, const char *section, const char *key, long default_value);
int    config_get_bool(const config_t *cfg, const char *section, const char *key, int default_value);

#endif