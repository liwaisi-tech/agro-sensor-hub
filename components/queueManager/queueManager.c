#include <stdio.h>
#include "queue_manager.h"
#include "esp_log.h"
#include "tempHumidity.h"

static const char *TAG = "queue_manager";

// Definición de los handles de las colas
QueueHandle_t queue_mediciones = NULL;
QueueHandle_t queue_control_pines = NULL;
QueueHandle_t queue_websocket = NULL;

// Tamaños de las colas
#define QUEUE_MEDICIONES_SIZE    10
#define QUEUE_CONTROL_PINES_SIZE 5
#define QUEUE_WEBSOCKET_SIZE     10

esp_err_t init_queues(void) {
    // Crear cola para mediciones
    queue_mediciones = xQueueCreate(QUEUE_MEDICIONES_SIZE, sizeof(tempHumidity_t));
    if (queue_mediciones == NULL) {
        ESP_LOGE(TAG, "Error creating queue_mediciones");
        return ESP_FAIL;
    }

    // Crear cola para control de pines
    queue_control_pines = xQueueCreate(QUEUE_CONTROL_PINES_SIZE, sizeof(tempHumidity_t));
    if (queue_control_pines == NULL) {
        ESP_LOGE(TAG, "Error creating queue_control_pines");
        return ESP_FAIL;
    }

    // Crear cola para websocket
    queue_websocket = xQueueCreate(QUEUE_WEBSOCKET_SIZE, sizeof(tempHumidity_t));
    if (queue_websocket == NULL) {
        ESP_LOGE(TAG, "Error creating queue_websocket");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "All queues created successfully");
    return ESP_OK;
}
