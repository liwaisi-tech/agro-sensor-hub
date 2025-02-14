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

typedef struct {
    uint8_t sensor_id;
    int16_t humidity;
} yl69_reading_t;

typedef struct {
    adc_channel_t channel;
    adc_bitwidth_t bitwidth;
    adc_atten_t atten;
    adc_unit_t unit; 
    uint32_t read_interval_ms;  
    QueueHandle_t queue;    
    uint8_t sensor_id; 
    uint8_t priority;
} yl69_config_t;

#define YL69_DEFAULT_CONFIG { \
    .channel = ADC_CHANNEL_6,        \
    .bitwidth = ADC_BITWIDTH_12,     \
    .atten = ADC_ATTEN_DB_12,        \
    .unit = ADC_UNIT_1,              \
    .read_interval_ms = 1000,        \
    .queue = NULL,                   \
    .sensor_id = 0,                  \
    .priority = 1,               \
}

esp_err_t yl69_init(yl69_config_t *config);
int yl69_read_raw(adc_channel_t channel);
int yl69_read_percentage(adc_channel_t channel);

#endif // YL69_H