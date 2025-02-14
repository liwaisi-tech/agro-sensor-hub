#ifndef SENSORDHT_H
#define SENSORDHT_H

#include "freertos/FreeRTOS.h"
#include "tempHumidity.h"

#define SENSOR_TYPE  DHT_TYPE_AM2301
#define QHANDLE_DHT NULL

typedef struct {
    float humidity;
    float temperature;
} dht22_reading_t;

typedef struct {
    int pin;
    uint8_t sensor_id;
    QueueHandle_t queue;
    int sensor_type;
    dht22_reading_t reading;
} sensor_config_t;

#define DHT_DEFAULT_CONFIG { \
    .pin = 1,   \
    .sensor_id = 0,     \
    .queue = QHANDLE_DHT,     \
    .sensor_type = SENSOR_TYPE, \
    .reading = {0}   \
}

void sensor_dht22_main(sensor_config_t *configdht);
void read_sensor_task(void *arg);

#endif // SENSORDHT_H