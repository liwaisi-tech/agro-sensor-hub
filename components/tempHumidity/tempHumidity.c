#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>
#include "tempHumidity.h"


// Function to convert TempHumidity struct to JSON string
char* convert_to_json_string(tempHumidity_t* data) {
    // Create a cJSON object
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "humCajon1", data->humidity1);
    cJSON_AddNumberToObject(json, "tempCajon2", data->temperature2);
    cJSON_AddNumberToObject(json, "humCajon2", data->humidity2);
    cJSON_AddNumberToObject(json, "humSueloCajon1", data->humGroud1);
    cJSON_AddNumberToObject(json, "humSueloCajon2", data->humGroud2);

    // Convert cJSON object to string
    char *json_string = cJSON_Print(json);

    // Clean up cJSON object
    cJSON_Delete(json);

    return json_string; // Remember to free this string after use
}