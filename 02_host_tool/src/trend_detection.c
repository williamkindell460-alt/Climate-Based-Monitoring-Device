#include "trend_detection.h"
#include <stdio.h>
#include <stdint.h>


#define HISTORY_SIZE 32

// ---------------- TEMPERATURE ----------------

static float temp_history[HISTORY_SIZE];
static uint8_t temp_index = 0;

void trend_temp_add_sample(float temp)
{
    temp_history[temp_index] = temp;
    temp_index = (temp_index + 1) % HISTORY_SIZE;
}

float trend_temp_delta(void)
{
    uint8_t oldest = (temp_index + 1) % HISTORY_SIZE;
    uint8_t newest = (temp_index == 0 ? HISTORY_SIZE - 1 : temp_index - 1);

    return temp_history[newest] - temp_history[oldest];
}

float trend_temp_average(void)
{
    float sum = 0.0f;
    for (int i = 0; i < HISTORY_SIZE; i++)
        sum += temp_history[i];
    return sum / HISTORY_SIZE;
}


// ---------------- HUMIDITY ----------------

static float hum_history[HISTORY_SIZE];
static uint8_t hum_index = 0;

void trend_hum_add_sample(float hum)
{
    hum_history[hum_index] = hum;
    hum_index = (hum_index + 1) % HISTORY_SIZE;
}

float trend_hum_delta(void)
{
    uint8_t oldest = (hum_index + 1) % HISTORY_SIZE;
    uint8_t newest = (hum_index == 0 ? HISTORY_SIZE - 1 : hum_index - 1);

    return hum_history[newest] - hum_history[oldest];
}

float trend_hum_average(void)
{
    float sum = 0.0f;
    for (int i = 0; i < HISTORY_SIZE; i++)
        sum += hum_history[i];
    return sum / HISTORY_SIZE;
}


// ---------------- PRESSURE ----------------

static float pres_history[HISTORY_SIZE];
static uint8_t pres_index = 0;

void trend_pres_add_sample(float pres)
{
    pres_history[pres_index] = pres;
    pres_index = (pres_index + 1) % HISTORY_SIZE;
}

float trend_pres_delta(void)
{
    uint8_t oldest = (pres_index + 1) % HISTORY_SIZE;
    uint8_t newest = (pres_index == 0 ? HISTORY_SIZE - 1 : pres_index - 1);

    return pres_history[newest] - pres_history[oldest];
}

float trend_pres_average(void)
{
    float sum = 0.0f;
    for (int i = 0; i < HISTORY_SIZE; i++)
        sum += pres_history[i];
    return sum / HISTORY_SIZE;
}


// ---------------- Air Quality ----------------

static float air_quality_history[HISTORY_SIZE];
static uint8_t air_quality_index = 0;

void trend_air_quality_add_sample(float air_quality)
{
    air_quality_history[air_quality_index] = air_quality;
    air_quality_index = (air_quality_index + 1) % HISTORY_SIZE;
}

float trend_air_quality_delta(void)
{
    uint8_t oldest = (air_quality_index + 1) % HISTORY_SIZE;
    uint8_t newest = (air_quality_index == 0 ? HISTORY_SIZE - 1 : air_quality_index - 1);

    return air_quality_history[newest] - air_quality_history[oldest];
}

float trend_air_quality_average(void)
{
    float sum = 0.0f;
    for (int i = 0; i < HISTORY_SIZE; i++)
        sum += air_quality_history[i];
    return sum / HISTORY_SIZE;
}

static const char* classify_trend(float delta)
{
    if (delta > 0.5f)      return "Rising";
    if (delta > 0.1f)      return "Slight increase";
    if (delta < -0.5f)     return "Falling";
    if (delta < -0.1f)     return "Slight decrease";
    return "Stable";
}

void trend_print_report(void)
{
    float d_temp = trend_temp_delta();
    float d_hum  = trend_hum_delta();
    float d_pres = trend_pres_delta();
    float d_gas  = trend_gas_delta();

    printf("----------------------------------------------------\n");
    printf("Trend Detection\n");
    printf("----------------------------------------------------\n");

    printf("Temperature: %s (Δ = %.2f C)\n", classify_trend(d_temp), d_temp);
    printf("Humidity:    %s (Δ = %.2f %%)\n", classify_trend(d_hum), d_hum);
    printf("Pressure:    %s (Δ = %.2f hPa)\n", classify_trend(d_pres), d_pres);

    // Gas resistance → you will convert to AQI, CO2, VOC later
    printf("Gas resistance:         %s (Δ = %.2f Ω)\n", classify_trend(d_gas), d_gas);
}
