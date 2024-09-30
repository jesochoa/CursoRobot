
#include <freertos/FreeRTOS.h> //Siempre se pone la primera si no da error
#include <freertos/task.h>
#include <driver/gpio.h>

// Tema 7 controlador pwm con ledc
#include <driver/ledc.h>

constexpr gpio_num_t LED_PIN = GPIO_NUM_2; //Donde conectamos el LED
constexpr gpio_num_t BUTTON_PIN = GPIO_NUM_13; //Donde conectamos el Boton

//Como usamos C++ con esta declaración conseguimos que el void app_main() no cambie de nombre
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
    
    
    // Init GPIO_13 como INPUT
    
    config.mode = GPIO_MODE_INPUT; //LO DECLARO COMO ENTRADA
    config.pin_bit_mask = (1<<BUTTON_PIN); //Corro el 1 13 posiciones
    
    //config.intr_type = GPIO_INTR_DISABLE; // no hace falta ponerla ya que esta igual al LED
    //config.pull_down_en = GPIO_PULLDOWN_DISABLE; // no hace falta ponerla ya que esta igual al LED
    
    config.pull_up_en = GPIO_PULLUP_ENABLE; // Activo la resistencia PULLUP
    gpio_config(&config); // Grabo la configuración del pulsador
    

    //Tema 5 configuro el TIMER lo llamo ledcTimer
    ledc_timer_config_t ledcTimer = {}; //Hasta que no he puesto los corchetes no me ha funcionado
    ledcTimer.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcTimer.freq_hz =  500; //Pruebo con 500Hz
    ledcTimer.duty_resolution = LEDC_TIMER_13_BIT; //Pongo una resolución de 13Bits 
    ledcTimer.clk_cfg = LEDC_AUTO_CLK ; // Pongo automatico
    ledcTimer.timer_num = LEDC_TIMER_0; //Utilizo el primer Timer
    ledc_timer_config(&ledcTimer); //Mando la configuración

    ledc_channel_config_t ledcChannel = {};
    ledcChannel.channel = LEDC_CHANNEL_0; //Elijo el canal 0
    ledcChannel.gpio_num = GPIO_NUM_22;
    ledcChannel.duty = 4096; //Lo dejo 50%
    ledcChannel.timer_sel = LEDC_TIMER_0;
    ledcChannel.intr_type = LEDC_INTR_DISABLE;
    ledcChannel.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcChannel.flags.output_invert = 0; //Pongo 0 para que no me invierta la señal
    ledc_channel_config(&ledcChannel);

    uint32_t cnt = 0;

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

        gpio_set_level(LED_PIN , !gpio_get_level(BUTTON_PIN)); //si se pulsa el led se enciende
        
        vTaskDelay(pdMS_TO_TICKS(10)); // espera para que no de error Task watchdog

        ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0, cnt++);
        ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0);

        if (cnt > 8192)
        {
            cnt = 0;
        }
    }

}