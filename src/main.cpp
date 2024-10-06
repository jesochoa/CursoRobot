
#include <stdio.h>
#include <freertos/FreeRTOS.h> //Siempre se pone la primera si no da error
#include <freertos/task.h>
#include <driver/gpio.h>


// Tema 7 controlador pwm con ledc
#include <driver/ledc.h>

// constexpr gpio_num_t LED_PIN = GPIO_NUM_2; //Donde conectamos el LED
constexpr gpio_num_t BUTTON_PIN = GPIO_NUM_13; //Donde conectamos el Boton

//Como usamos C++ con esta declaración conseguimos que el void app_main() no cambie de nombre
extern "C" void app_main();

void app_main() 
{
    // Setup
    
    gpio_config_t config;
    /*
    config.mode = GPIO_MODE_OUTPUT; // Lo declaro como salida
    config.pin_bit_mask = (1<<LED_PIN); // Desplazo 2 posiciones el 1
    config.intr_type = GPIO_INTR_DISABLE; // Desabilito la interrupción
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&config); //Grabo la configuracion
    */
    
    // Init GPIO_13 como INPUT
    config.mode = GPIO_MODE_INPUT; //LO DECLARO COMO ENTRADA
    config.pin_bit_mask = (1<<BUTTON_PIN); //Corro el 1 13 posiciones par elejir GPIO13
    
    config.intr_type = GPIO_INTR_DISABLE; // no hace falta ponerla ya que esta igual al LED
    config.pull_down_en = GPIO_PULLDOWN_DISABLE; // no hace falta ponerla ya que esta igual al LED
    
    config.pull_up_en = GPIO_PULLUP_ENABLE; // Activo la resistencia PULLUP
    gpio_config(&config); // Grabo la configuración del pulsador
    

    //Tema 5 configuro el TIMER lo llamo ledcTimer
    ledc_timer_config_t ledcTimer = {}; //Hasta que no he puesto los corchetes no me ha funcionado
    ledcTimer.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcTimer.freq_hz =  50; //Pruebo con 500Hz
    ledcTimer.duty_resolution = LEDC_TIMER_10_BIT; //Pongo una resolución de 10Bits 
    ledcTimer.clk_cfg = LEDC_AUTO_CLK ; // Pongo automatico
    ledcTimer.timer_num = LEDC_TIMER_0; //Utilizo el primer Timer
    ledc_timer_config(&ledcTimer); //Mando la configuración

    //configuro el PIO22 para el LED
    ledc_channel_config_t ledcChannel = {};
    ledcChannel.channel = LEDC_CHANNEL_0; //Elijo el canal 0
    ledcChannel.gpio_num = GPIO_NUM_22;
    ledcChannel.duty = 0; //Lo dejo 50% 4096 lo cambio 0%
    ledcChannel.timer_sel = LEDC_TIMER_0;
    ledcChannel.intr_type = LEDC_INTR_DISABLE;
    ledcChannel.speed_mode = LEDC_LOW_SPEED_MODE;
    ledcChannel.flags.output_invert = 0; //Pongo 0 para que no me invierta la señal
    ledc_channel_config(&ledcChannel);

    //Tema 5 configuro el TIMER lo llamo ledcTimer
    ledc_timer_config_t servoTimer = {}; //Hasta que no he puesto los corchetes no me ha funcionado
    servoTimer.speed_mode = LEDC_LOW_SPEED_MODE;
    servoTimer.freq_hz =  50; //Pruebo con 500Hz
    servoTimer.duty_resolution = LEDC_TIMER_10_BIT; //Pongo una resolución de 10Bits 
    servoTimer.clk_cfg = LEDC_AUTO_CLK ; // Pongo automatico
    servoTimer.timer_num = LEDC_TIMER_1; //Utilizo el primer Timer
    ledc_timer_config(&servoTimer); //Mando la configuración
    
    //Configuro el GPIO 16  para el servoChanelc_channel_config_t servoChanel = {};
    ledc_channel_config_t servoChanel = {};
    servoChanel.channel = LEDC_CHANNEL_1; //Elijo el canal 0
    servoChanel.gpio_num = GPIO_NUM_16;
    servoChanel.duty = 0;
    servoChanel.timer_sel = LEDC_TIMER_1;
    servoChanel.intr_type = LEDC_INTR_DISABLE;
    servoChanel.speed_mode = LEDC_LOW_SPEED_MODE;
    servoChanel.flags.output_invert = 0; //Pongo 0 para que no me invierta la señal
    ledc_channel_config(&servoChanel);

    uint32_t cnt = 0;
    uint32_t cnt_1 = 0 ;

    printf("PREPARADOS \n");
    printf("el pulsador vale = %d \n", gpio_get_level(BUTTON_PIN));
    vTaskDelay(pdMS_TO_TICKS(2000)); // ESPERA 2 SEGUNDOS
    printf("El contador cnt = %ld \n", cnt);
    printf("EMPIEZA EL PROGRAMA \n");
    
    // no funciona la entrada de teclado
    // printf("Introduce la primera opcion ");
    // scanf("%ld",&cnt);
    // vTaskDelay(pdMS_TO_TICKS(2000)); // ESPERA 2 SEGUNDOS
    // printf("El contador vale ahora cnt = %ld \n", cnt);

    // Loop
    while(1)
    {   

        // gpio_set_level(LED_PIN, 1); // enciende el LED
        // vTaskDelay(pdMS_TO_TICKS(1000)); //ESPERA 1 segundo
       

        // gpio_set_level(LED_PIN , !gpio_get_level(BUTTON_PIN)); //si se pulsa el led se enciende
        
        /*
        Al estar configurado el GPIO en "config.pull_up_en = GPIO_PULLUP_ENABLE" el valor que da
        gpio_get_level(BUTTON_PIN) es 1 asi que para saber si le hemos pulsado debemos negarlo
        */
        if (!gpio_get_level(BUTTON_PIN))
        {
            while (!gpio_get_level(BUTTON_PIN)){vTaskDelay(pdMS_TO_TICKS(10));} //para arreglar el rebote de pulsador
            
            cnt++;
            printf("Vale el contador cnt = %ld \n", cnt);
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // espera para que no de error Task watchdog

        switch (cnt)
        {
        case 0 :
            //apago el led
            ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0, 0);
            ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0); 
            //pongo el servo a 0º
            ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1, 54);
            ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1); 
            break;

        case 1 :
            ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0, 1024);
            ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0);
            //pongo el servo a 90º
            ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1, 74);
            ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1);
            break;

        case 2 :
            ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0, 512);
            ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0);
            //pongo el servo a -90º
            ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1, 88);
            ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1);
            break;

        case 3 :
            ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0, 100);
            ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0);
            //pongo el servo a 90º
            ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1, 100);
            ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1); 
            break;
        
        case 4 :
            cnt_1 = 0 ;
            while (cnt_1 < 1024)
            {
                vTaskDelay(pdMS_TO_TICKS(10)); // menos de 10 va super rapido
                ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0, cnt_1++);
                cnt_1 = cnt_1+4; //para que valla mas rapido el LED
                ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0);
            }
            cnt = 0 ;
            printf("Pone a cero el contador \n");
            printf(" cnt vale = %ld \n", cnt);
            break;

        
        default:
            
            break;
        }
    }
}