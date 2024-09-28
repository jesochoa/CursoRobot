
#include <freertos/FreeRTOS.h> //Siempre se pone la primera si no da error
#include <freertos/task.h>
#include <driver/gpio.h>

//usamos el led interno que esta en el GPIO_2
constexpr gpio_num_t LED_PIN = GPIO_NUM_2;
constexpr gpio_num_t BUTTOM_PIN = GPIO_NUM_13; //Boton

//Como usamos C++ con esta función conseguimos que el void app_main() no cambie de nombre
extern "C" void app_main();

void app_main() 
{
    // Setup
    gpio_config_t config;
    config.mode = GPIO_MODE_OUTPUT; // Lo declaro como salida
    config.pin_bit_mask = (1<<LED_PIN); // Desplazo 2 posiciones el 1
    config.intr_type = GPIO_INTR_DISABLE; // Desabilito la interrupción
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&config); //Grabo la configuracion

    //
    // Init GPIO_13 como INPUT
    config.mode = GPIO_MODE_INPUT; //LO DECLARO COMO ENTRADA
    config.pin_bit_mask = (1<<BUTTOM_PIN); //Corro el 1 13 posiciones
    config.intr_type = GPIO_INTR_DISABLE; // no hace falta ponerla ya que esta igual al LED
    config.pull_down_en = GPIO_PULLDOWN_DISABLE; // no hace falta ponerla ya que esta igual al LED
    config.pull_up_en = GPIO_PULLUP_ENABLE; // Activo la resistencia PULLUP
    gpio_config(&config); // Grabo la configuración del pulsador
    
    // Loop
    while(1)
    {   

        // Set LED_PIN to INPUT value.
        // If INPUT == 1 then LED_PIN = 1
        // If INPUT == 0 then LED_PIN = 0
        // este codigo es para encender y apagar el LED
        // gpio_set_level(LED_PIN, 1); // enciende el LED
        // vTaskDelay(pdMS_TO_TICKS(1000)); //ESPERA 1 segundo
        // gpio_set_level(LED_PIN, 0);
        // vTaskDelay(pdMS_TO_TICKS(1000));

        gpio_set_level(LED_PIN , !gpio_set_level(BUTTOM_PIN));
    }

}