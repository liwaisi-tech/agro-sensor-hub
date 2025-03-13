/*
 * Componente de ESP32 
 * Lectura sensor de humedad del suelo YL-69 o HL-69 Driver
 * Copyright 2024, Briggitte Castañeda <btatacc@gmail.com>
 */
#ifndef YL69_H
#define YL69_H
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"

typedef enum
{
    TYPE_YL69 = 0,   //!< YL69
    TYPE_CAP,      //!< Capacitivo
} groud_sensor_type_t;
typedef struct {
    adc_channel_t channel;
    adc_bitwidth_t bitwidth;
    adc_atten_t atten;
    adc_unit_t unit; 
    uint32_t read_interval_ms;  
    uint8_t sensor_id; 
    uint8_t priority;
    groud_sensor_type_t sensor_type;
    
} yl69_config_t;

#define YL69_DEFAULT_CONFIG { \
    .channel = ADC_CHANNEL_6,        \
    .bitwidth = ADC_BITWIDTH_12,     \
    .atten = ADC_ATTEN_DB_12,        \
    .unit = ADC_UNIT_1,              \
    .read_interval_ms = 1000,        \
    .sensor_id = 0,                  \
    .priority = 1,               \
    .sensor_type = TYPE_YL69,  \
}

esp_err_t yl69_init(yl69_config_t *config);
int yl69_read_raw(adc_channel_t channel);
void yl69_read_percentage(adc_channel_t channel, int *humidity, groud_sensor_type_t sensor_type);

#endif // YL69_H