#include "yl69.h"
#include "esp_log.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "YL69";
static adc_oneshot_unit_handle_t adc_handle;
static bool adc_initialized = false;


#define SENSOR_MAX_VALUE 4095
#define SENSOR_MIN_VALUE 0
#define HUMIDITY_MAX 100
#define HUMIDITY_MIN 0

static int map_value(int value) {
    return (value - SENSOR_MIN_VALUE) * (HUMIDITY_MIN - HUMIDITY_MAX) / 
           (SENSOR_MAX_VALUE - SENSOR_MIN_VALUE) + HUMIDITY_MAX;
}

esp_err_t yl69_init(yl69_config_t *config) {
    if (!adc_initialized) {
        // Inicializar ADC solo una vez
        adc_oneshot_unit_init_cfg_t init_config = {
            .unit_id = config->unit,
            .ulp_mode = false
        };
        ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));
        adc_initialized = true;
    }
    
    // Configurar el canal específico
    adc_oneshot_chan_cfg_t chan_config = {
        .atten = config->atten,
        .bitwidth = config->bitwidth
    };
    
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, config->channel, &chan_config));
    return ESP_OK;
}

int yl69_read_raw(adc_channel_t channel) {
    if (!adc_initialized) {
        ESP_LOGE(TAG, "YL69 no inicializado");
        return -1;
    }

    int raw_value;
    esp_err_t ret = adc_oneshot_read(adc_handle, channel, &raw_value);
    
    // Manejo de errores
    if (ret == ESP_ERR_TIMEOUT) {
        ESP_LOGE(TAG, "Error de tiempo de espera al leer el ADC en el canal %d", channel);
        return -1; // O manejar el error de otra manera
    } else if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error al leer el ADC: %d", ret);
        return -1; // O manejar el error de otra manera
    }

    return raw_value;
}

void yl69_read_percentage(adc_channel_t channel, int *humidity) {
    *humidity = map_value(yl69_read_raw(channel));
}

