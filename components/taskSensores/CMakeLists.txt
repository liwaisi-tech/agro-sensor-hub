idf_component_register(SRCS "taskSensores.c"
    INCLUDE_DIRS "include"
    REQUIRES "freertos"
    REQUIRES "esp_common"
    REQUIRES "queueManager"
    REQUIRES "tempHumidity"
    REQUIRES "dht"
    REQUIRES "yl69"
)
