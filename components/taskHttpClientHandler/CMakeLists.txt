idf_component_register(SRCS "taskHttpClientHandler.c"
    INCLUDE_DIRS "include"
    REQUIRES "freertos"
    "esp_common"
    "queueManager"
    "tempHumidity"
    "esp_http_client"
) 