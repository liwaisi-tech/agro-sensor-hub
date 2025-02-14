#include <stdio.h>
#include "tempHumidity.h"
#include "readDataSensor.h"


static const char *TAG = "agro-sensor-hub";
#define DHT22_TYPE 1
#define YL69_TYPE  2
#define ID1YL69  1
#define ID2YL69  2
#define SENSOR_TYPE DHT_TYPE_AM2301
#define GPIO_PIN2DHT 33
#define GPIO_PIN1DHT 32
#define CHANNEL2_Yl69 ADC_CHANNEL_7
#define CHANNEL1_Yl69 ADC_CHANNEL_6
#define QUEUE_SIZE 10  // Define el tamaño de la cola
QueueHandle_t queue_mediciones;  // Declara la variable global para la cola de mediciones

void app_main(void)
{
    
    while (1) {
        tempHumidity_t data; 
        config_sensor_hub_t cfg_sensor_hub;
        cfg_sensor_hub.pin_sensor_DHT22 = GPIO_PIN1DHT;
        cfg_sensor_hub.sensor_type_DHT22 = SENSOR_TYPE;
        cfg_sensor_hub.sensor_id_Yl69 = ID1YL69;
        cfg_sensor_hub.channel_Yl69 = CHANNEL1_Yl69;
        
        // Leer sensores cajon 1
        read_data_Yl69(&cfg_sensor_hub);
        data.humGroud1 = cfg_sensor_hub.readingYL69.humidity;
        ESP_LOGI(TAG, "Dato recibido humedad suelo cajon %d  valor: %d", cfg_sensor_hub.sensor_id_Yl69,data.humGroud1);
        read_data_DHT22(&cfg_sensor_hub);
        data.humidity1 = cfg_sensor_hub.readingDHT22.humidity;
        data.temperature1 = cfg_sensor_hub.readingYL69.temperature; 
        ESP_LOGI(TAG, "Dato recibido temperatura cajon %d  valor: %d", cfg_sensor_hub.sensor_id_Yl69,data.temperature1);
        ESP_LOGI(TAG, "Dato recibido humedad cajon %d  valor: %d", cfg_sensor_hub.sensor_id_Yl69,data.humidity1);
       // Leer sensores cajon 2
        cfg_sensor_hub.sensor_id_Yl69 = ID2YL69;
        cfg_sensor_hub.channel_Yl69 = CHANNEL2_Yl69;
        cfg_sensor_hub.pin_sensor_DHT22 = GPIO_PIN2DHT;
        read_data_Yl69(&cfg_sensor_hub);
        data.humGroud2 = cfg_sensor_hub.readingYL69.humidity;
         ESP_LOGI(TAG, "Dato recibido humedad suelo cajon %d  valor: %d", cfg_sensor_hub.sensor_id_Yl69,data.humGroud2); 
        //create queue_mediciones buffer
       /*  queue_mediciones = xQueueCreate(QUEUE_SIZE, sizeof(data));
        if (queue_mediciones == NULL) {
        ESP_LOGE(TAG, "Error al crear la cola para YL69");
        return;
         }
        // Enviar datos a la cola
        xQueueSend(queue_mediciones, &data, portMAX_DELAY); */

        // Esperar intervalo de muestreo
        vTaskDelay(pdMS_TO_TICKS(5000)); // 5 segundo
    }
}
