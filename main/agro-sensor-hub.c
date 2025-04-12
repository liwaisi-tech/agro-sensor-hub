#include <stdio.h>
#include "tempHumidity.h"
#include "taskPinController.h"
#include "taskHttpClientHandler.h"
#include "taskSensores.h"
#include "wifi.h"
#include <esp_log.h>
#include "nvs_flash.h"
#include "esp_sleep.h"
#include "esp_wake_stub.h"
#include <time.h>
#include <sys/time.h>
#include "driver/rtc_io.h"  // Asegúrate de incluir este encabezado para las funciones RTC GPIO
#include "esp_wifi.h"  // Asegúrate de incluir este encabezado para usar esp_wifi_stop
#include "driver/gpio.h" 

static const char *TAG = "agro-sensor-hub";

#define SENSOR_TYPE DHT_TYPE_AM2301
#define GPIO_PIN1DHT 32

// Variables RTC que persisten durante el deep sleep
static RTC_DATA_ATTR struct timeval sleep_enter_time;
tempHumidity_t data; 



void app_main(void) {
    // Inicializar NVS (Non-volatile storage) necesario para WiFi
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    struct timeval now;
    gettimeofday(&now, NULL);
    int sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + 
                        (now.tv_usec - sleep_enter_time.tv_usec) / 1000;
    
    // Mostrar información de despertar
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
        printf("Despertar desde temporizador. Tiempo en deep sleep: %dms\n", sleep_time_ms);
    }

    // Inicializar WiFi
    ret  = wifi_init_sta();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize system");
        return;
    }

    ret = task_sensores(&data);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize system");
        return;
    }
  
    pin_control(&data);
    http_client_task(&data);
    vTaskDelay(pdMS_TO_TICKS(100));
    esp_wifi_stop();
// Configurar despertar por temporizador (60 segundos)
    esp_sleep_enable_timer_wakeup(CONFIG_WAKE_UP_TIME * 1000000);
   
    // Configurar el LED de alerta para mantener su estado durante el deep sleep
    
    
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
        printf("Despertar con temporizador. Tiempo de sueño profundo: %dms\n", sleep_time_ms);
    }
    // Esperar
        vTaskDelay(pdMS_TO_TICKS(1000));

    // En ESP32, aislar GPIO12 para reducir consumo en módulos como ESP32-WROVER
    rtc_gpio_isolate(GPIO_NUM_12);
    
    
    // Establecer la función wake stub
    extern void rtc_wake_stub(void);
    esp_set_deep_sleep_wake_stub(&rtc_wake_stub);
    
    // Guardar tiempo actual antes de entrar en deep sleep
    printf("Entrando en deep sleep\n");
    gettimeofday(&sleep_enter_time, NULL);
    
    // Entrar en deep sleep
    esp_deep_sleep_start();
}

