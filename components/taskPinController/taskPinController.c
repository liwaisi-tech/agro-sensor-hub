#include "taskPinController.h"
#include "queueManager.h"
#include "tempHumidity.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "task_pin_controller";

#define PIN_CONTROLLER_STACK_SIZE 2048
#define PIN_CONTROLLER_PRIORITY 7

static void pin_control_task(void *pvParameters) {
    tempHumidity_t sensor_data;

    while (1) {
        if (xQueueReceive(queue_control_pines, &sensor_data, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(TAG, "Llamar lógica de pines acá.");
        }
    }
}

esp_err_t init_pin_controller(void) {
    BaseType_t res = xTaskCreate(
        pin_control_task,
        "pin_control_task",
        PIN_CONTROLLER_STACK_SIZE,
        NULL,
        PIN_CONTROLLER_PRIORITY,
        NULL
    );

    if (res != pdPASS) {
        ESP_LOGE(TAG, "Failed to create pin controller task");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Pin controller task initialized successfully");
    return ESP_OK;
} 