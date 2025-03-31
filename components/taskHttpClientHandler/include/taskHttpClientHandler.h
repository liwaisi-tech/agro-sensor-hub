#ifndef HTTP_CLIENT_HANDLER_H
#define HTTP_CLIENT_HANDLER_H

#include "esp_err.h"
#include "tempHumidity.h"

/**
 * @brief Envía los datos del sensor al servidor mediante HTTP
 * @param sensor_data Puntero a la estructura con los datos del sensor
 * @return ESP_OK si el envío es exitoso, código de error en caso contrario
 */
void http_client_task(tempHumidity_t *sensor_data);

#endif // HTTP_CLIENT_HANDLER_H 