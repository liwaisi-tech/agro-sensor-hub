#include <stdio.h>
#include "taskSensores.h"
#include "queueManager.h"
#include "tempHumidity.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "dht.h"
#include "yl69.h"
#include "wifi.h"
#include <string.h>

static const char *TAG = "task_sensores";

#define TASK_SENSORES_STACK_SIZE 4096
#define TASK_SENSORES_PRIORITY 10
#define SENSOR_READ_DELAY_MS 30000
#define SENSOR_TYPE DHT_TYPE_AM2301
#define GPIO_PIN2DHT 33
#define GPIO_PIN1DHT 32
#define CHANNEL2_Yl69 ADC_CHANNEL_7
#define CHANNEL1_Yl69 ADC_CHANNEL_6
// Simulación de lecturas de sensores
/* static void sensorHumedadSuelo1(int *humedad) {
    *humedad = 65; // Simulando 65% de humedad
}*/

static void sensorHumedadSuelo(adc_channel_t channel, int *humedad, groud_sensor_type_t sensor_type) {
    yl69_config_t cfg_YL69 = YL69_DEFAULT_CONFIG;
    cfg_YL69.channel = channel;
    cfg_YL69.sensor_type = sensor_type;
    yl69_init(&cfg_YL69);
    yl69_read_percentage(cfg_YL69.channel,humedad,cfg_YL69.sensor_type);
} 

static void sensorAmbiente(dht_sensor_type_t sensor_type,gpio_num_t pin, float *temperatura, float *humedad) {
     esp_err_t ret = dht_read_float_data(sensor_type, pin, humedad, temperatura);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "Lectura DHT22 OK");
        } else {
            ESP_LOGE(TAG, "Error al leer el sensor DHT22: %d", ret);
        }
}



static void task_sensores(void *pvParameters) {
    tempHumidity_t sensor_data;
    
    // Obtener MAC address una sola vez al inicio
    get_mac_address(sensor_data.mac_address);
    strcpy(sensor_data.zona, CONFIG_ZONE);
    
    while (1) {
        // Leer sensores
        sensorAmbiente(SENSOR_TYPE, GPIO_PIN1DHT, &sensor_data.temperature, &sensor_data.humidity);


        sensorHumedadSuelo(CHANNEL1_Yl69, &sensor_data.humGroud1,TYPE_CAP);
        sensorHumedadSuelo(CHANNEL2_Yl69, &sensor_data.humGroud2, TYPE_YL69);

        ESP_LOGI(TAG, "Lecturas: Suelo1=%d%%, Suelo2=%d%%, H=%.1f%%, T=%.1f°C",
                 sensor_data.humGroud1, sensor_data.humGroud2,
                 sensor_data.humidity, sensor_data.temperature);

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
