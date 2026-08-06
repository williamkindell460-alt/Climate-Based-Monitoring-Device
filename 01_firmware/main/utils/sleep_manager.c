#include "sleep_manager.h"
#include "esp_sleep.h"

void enter_deep_sleep(uint64_t microseconds) {
    esp_sleep_enable_timer_wakeup(microseconds);
    esp_deep_sleep_start();
}
