idf_component_register(SRCS "taskPinController.c"
    INCLUDE_DIRS "include"
    REQUIRES "freertos"
    REQUIRES "esp_common"
    REQUIRES "tempHumidity"
    REQUIRES "esp_driver_gpio"
) 