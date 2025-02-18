#include "taskWebSocketHandler.h"
#include "queueManager.h"
#include "tempHumidity.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "websocket_handler";

#define WEBSOCKET_STACK_SIZE 8192
#define WEBSOCKET_PRIORITY 4

static void websocket_task(void *pvParameters) {
    tempHumidity_t sensor_data;

    while (1) {
        if (xQueueReceive(queue_websocket, &sensor_data, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(TAG, "Llamar lógica de WebSocket acá.");
        }
    }
}

esp_err_t init_websocket_handler(void) {
    BaseType_t res = xTaskCreate(
        websocket_task,
        "websocket_task",
        WEBSOCKET_STACK_SIZE,
        NULL,
        WEBSOCKET_PRIORITY,
        NULL
    );

    if (res != pdPASS) {
        ESP_LOGE(TAG, "Failed to create websocket task");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "WebSocket task initialized successfully");
    return ESP_OK;
} 