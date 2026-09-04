#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <complex.h>
#include <sys/stat.h>

#include "config.h"
#include "settings.h"
#include "../core/signal.h"
#include "../core/chain.h"

#define DEFAULT_COUNT      100
#define DEFAULT_N_SAMPLES  1024
#define DEFAULT_OUTPUT_DIR "output"
#define MAX_STAGES         16

// WILL GET SWAPPED, TEMPORARY

static signal_t *make_placeholder_signal(size_t n_samples) {
    signal_t *sig = signal_create(n_samples, 1e6, 915e6);
    if (!sig) return NULL;
    for (size_t i = 0; i < n_samples; i++) {
        sig->samples[i] = 1.0f + 0.0f * I;
    }
    return sig;
}

static int write_signal(const signal_t *sig, const char *path) {
    FILE *f = fopen(path, "wb");
    if (!f) return -1;
    size_t written = fwrite(sig->samples, sizeof(float complex), sig->n_samples, f);
    fclose(f);
    return (written == sig->n_samples) ? 0 : -1;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <config.ini>\n", argv[0]);
        return 1;
    }

    config_t *cfg = config_load(argv[1]);
    if (!cfg) {
        fprintf(stderr, "error: could not open config file '%s'\n", argv[1]);
        return 1;
    }

    long count     = config_get_long(cfg, "job", "count", DEFAULT_COUNT);
    long n_samples = config_get_long(cfg, "job", "n_samples", DEFAULT_N_SAMPLES);
    const char *out_dir = config_get(cfg, "job", "output_dir");
    if (!out_dir) out_dir = DEFAULT_OUTPUT_DIR;

    if (count <= 0) {
        fprintf(stderr, "error: [job] count must be positive, got %ld\n", count);
        config_destroy(cfg);
        return 1;
    }

    mkdir(out_dir, 0755);

    printf("generating %ld example(s) into '%s/'\n", count, out_dir);

    time_t last_report = time(NULL);
    int    failures     = 0;

    for (long i = 0; i < count; i++) {
        transform_t stages[MAX_STAGES];
        size_t n_stages;

        if (settings_build_chain(cfg, (uint64_t)i, stages, MAX_STAGES, &n_stages) != 0) {
            fprintf(stderr, "error: way too many stages enabled for MAX_STAGES=%d\n", MAX_STAGES);
            chain_free_stages(stages, n_stages)
            failures++;
            continue;
        }

        signal_t *sig = make_placeholder_signal((size_t)n_samples);
        if (!sig) {
            fprintf(stderr, "error: out of memory building example %ld\n", i);
            chain_free_stages(stages, n_stages);
            failures++;
            continue;
        }

        if (chain_apply(stages, n_stages, sig) != 0) {
            fprintf(stderr, "error: chain failed on example %ld\n", i);
            failures++;
        } else {
            char path[512];
            snprintf(path, sizeof(path), "%s/example_%06ld.iq", out_dir, i);
            if (write_signal(sig, path) != 0) {
                fprintf(stderr, "error: could not write '%s'\n", path);
                failures++;
            }
        }

        signal_destroy(sig);
        chain_free_stages(stages, n_stages);

        time_t now = time(NULL);
        if (difftime(now, last_report) >= 1.0 || i == count - 1) {
            printf("\r  %ld / %ld", i + 1, count);
            fflush(stdout);
            last_report = now;
        }
    }
    printf("\n");
    
    config_destroy(cfg);

    if (failures > 0) {
        fprintf(stderr, "done with %d failure(s)\n", failures);
        return 1;
    }
    printf("done: %ld example(s) written to '%s/'\n", count, out_dir);
    return 0;
}
