idf_component_register(SRCS "readDataSensor.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "dht"
                    REQUIRES "sensorDHT"
                    REQUIRES "yl69"
                    REQUIRES "tempHumidity")
