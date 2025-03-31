#include "taskHttpClientHandler.h"
#include "queueManager.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_http_client.h"
#include "esp_system.h"
#include <string.h>
#include "wifi.h"
#include "esp_wifi.h"

static const char *TAG = "http_client";

#define HTTP_CLIENT_STACK_SIZE 4096  // Reducido para optimizar memoria
#define HTTP_CLIENT_PRIORITY 4
#define MAX_HTTP_OUTPUT_BUFFER 256

// URL del servidor (debería estar en un archivo de configuración)
static const char *SERVER_URL = CONFIG_SERVER_URL;

// Función para enviar datos mediante HTTP
esp_err_t http_client_send_data(const tempHumidity_t *sensor_data) {
    if (sensor_data == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    
    // Configuración del cliente HTTP
    esp_http_client_config_t config = {
        .url = SERVER_URL,
        .method = HTTP_METHOD_POST,
        .timeout_ms = 5000,
    };
    
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        ESP_LOGE(TAG, "Error al inicializar el cliente HTTP");
        return ESP_FAIL;
    }
    
    // Usar el método convert_to_json_string de tempHumidity_t
    char post_data[MAX_HTTP_OUTPUT_BUFFER];
    if (convert_to_json_string(sensor_data, &post_data, sizeof(post_data)) != ESP_OK) {
        ESP_LOGE(TAG, "Error al convertir datos a JSON");
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }
    //ESP_LOGI(TAG, "JsonString: %s", post_data);
    
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    
    // Realizar la solicitud HTTP
    esp_err_t err = esp_http_client_perform(client);
    
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %lld",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }
    
    esp_http_client_cleanup(client);
    return err;
}

void http_client_task(tempHumidity_t *sensor_data) {

    ESP_LOGI(TAG, "Enviando datos mediante HTTP...");
    // Enviar datos
    esp_err_t err = http_client_send_data(sensor_data);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error al enviar datos, revise el server");
    }

}
