#ifndef TASK_SENSORES_H
#define TASK_SENSORES_H

#include "esp_err.h"
#include "tempHumidity.h"

esp_err_t read_all_soil_sensors(tempHumidity_t *sensor_data);
esp_err_t task_sensores(tempHumidity_t *sensor_data);

#endif // TASK_SENSORES_H
