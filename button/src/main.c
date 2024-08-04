#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define LED_PIN 21
#define Forward 20
#define Backward 19

#define SeatForeWard 2
#define SeatBackWard 3
#define SeatLegBack_up 8
#define SeatAngleBack 9
#define SeatLegBack_down 4
#define SeatAngleFront 5
#define SeatLegFront_up 6
#define SeatLegFront_down 7

int main() {

    //const uint BUTTON_PIN = 2; 
//
    //gpio_init(BUTTON_PIN);
    //gpio_set_dir(BUTTON_PIN, GPIO_IN);
    //gpio_pull_up(BUTTON_PIN);

    for(int pincnt = 2; pincnt < 10 ; pincnt++)
    {
        gpio_init(pincnt);
        gpio_set_dir(pincnt, GPIO_IN);
        gpio_pull_up(pincnt);
    }

    // GPIO 초기화
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    gpio_init(Forward);
    gpio_set_dir(Forward, GPIO_OUT);
    gpio_init(Backward);
    gpio_set_dir(Backward, GPIO_OUT);

    

    bool button_pressed_f[4] = {false,false,false,false};
    bool button_pressed_b[4] = {false,false,false,false};

    // 레지스터 제어 예제: LED 깜박이기
    while (true) {

        button_pressed_f[0] = !gpio_get(SeatForeWard);
        button_pressed_f[1] = !gpio_get(SeatLegBack_up);
        button_pressed_f[2] = !gpio_get(SeatAngleFront);
        button_pressed_f[3] = !gpio_get(SeatLegFront_up);
        
        button_pressed_b[0] = !gpio_get(SeatBackWard);
        button_pressed_b[1] = !gpio_get(SeatLegBack_down);
        button_pressed_b[2] = !gpio_get(SeatAngleBack);
        button_pressed_b[3] = !gpio_get(SeatLegFront_down);

        if( button_pressed_f[1] || button_pressed_f[2] || button_pressed_f[3])
        {
            // GPIO 레지스터를 통해 LED 켜기
            gpio_put(Forward, 1);
            //gpio_put(Backward, 0);
            //sleep_ms(500);
        }
        else
        {
            gpio_put(Forward, 0);
            //sleep_ms(500);
        }
        if(button_pressed_b[0] || button_pressed_b[1] || button_pressed_b[2] || button_pressed_b[3])
        {
            //gpio_put(Forward, 0);
            gpio_put(Backward, 1);
            //sleep_ms(500);
        }
        else
        {
            gpio_put(Backward, 0);
            //sleep_ms(500);
        }
        
    }
    return 0;
}
