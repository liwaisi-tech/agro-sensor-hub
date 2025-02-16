#include <stdio.h>
#include "taskSensores.h"
#include "queueManager.h"
#include "tempHumidity.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sensorDHT.h"

static const char *TAG = "task_sensores";

#define TASK_SENSORES_STACK_SIZE 2048
#define TASK_SENSORES_PRIORITY 10
#define SENSOR_READ_DELAY_MS 30000

// Simulación de lecturas de sensores
static void sensorHumedadSuelo1(int *humedad) {
    *humedad = 65; // Simulando 65% de humedad
}

static void sensorHumedadSuelo2(int *humedad) {
    *humedad = 70; // Simulando 70% de humedad
}

static void sensorAmbiente1(float *temperatura, float *humedad) {
    *temperatura = 25.5f; // Simulando 25.5°C
    *humedad = 60.0f;    // Simulando 60% de humedad
}

static void sensorAmbiente2(float *temperatura, float *humedad) {
        esp_err_t ret = dht_read_float_data(sensor_type, pin, &humedad, &temperatura);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "Lectura DHT22: Humedad=%.1f%%, Temperatura=%.1f°C", sensor_data.humidity1, sensor_data.temperature1);
        } else {
            ESP_LOGE(TAG, "Error al leer el sensor DHT22: %d", ret);
        }
}

dht_sensor_type_t sensor_type = DHT_TYPE_AM2301;
gpio_num_t pin = GPIO_NUM_32;

static void task_sensores(void *pvParameters) {
    tempHumidity_t sensor_data;

    while (1) {
        // Leer sensores

        sensorAmbiente1(&sensor_data.temperature1, &sensor_data.humidity1);

        // sensorHumedadSuelo1(&sensor_data.humGroud1);
        // sensorHumedadSuelo2(&sensor_data.humGroud2);
        // sensorAmbiente2(&sensor_data.temperature2, &sensor_data.humidity2);

        ESP_LOGI(TAG, "Lecturas: Suelo1=%d%%, Suelo2=%d%%, T1=%.1f°C, H1=%.1f%%, T2=%.1f°C, H2=%.1f%%",
                 sensor_data.humGroud1, sensor_data.humGroud2,
                 sensor_data.temperature1, sensor_data.humidity1,
                 sensor_data.temperature2, sensor_data.humidity2);

        // Enviar a la cola de mediciones
        if (xQueueSend(queue_mediciones, &sensor_data, pdMS_TO_TICKS(100)) != pdTRUE) {
            ESP_LOGW(TAG, "Cola llena - no se pudieron enviar datos");
        }

        vTaskDelay(pdMS_TO_TICKS(SENSOR_READ_DELAY_MS));
    }
}

esp_err_t init_task_sensores(void) {
    BaseType_t res = xTaskCreate(
        task_sensores,
        "task_sensores",
        TASK_SENSORES_STACK_SIZE,
        NULL,
        TASK_SENSORES_PRIORITY,
        NULL
    );

    if (res != pdPASS) {
        ESP_LOGE(TAG, "Error creando task_sensores");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Task sensores iniciada correctamente");
    return ESP_OK;
}
