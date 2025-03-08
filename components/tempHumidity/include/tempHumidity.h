#ifndef TEMPHUMIDITY_H
#define TEMPHUMIDITY_H
#include <freertos/FreeRTOS.h>

typedef struct
{
    int humGroud1;
    int humGroud2;
    float temperature;
    float humidity;
} tempHumidity_t;

char* convert_to_json_string(tempHumidity_t* data);


#endif // TEMPHUMIDITY_H
