#ifndef QUEUE_MANAGER_H
#define QUEUE_MANAGER_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_err.h"


// Handles para las colas
extern QueueHandle_t queue_mediciones;
extern QueueHandle_t queue_control_pines;
extern QueueHandle_t queue_websocket;

// Función de inicialización
esp_err_t init_queues(void);

#endif // QUEUE_MANAGER_H
