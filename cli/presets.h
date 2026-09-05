#ifndef ENGINE_CLI_PRESETS_H
#define ENGINE_CLI_PRESETS_H

#include "config.h"

#define PRESET_REALISTIC_JOB_COUNT     100
#define PRESET_REALISTIC_JOB_NSAMPLES  1024
#define PRESET_REALISTIC_JOB_OUTPUTDIR "output"
#define PRESET_REALISTIC_AWGN_ENABLED  1
#define PRESET_REALISTIC_AWGN_SNRDB    15.0
#define PRESET_REALISTIC_AWGN_SEED     42

config_t *preset_build(const char *name);

#endif