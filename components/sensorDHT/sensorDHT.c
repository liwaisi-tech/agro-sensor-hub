#include "dht.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_log.h>
#include "sensorDHT.h"

static const char *TAG = "SensorDHT";

typedef struct {
    sensor_config_t configdht;
    dht22_reading_t reading;
} taskParams;

void read_sensor_task(void *arg) {
    taskParams *params = (taskParams *) arg;
    esp_err_t ret = dht_read_float_data(params->configdht.sensor_type, params->configdht.pin,
             &params->reading.humidity, &params->reading.temperature);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Sensor en GPIO %d: Humedad %.2f %%, Temperatura %.2f C",
                 params->configdht.pin, params->reading.humidity, params->reading.temperature);
    } else {
        ESP_LOGE(TAG, "Error al leer el sensor en GPIO %d: %d", params->configdht.pin, ret);
    }
    vTaskDelete(NULL);
}

void sensor_dht22_main(sensor_config_t *configdht) {
    dht22_reading_t readingdht = {0}; // Inicializar la estructura
    taskParams params = {
        .configdht = *configdht,
        .reading = readingdht
    };
    // Crear tarea leer el sensor
    BaseType_t ret = xTaskCreate(read_sensor_task, "read_sensor_task", 2048, &params, 1, NULL);
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Error al crear la tarea para el sensor del pin GPIO%d", configdht->pin);
    }
    configdht->reading = params.reading;
}

