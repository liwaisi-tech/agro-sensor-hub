#ifndef _WIFI_H_
#define _WIFI_H_

#include <stdbool.h>
#include "esp_err.h" 

/**
 * @brief Inicializa y conecta el ESP32 a la red WiFi configurada
 *
 * Esta función inicializa el stack WiFi, configura el modo estación,
 * y conecta a la red WiFi usando las credenciales definidas en 
 * el menuconfig (CONFIG_ESP_WIFI_SSID y CONFIG_ESP_WIFI_PASSWORD)
 *
 * @return
 *    - ESP_OK si la conexión es exitosa
 *    - ESP_FAIL si la conexión falla después de los reintentos máximos
 *    - ESP_ERR_INVALID_STATE si ocurre un error inesperado
 */
esp_err_t wifi_action_mode(bool enable);
esp_err_t wifi_init_sta(void);
void get_mac_address(char* mac_str);

#endif /* _WIFI_H_ */
