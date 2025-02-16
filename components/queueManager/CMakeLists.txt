idf_component_register(SRCS "queueManager.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "freertos"
                    REQUIRES "esp_common"
                    REQUIRES "tempHumidity")
