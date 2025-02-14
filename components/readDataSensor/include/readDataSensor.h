#ifndef READDATASENSOR_H
#define READDATASENSOR_H
#include "freertos/FreeRTOS.h"
#include "yl69.h"
#include "esp_log.h"
#include "sensorDHT.h"
#include "dht.h"
#include "esp_err.h"

typedef struct  {
    int pin_sensor_DHT22;
    int sensor_type_DHT22;
    QueueHandle_t buffer;
    int sensor_id_Yl69;
    int channel_Yl69;
    yl69_reading_t readingYL69;
    dht22_reading_t readingDHT22;
}config_sensor_hub_t;

sensor_config_t set_config_DHT22 (int pin, int sensor_type,  QueueHandle_t buffer);
yl69_config_t set_config_YL69 (int sensor_id, int channel);
void read_data_Yl69 (config_sensor_hub_t *cfg_sensor_hub);
void read_data_DHT22 (config_sensor_hub_t *cfg_sensor_hub);
#endif 