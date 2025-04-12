#include "taskPinController.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define NUM_SENSORS CONFIG_NUM_SENSORS_SOIL // numero de sensores configurados
#define PIN_CONTROLLER_STACK_SIZE 4096  // Define el tamaño de la pila
#define PIN_CONTROLLER_PRIORITY 7         // Define la prioridad de la tarea

gpio_num_t led_pin = GPIO_NUM_2; // Define el pin donde está conectado el LED

esp_err_t pin_controller_config(pin_controller_t *controller, gpio_num_t led_pin) {
    if (controller == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    // Configurar el pin del LED como salida
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << led_pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    esp_err_t ret = gpio_config(&io_conf);
    if (ret != ESP_OK) {
        return ret;
    }

    // Inicializar la estructura del controlador
    controller->led_pin = led_pin;
    controller->led_state = false;
    controller->last_humidity_average = 0.0f;

    // Asegurar que el LED comience apagado
    gpio_set_level(led_pin, 0);
    return ESP_OK;
}

float calculate_humidity_average(const int *humidity_readings, size_t num_readings) {
    if (humidity_readings == NULL || num_readings == 0) {
        return 0.0f;
    }

    float sum = 0.0f;
    for (size_t i = 0; i < num_readings; i++) {
        sum += humidity_readings[i];
    }

    return sum / num_readings;
}

/**
 * @brief Valida si algún valor de humedad está por debajo o es igual al umbral bajo
 * @param sensor_data Estructura que contiene las lecturas de humedad
 * @return true si algún valor está por debajo o es igual al umbral, false en caso contrario
 */
bool is_any_humidity_below_threshold(tempHumidity_t *sensor_data) {
    float reading_groud[CONFIG_NUM_SENSORS_SOIL]; // Cambiar a NUM_SENSORS_SOIL
    // Verificar que el puntero no sea nulo y que haya sensores
    if (sensor_data == NULL) {
        return false; // O manejar el error según sea necesario
    }
    reading_groud[0]= sensor_data->humGroud1;
    reading_groud[1] = sensor_data->humGroud2;
    // Validar cada uno de los valores de humedad
    for (size_t i = 0; i < CONFIG_NUM_SENSORS_SOIL; i++) { // Cambiar a NUM_SENSORS_SOIL
        if (reading_groud[i] <= HUMIDITY_LOW_THRESHOLD) {
            return true; // Retornar true si se encuentra un valor por debajo del umbral
        }
    }

    return false; // Retornar false si no se encontró ningún valor por debajo del umbral
} 

esp_err_t update_led_state(pin_controller_t *controller, tempHumidity_t *sensor_data) {
    if (controller == NULL || sensor_data == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    // Verificar si algún sensor de humedad está por debajo del umbral
    bool should_led_be_on = is_any_humidity_below_threshold(sensor_data);

    // Actualizar el estado del LED solo si es necesario
    if (should_led_be_on != controller->led_state) {
        controller->led_state = should_led_be_on;
        gpio_set_level(controller->led_pin, should_led_be_on ? 1 : 0);
    }

    return ESP_OK;
}

void pin_control(tempHumidity_t *sensor_data) {
    pin_controller_t controller;
    pin_controller_config(&controller, led_pin);
    update_led_state(&controller, sensor_data);
}

