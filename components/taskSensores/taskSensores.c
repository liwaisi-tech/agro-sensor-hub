#include <stdio.h>
#include "taskSensores.h"
#include "queueManager.h"
#include "tempHumidity.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "dht.h"
#include "yl69.h"
#include "wifi.h"
#include <string.h>
#include "esp_sleep.h"

static const char *TAG = "task_sensores";

// Constantes de configuración
#define TASK_SENSORES_STACK_SIZE 4096
#define TASK_SENSORES_PRIORITY 10
#define GPIO_PIN1DHT 4
#define SENSOR_READ_DELAY_MS CONFIG_SENSOR_READ_INTERVAL  // 30 segundos
#define SENSOR_TYPE DHT_TYPE_AM2301

// Configuración de pines y canales
typedef struct {
    adc_channel_t channel;
    groud_sensor_type_t type;
    char sensor_name[32];
} soil_sensor_config_t;

// Canales ADC disponibles
static const adc_channel_t AVAILABLE_ADC_CHANNELS[] = {
    ADC_CHANNEL_5, ADC_CHANNEL_4,
    ADC_CHANNEL_7, ADC_CHANNEL_6
};

#define TOTAL_SENSORS CONFIG_NUM_SENSORS_SOIL

// Cache de configuración de sensores
static soil_sensor_config_t sensors_cache[TOTAL_SENSORS];

// Función para inicializar la configuración de los sensores
static void init_soil_sensors_config(void) {
    char sensor_name[32];
    
    for (int i = 0; i < TOTAL_SENSORS; i++) {
        sensors_cache[i].channel = AVAILABLE_ADC_CHANNELS[i];
        sensors_cache[i].type = CONFIG_SOIL_SENSOR_TYPE;
        snprintf(sensor_name, sizeof(sensor_name), "SensorSoil_%d", i+1);
        snprintf(sensors_cache[i].sensor_name, sizeof(sensor_name),"%s",sensor_name);
    }   

}

// Función para leer un sensor de suelo individual
static esp_err_t read_soil_sensor(const soil_sensor_config_t *config, int *humidity) {
    static yl69_config_t cfg_YL69 = YL69_DEFAULT_CONFIG;
    cfg_YL69.channel = config->channel;
    cfg_YL69.sensor_type = config->type;
    
    esp_err_t ret = yl69_init(&cfg_YL69);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error inicializando sensor %s: %d", config->sensor_name, ret);
        return ret;
    }
    
    yl69_read_percentage(cfg_YL69.channel, humidity, cfg_YL69.sensor_type);
    return ESP_OK;
}

// Función para leer todos los sensores de suelo
esp_err_t read_all_soil_sensors(tempHumidity_t *sensor_data) {
    init_soil_sensors_config();  // Se asegura que los sensores estén inicializados

    int *humidity_values[] = {
        &sensor_data->humGroud1, &sensor_data->humGroud2,
        &sensor_data->humGroud3, &sensor_data->humGroud4,
        &sensor_data->humGroud5, &sensor_data->humGroud6
    };

    esp_err_t ret;
    for (int i = 0; i < TOTAL_SENSORS; i++) {
        ret = read_soil_sensor(&sensors_cache[i], humidity_values[i]);
        if (ret != ESP_OK) return ret;
        
        // Pequeña pausa entre lecturas para evitar sobrecarga
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    return ESP_OK;
}

// Función para la tarea principal de sensores
esp_err_t task_sensores(tempHumidity_t *sensor_data) {

    esp_err_t ret;
    vTaskDelay(pdMS_TO_TICKS(1000));
    // Configuración inicial
    get_mac_address(sensor_data->mac_address);
    strcpy(sensor_data->zona, CONFIG_ZONE);
      // Pequeña pausa entre lecturas para evitar sobrecarga
 
      // Leer sensor de ambiente
    ret = dht_read_float_data(SENSOR_TYPE, GPIO_PIN1DHT, &sensor_data->humidity, &sensor_data->temperature);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error leyendo sensor DHT: %d", ret);
    }
   vTaskDelay(pdMS_TO_TICKS(1000));

    ret = read_all_soil_sensors(sensor_data);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Lecturas de suelo: [%d,%d,%d,%d]%%",
                 sensor_data->humGroud1, sensor_data->humGroud2,
                 sensor_data->humGroud3, sensor_data->humGroud4);
    } else {
        ESP_LOGE(TAG, "Error en lectura de sensores de suelo");
    }

    return ret;
}
