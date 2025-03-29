#include <stdio.h>
#include "tempHumidity.h"
#include "queueManager.h"
#include "taskProcessor.h"
#include "taskPinController.h"
#include "taskHttpClientHandler.h"
#include "taskSensores.h"
#include "wifi.h"
#include <esp_log.h>
#include "nvs_flash.h"

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
//QueueHandle_t queue_mediciones;  // Declara la variable global para la cola de mediciones
tempHumidity_t data; 

static esp_err_t init_wifi_and_services(void) {
    esp_err_t ret;

    // Inicializar WiFi primero
    ret = wifi_action_mode(true);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize WiFi");
        return ret;
    }
    ESP_LOGI(TAG, "WiFi initialized successfully");

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

    ret = init_http_client_handler();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize http client handler");
        return ret;
    }

    ESP_LOGI(TAG, "All services initialized successfully");
    return ESP_OK;
}

void app_main(void) {
    // Inicializar NVS (Non-volatile storage) necesario para WiFi
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Inicializar WiFi y servicios
    ret = init_wifi_and_services();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize system");
        return;
    }

    ESP_LOGI(TAG, "System initialized and running");
}

