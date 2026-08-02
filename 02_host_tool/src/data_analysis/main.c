#include "run_pipeline.h"

// MODE_DEFAULT    stats + anomalies
// MODE_ANOMALIES  anomalies only
// MODE_STATS      stats only

int main(int argc, char *argv[]) {
    pipeline_argc = argc;
    pipeline_argv = argv;
    return run_pipeline(MODE_DEFAULT);
}