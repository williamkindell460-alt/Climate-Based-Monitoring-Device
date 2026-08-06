#include "logger.h"
#include "read_bme680.h"
#include <stdarg.h>

static const char *TAG = "CLIMATE";

void logger_init(void) {
    // Nothing needed yet
}

void log_info() {
    char *data = read_bme680_sensors();
}
