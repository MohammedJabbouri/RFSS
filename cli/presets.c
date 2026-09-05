#include "presets.h"
#include <string.h>

static config_t *preset_build_realistic(void) {
    config_t *cfg = config_create_empty();
    if (!cfg) return NULL;

    config_set_long(cfg, "job", "count", PRESET_REALISTIC_JOB_COUNT);
    config_set_long(cfg, "job", "n_samples", PRESET_REALISTIC_JOB_NSAMPLES);
    config_set_string(cfg, "job", "output_dir", PRESET_REALISTIC_JOB_OUTPUTDIR);

    config_set_bool(cfg, "awgn", "enabled", PRESET_REALISTIC_AWGN_ENABLED);
    config_set_double(cfg, "awgn", "snr_db", PRESET_REALISTIC_AWGN_SNRDB);
    config_set_long(cfg, "awgn", "seed", PRESET_REALISTIC_AWGN_SEED);

    return cfg;
}

config_t *preset_build(const char *name) {
    if (strcmp(name, "realistic") == 0) return preset_build_realistic();
    return NULL;
}