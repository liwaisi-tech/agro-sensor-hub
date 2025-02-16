idf_component_register(SRCS "queue_manager.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "freertos"
                    REQUIRES "esp_common"
                    REQUIRES "tempHumidity")
