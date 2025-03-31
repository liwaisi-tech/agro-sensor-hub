/**
 * @file taskPinController.h
 * @brief Control de LED basado en lecturas de humedad del suelo
 */

#ifndef TASK_PIN_CONTROLLER_H
#define TASK_PIN_CONTROLLER_H

#include "esp_err.h"
#include "driver/gpio.h"
#include "tempHumidity.h"

// Constantes para los umbrales de humedad
#define HUMIDITY_LOW_THRESHOLD    50
#define HUMIDITY_HIGH_THRESHOLD_MAX   75

// Estructura para mantener el estado del controlador
typedef struct {
    gpio_num_t led_pin;              // Número del pin GPIO para el LED
    bool led_state;                  // Estado actual del LED
    float last_humidity_average;     // Último promedio de humedad calculado
} pin_controller_t;

// Función para iniciar la tarea
void pin_control(tempHumidity_t *sensor_data);
/**
 * @brief Inicializa el controlador del LED
 * @param controller Puntero a la estructura del controlador
 * @param led_pin Número del pin GPIO para el LED
 * @return ESP_OK si la inicialización fue exitosa
 */
esp_err_t pin_controller_config(pin_controller_t *controller, gpio_num_t led_pin);

/**
 * @brief Calcula el promedio de las lecturas de humedad
 * @param humidity_readings Array con las lecturas de humedad
 * @param num_readings Número de lecturas en el array
 * @return Promedio de las lecturas
 */
float calculate_humidity_average(const int *humidity_readings, size_t num_readings);

/**
 * @brief Actualiza el estado del LED basado en el promedio de humedad
 * @param controller Puntero a la estructura del controlador
 * @param humidity_average Promedio actual de humedad
 * @return ESP_OK si la actualización fue exitosa
 */
esp_err_t update_led_state(pin_controller_t *controller, tempHumidity_t *sensor_data);

#endif // TASK_PIN_CONTROLLER_H