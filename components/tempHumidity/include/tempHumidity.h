#ifndef TEMPHUMIDITY_H
#define TEMPHUMIDITY_H
#include <freertos/FreeRTOS.h>

#define MAX_HTTP_OUTPUT_BUFFER 256

typedef struct
{
    int humGroud1;
    int humGroud2;
    int humGroud3;
    int humGroud4;
    int humGroud5;
    int humGroud6;
    float temperature;
    float humidity;
    char zona[32];
    char mac_address[18];
} tempHumidity_t;

esp_err_t convert_to_json_string(const tempHumidity_t* data, char (*output)[MAX_HTTP_OUTPUT_BUFFER], size_t size);

#endif // TEMPHUMIDITY_H
