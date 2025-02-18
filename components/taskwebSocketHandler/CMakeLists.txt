idf_component_register(SRCS "taskWebSocketHandler.c"
    INCLUDE_DIRS "include"
    REQUIRES "freertos"
    REQUIRES "esp_common"
    REQUIRES "queueManager"
    REQUIRES "tempHumidity"
) 