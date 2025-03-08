#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>
#include "tempHumidity.h"


// Function to convert TempHumidity struct to JSON string
char* convert_to_json_string(tempHumidity_t* data) {
    // Create a cJSON object
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "humGroud1", data->humGroud1);
    cJSON_AddNumberToObject(json, "humGroud2", data->humGroud2);
    cJSON_AddNumberToObject(json, "temperature", data->temperature);
    cJSON_AddNumberToObject(json, "humidity", data->humidity);

    // Convert cJSON object to string
    char *json_string = cJSON_Print(json);

    // Clean up cJSON object
    cJSON_Delete(json);

    return json_string; // Remember to free this string after use
}