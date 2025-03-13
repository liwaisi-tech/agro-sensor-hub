#ifndef HTTP_CLIENT_HANDLER_H
#define HTTP_CLIENT_HANDLER_H

#include "esp_err.h"
#include "tempHumidity.h"

/**
 * @brief Inicializa el cliente HTTP
 * @return ESP_OK si la inicialización es exitosa, ESP_FAIL en caso contrario
 */
esp_err_t init_http_client_handler(void);

/**
 * @brief Envía los datos del sensor al servidor mediante HTTP
 * @param sensor_data Puntero a la estructura con los datos del sensor
 * @return ESP_OK si el envío es exitoso, código de error en caso contrario
 */
esp_err_t http_client_send_data(const tempHumidity_t *sensor_data);

#endif // HTTP_CLIENT_HANDLER_H 