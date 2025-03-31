#include <inttypes.h>
#include "esp_sleep.h"
#include "esp_cpu.h"
#include "esp_rom_sys.h"
#include "esp_wake_stub.h"
#include "driver/rtc_io.h"
#include "sdkconfig.h"
#include "taskPinController.h"
#include "taskSensores.h"
#include "tempHumidity.h"

// Variables que se mantienen en memoria RTC durante el deep sleep
static RTC_DATA_ATTR uint32_t wake_count = 0;
static RTC_DATA_ATTR uint32_t last_server_update_count = 0;

// Define el pin del LED de alerta (asegúrate de usar un pin RTC_GPIO)
#define ALERT_LED_PIN RTC_GPIO_NUM_27

// Número de despertares antes de enviar datos al servidor (60 = 10 minutos si el tiempo de despertar es 10 segundos)
#define SERVER_UPDATE_INTERVAL 10

void rtc_wake_stub(void)
{
    // Incrementa el contador de despertares
    wake_count++;
        // Obtiene la causa del despertar
    uint32_t wakeup_cause = esp_wake_stub_get_wakeup_cause();
       // Imprime información de diagnóstico
    ESP_RTC_LOGI("Wake stub: count=%d, cause=%d", 
                 wake_count, wakeup_cause);
    
    // Verificar si es momento de enviar datos al servidor
    bool send_to_server = (wake_count - last_server_update_count) >= SERVER_UPDATE_INTERVAL;
    
    if (send_to_server) {
        // Si es momento de enviar datos al servidor, dejamos que el sistema se inicie completamente
        // Guardar el contador actual como el último envío
        last_server_update_count = wake_count;
        ESP_RTC_LOGI("Es momento de prender todo para enviar"); 
        // Configurar el wake stub por defecto para permitir arranque normal
        esp_default_wake_deep_sleep();
        return;
    }
    
    // Si no es momento de enviar datos, volvemos a dormir inmediatamente
    
    // Configurar tiempo para próximo despertar (10 segundos = 10,000,000 us)
    esp_wake_stub_set_wakeup_time(6 * 10000000);
    
    // Imprimir mensaje antes de volver a dormir
    ESP_RTC_LOGI("Wake stub: volviendo a dormir");
    
    // Volver a dormir directamente desde el wake stub
    esp_wake_stub_sleep(&rtc_wake_stub);
}