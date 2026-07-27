#include "logging.h"
#include <stdio.h>

int main(void)
{
    load_csv_and_feed_trends("sample_log_01.csv");
    trend_print_report();
    return 0;
}