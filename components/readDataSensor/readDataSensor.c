#include <stdio.h>
#include "readDataSensor.h"

static const char *TAG = "readDataSensor";
yl69_config_t set_config_YL69(int sensor_id, int channel) {
    yl69_config_t config = YL69_DEFAULT_CONFIG;
    config.sensor_id = sensor_id;
    config.channel = channel;
        ESP_LOGI(TAG, "Channel: %d",config.channel);
    return config;
}

sensor_config_t set_config_DHT22(int pin, int sensor_type, QueueHandle_t buffer) {
    sensor_config_t configDHT = DHT_DEFAULT_CONFIG;
    configDHT.pin = pin;
    configDHT.sensor_type = sensor_type;
    configDHT.queue = buffer;
    return configDHT;
}

void read_data_Yl69(config_sensor_hub_t *cfg_sensor_hub) {
    yl69_config_t cfg_yl69_sensor = set_config_YL69(cfg_sensor_hub->sensor_id_Yl69, cfg_sensor_hub->channel_Yl69);
    esp_err_t ret = yl69_init(&cfg_yl69_sensor);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error al iniciar config del sensor: %d", ret);
    }
    cfg_sensor_hub->readingYL69.humidity = yl69_read_percentage(cfg_yl69_sensor.channel);
}

void read_data_DHT22(config_sensor_hub_t *cfg_sensor_hub) {
   sensor_config_t cfg_dht22_sensor = set_config_DHT22(cfg_sensor_hub->pin_sensor_DHT22, cfg_sensor_hub->sensor_type_DHT22, cfg_sensor_hub->buffer);
   sensor_dht22_main (&cfg_dht22_sensor);
   cfg_sensor_hub->readingDHT22.humidity = cfg_dht22_sensor.reading.humidity;
   cfg_sensor_hub->readingDHT22.temperature = cfg_dht22_sensor.reading.temperature;
}
