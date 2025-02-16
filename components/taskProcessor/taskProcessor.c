#include <stdio.h>
#include "taskProcessor.h"
#include "queueManager.h"
#include "tempHumidity.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "task_processor";

// Configuración de la tarea
#define TASK_PROCESSOR_STACK_SIZE 4096
#define TASK_PROCESSOR_PRIORITY 5

// Función principal de la tarea
static void process_sensor_data(void *pvParameters) {
    tempHumidity_t sensor_data;

    while (1) {
        // Esperar datos de los sensores
        if (xQueueReceive(queue_mediciones, &sensor_data, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(TAG, "Received sensor data - Temp: %.2f, Humidity: %d", 
                     sensor_data.temperature1, sensor_data.humGroud1);

            // Enviar a cola de websocket
            if (xQueueSend(queue_websocket, &sensor_data, 0) != pdTRUE) {
                ESP_LOGW(TAG, "Failed to send data to websocket queue");
            }

            // Enviar a cola de control de pines
            if (xQueueSend(queue_control_pines, &sensor_data, 0) != pdTRUE) {
                ESP_LOGW(TAG, "Failed to send data to pin control queue");
            }
        }
    }
}

esp_err_t init_task_processor(void) {
    BaseType_t res = xTaskCreate(
        process_sensor_data,         // Función de la tarea
        "process_sensor_data",       // Nombre de la tarea
        TASK_PROCESSOR_STACK_SIZE,   // Stack size
        NULL,                        // Parámetros
        TASK_PROCESSOR_PRIORITY,     // Prioridad
        NULL                         // Handle de la tarea
    );

    if (res != pdPASS) {
        ESP_LOGE(TAG, "Failed to create task");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Task processor initialized successfully");
    return ESP_OK;
}

void func(void)
{

}
