#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define BUZ_PIN 10
#define BOTAO_A 5
#define BOTAO_B 6
#define V_PIN 13
#define G_PIN 11
#define B_PIN 12

TaskHandle_t ledTaskHandle = NULL;
TaskHandle_t buzzerTaskHandle = NULL;

void iniciar(){
    gpio_init(V_PIN); 
    gpio_set_dir(V_PIN, GPIO_OUT);

    gpio_init(G_PIN); 
    gpio_set_dir(G_PIN, GPIO_OUT);

    gpio_init(B_PIN); 
    gpio_set_dir(B_PIN, GPIO_OUT);

    gpio_init(BOTAO_A); 
    gpio_set_dir(BOTAO_A, GPIO_IN); 
    gpio_pull_up(BOTAO_A);

    gpio_init(BOTAO_B); 
    gpio_set_dir(BOTAO_B, GPIO_IN); 
    gpio_pull_up(BOTAO_B);

    gpio_set_function(BUZ_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZ_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (200 * 4096));
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(BUZ_PIN, 0);
}


void task_led(void *pvParameters) {
    int cor = 0;

   while (true) {
        if (cor == 0){
            gpio_put(V_PIN, 1);
            gpio_put(G_PIN, 0);
            gpio_put(B_PIN, 0);
            }
        else if (cor == 1){
            gpio_put(V_PIN, 0);
            gpio_put(G_PIN, 1);
            gpio_put(B_PIN, 0);
        }
        else {
            gpio_put(V_PIN, 0);
            gpio_put(G_PIN, 0);
            gpio_put(B_PIN, 1);
        }

        cor = (cor + 1) % 3; 
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


void task_buzzer(void *pvParameters) {
    while (1) {
        uint slice_num = pwm_gpio_to_slice_num(BUZ_PIN);
        pwm_set_gpio_level(BUZ_PIN, 2048);
        vTaskDelay(pdMS_TO_TICKS(100));
        pwm_set_gpio_level(BUZ_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(100));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


void task_botoes(void *param) {
    bool anteriorA = true;
    bool anteriorB = true;

    while (1) {
        bool atualA = gpio_get(BOTAO_A);
        bool atualB = gpio_get(BOTAO_B);

        if (!atualA && anteriorA) {
            if (eTaskGetState(ledTaskHandle) == eSuspended)
                vTaskResume(ledTaskHandle);
            else
                vTaskSuspend(ledTaskHandle);
        }

        if (!atualB && anteriorB) {
            if (eTaskGetState(buzzerTaskHandle) == eSuspended)
                vTaskResume(buzzerTaskHandle);
            else
                vTaskSuspend(buzzerTaskHandle);
        }

        anteriorA = atualA;
        anteriorB = atualB;

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


int main() {
    stdio_init_all();
    iniciar();

    xTaskCreate(task_botoes, "Botão Task", 256, NULL, 2, NULL); 
    xTaskCreate(task_led, "LED Task", 256, NULL, 1, &ledTaskHandle);
    xTaskCreate(task_buzzer, "Buzzer Task", 256, NULL, 1, &buzzerTaskHandle);
    
    vTaskStartScheduler();
}