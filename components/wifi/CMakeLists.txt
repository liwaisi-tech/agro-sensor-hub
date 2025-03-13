idf_component_register(
                    SRCS "wifi.c"          # Tu archivo de implementación
                    INCLUDE_DIRS "include" # Directorio donde está wifi.h
                    REQUIRES "esp_wifi"    # Dependencias necesarias
                    "esp_event"
                    "nvs_flash"
                    "esp_netif"
                    "freertos"
)