#include <stdio.h>
#include "tempHumidity.h"
#include "readDataSensor.h"
#include "queueManager.h"
#include "taskProcessor.h"
#include "pinController.h"
#include "webSocketHandler.h"
#include "taskSensores.h"

static const char *TAG = "agro-sensor-hub";
#define DHT22_TYPE 1
#define YL69_TYPE  2
#define ID1YL69  1
#define ID2YL69  2
#define SENSOR_TYPE DHT_TYPE_AM2301
#define GPIO_PIN2DHT 33
#define GPIO_PIN1DHT 32
#define CHANNEL2_Yl69 ADC_CHANNEL_7
#define CHANNEL1_Yl69 ADC_CHANNEL_6
#define QUEUE_SIZE 10  // Define el tamaño de la cola
QueueHandle_t queue_mediciones;  // Declara la variable global para la cola de mediciones
tempHumidity_t data; 

static esp_err_t init_queues_and_tasks(void) {
    esp_err_t ret;

    // Inicializar colas
    ret = init_queues();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize queues");
        return ret;
    }

    // Inicializar tareas
    ret = init_task_sensores();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize task sensores");
        return ret;
    }

    ret = init_task_processor();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize task processor");
        return ret;
    }

    ret = init_pin_controller();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize pin controller");
        return ret;
    }

    ret = init_websocket_handler();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize websocket handler");
        return ret;
    }

    ESP_LOGI(TAG, "All queues and tasks initialized successfully");
    return ESP_OK;
}

void read_YL69(config_sensor_hub_t *cfg, tempHumidity_t *data) {
    read_data_Yl69(cfg);
    data->humGroud1 = cfg->readingYL69.humidity; // Asumiendo que hay un campo para el suelo
}

void read_DHT22(config_sensor_hub_t *cfg, tempHumidity_t *data) {
    read_data_DHT22(cfg);
    data->humidity1 = cfg->readingDHT22.humidity;
    data->temperature1 = cfg->readingDHT22.temperature;
}

void send_medicones_task (void *pvParameters)
{
        queue_mediciones = xQueueCreate(QUEUE_SIZE, sizeof(data));
        if (queue_mediciones == NULL) {
        ESP_LOGE(TAG, "Error al crear la cola para YL69");
        return;
        }

    config_sensor_hub_t cfg_sensor_hub1 = {GPIO_PIN1DHT, SENSOR_TYPE, queue_mediciones, ID1YL69, CHANNEL1_Yl69};
    config_sensor_hub_t cfg_sensor_hub2 = {GPIO_PIN2DHT, SENSOR_TYPE,queue_mediciones, ID2YL69, CHANNEL2_Yl69};

    while (1) {
        // Leer sensores cajon 1
        read_YL69(&cfg_sensor_hub1, &data);
        ESP_LOGI(TAG, "Dato recibido humedad suelo cajon %d  valor: %d", cfg_sensor_hub1.sensor_id_Yl69, data.humGroud1);
        read_DHT22(&cfg_sensor_hub1, &data);
        ESP_LOGI(TAG, "Dato recibido temperatura cajon %d  valor: %f", cfg_sensor_hub1.sensor_id_Yl69, data.temperature1);
        ESP_LOGI(TAG, "Dato recibido humedad cajon %d  valor: %f", cfg_sensor_hub1.sensor_id_Yl69, data.humidity1);

        // Leer sensores cajon 2
        read_YL69(&cfg_sensor_hub2, &data);
        ESP_LOGI(TAG, "Dato recibido humedad suelo cajon %d  valor: %d", cfg_sensor_hub2.sensor_id_Yl69, data.humGroud2);
        read_DHT22(&cfg_sensor_hub2, &data);
        ESP_LOGI(TAG, "Dato recibido temperatura suelo cajon %d  valor: %f", cfg_sensor_hub2.sensor_id_Yl69, data.temperature2);
        ESP_LOGI(TAG, "Dato recibido humedad suelo cajon %d  valor: %f", cfg_sensor_hub2.sensor_id_Yl69, data.humidity2);

        // Enviar datos a la cola
        xQueueSend(queue_mediciones, &data, portMAX_DELAY);

        // Esperar intervalo de muestreo
        vTaskDelay(pdMS_TO_TICKS(5000)); // 5 segundos
    }
}

void receive_medicones_task(void *pvParameters) {
    char *printToData; 
    while (1) {
        // Recibe datos desde la cola
        if (xQueueReceive(queue_mediciones, &data, portMAX_DELAY)) {
            // Lógica de validación y decisión
            printToData = convert_to_json_string(&data);
            ESP_LOGI(TAG, "Datos de los cajones recibidos %s:", printToData);  
        }
    }  

}


void app_main (void) {
    esp_err_t ret = init_queues_and_tasks();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize queues and tasks");
        return;
    }
}

