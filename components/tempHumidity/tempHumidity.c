#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>
#include "tempHumidity.h"


// Function to convert TempHumidity struct to JSON string
esp_err_t convert_to_json_string(const tempHumidity_t *data, char (*buffer)[MAX_HTTP_OUTPUT_BUFFER], size_t buffer_size) {
    if (data == NULL || buffer == NULL || buffer_size == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    // Create a cJSON object
    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        return ESP_FAIL;
    }

    cJSON_AddNumberToObject(json, "ground_sensor_1", data->humGroud1);
    cJSON_AddNumberToObject(json, "ground_sensor_2", data->humGroud2);
    cJSON_AddNumberToObject(json, "ground_sensor_3", data->humGroud3);
    cJSON_AddNumberToObject(json, "ground_sensor_4", data->humGroud4);
    cJSON_AddNumberToObject(json, "ground_sensor_5", data->humGroud5);
    cJSON_AddNumberToObject(json, "ground_sensor_6", data->humGroud6);
    cJSON_AddNumberToObject(json, "env_temperature", data->temperature);
    cJSON_AddNumberToObject(json, "env_humidity", data->humidity);
    cJSON_AddStringToObject(json, "zone", data->zona);
    cJSON_AddStringToObject(json, "mac_address", data->mac_address);

    // Convert cJSON object to string
    char *json_string = cJSON_PrintUnformatted(json);
    if (json_string == NULL) {
        cJSON_Delete(json);
        return ESP_FAIL;
    }

    // Copy to provided buffer with size checking
    if (strlen(json_string) >= buffer_size) {
        free(json_string);
        cJSON_Delete(json);
        return ESP_ERR_INVALID_SIZE;
    }

    strcpy(*buffer, json_string);

    // Clean up
    free(json_string);
    cJSON_Delete(json);

    return ESP_OK;
}