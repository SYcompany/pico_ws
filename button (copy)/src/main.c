#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define LED_PIN 21
#define Forward 20
#define Backward 19


int main() {

    const uint BUTTON_PIN = 2; 

    // GPIO 초기화
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    gpio_init(Forward);
    gpio_set_dir(Forward, GPIO_OUT);
    gpio_init(Backward);
    gpio_set_dir(Backward, GPIO_OUT);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    bool button_pressed;
    // 레지스터 제어 예제: LED 깜박이기
    while (true) {

        button_pressed = !gpio_get(BUTTON_PIN);
        
        if(button_pressed)
        {
            // GPIO 레지스터를 통해 LED 켜기
            gpio_put(Forward, 1);
            gpio_put(Backward, 0);
            sleep_ms(500);
        }
        else{
            gpio_put(Forward, 0);
            gpio_put(Backward, 1);
            sleep_ms(500);
        }
        
    }
    return 0;
}
