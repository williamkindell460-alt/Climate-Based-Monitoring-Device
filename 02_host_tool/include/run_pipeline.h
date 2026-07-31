#ifndef RUN_PIPELINE_H
#define RUN_PIPELINE_H

#define MODE_DEFAULT 0   // stats + anomalies
#define MODE_ANOMALIES 1 // anomalies only
#define MODE_STATS 2     // stats only

int run_pipeline(int mode);

#endif
