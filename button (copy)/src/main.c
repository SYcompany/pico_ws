#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"

#define Forward 20
#define Backward 19


int main() {

    const uint BUTTON_PIN = 2; 

    // GPIO 초기화
    stdio_init_all();


    
    
    gpio_init(1);
    gpio_set_dir(1, GPIO_IN);
    gpio_pull_up(1);

    gpio_init(2);
    gpio_set_dir(2, GPIO_IN);
    gpio_pull_up(2);

    gpio_init(3);
    gpio_set_dir(3, GPIO_IN);
    gpio_pull_up(3);

    gpio_init(4);
    gpio_set_dir(4, GPIO_IN);
    gpio_pull_up(4);

    gpio_init(5);
    gpio_set_dir(5, GPIO_IN);
    gpio_pull_up(5);

    gpio_init(6);
    gpio_set_dir(6, GPIO_IN);
    gpio_pull_up(6);

    gpio_init(7);
    gpio_set_dir(7, GPIO_IN);
    gpio_pull_up(7);

    gpio_init(8);
    gpio_set_dir(8, GPIO_IN);
    gpio_pull_up(8);

    gpio_init(9);
    gpio_set_dir(9, GPIO_IN);
    gpio_pull_up(9);

    gpio_init(10);
    gpio_set_dir(10, GPIO_IN);
    gpio_pull_up(10);

    gpio_init(11);
    gpio_set_dir(11, GPIO_IN);
    gpio_pull_up(11);

    gpio_init(12);
    gpio_set_dir(12, GPIO_IN);
    gpio_pull_up(12);

    gpio_init(13);
    gpio_set_dir(13, GPIO_IN);
    gpio_pull_up(13);

    gpio_init(14);
    gpio_set_dir(14, GPIO_IN);
    gpio_pull_up(14);

    gpio_init(15);
    gpio_set_dir(15, GPIO_IN);
    gpio_pull_up(15);

    gpio_init(16);
    gpio_set_dir(16, GPIO_IN);
    gpio_pull_up(16);

    gpio_init(17);
    gpio_set_dir(17, GPIO_IN);
    gpio_pull_up(17);

    gpio_init(18);
    gpio_set_dir(18, GPIO_IN);
    gpio_pull_up(18);

    gpio_init(19);
    gpio_set_dir(19, GPIO_IN);
    gpio_pull_up(19);

    gpio_init(20);
    gpio_set_dir(20, GPIO_IN);
    gpio_pull_up(20);

    bool button_pressed;
    // 레지스터 제어 예제: LED 깜박이기
    while (true) {

        
        
        if(!gpio_get(1))
        {
            // GPIO 레지스터를 통해 LED 켜기
            printf("1 on\n");
            //sleep_ms(500);
        }
        else{}
        if(!gpio_get(2))
        {
            printf("2 on\n");
            //sleep_ms(500);
        }
        else{} 
        if(!gpio_get(3))
        {
            printf("3 on\n");
            sleep_ms(500);
        }
        else{}
        if(!gpio_get(4))
        {
            printf("4 on\n");
            sleep_ms(500);
        }
        else{}
        if(!gpio_get(5))
        {
            // GPIO 레지스터를 통해 LED 켜기
            printf("5 on\n");
            sleep_ms(500);
        }
        else{}
        if(!gpio_get(6))
        {
            printf("6 on\n");
            sleep_ms(500);
        }
        else{}
        if(!gpio_get(7))
        {
            printf("7 on\n");
            sleep_ms(500);
        }
        else{}
        if(!gpio_get(8))
        {
            printf("8 on\n");
            sleep_ms(500);
        }
        else
        {}

        if(!gpio_get(9))
        {
            // GPIO 레지스터를 통해 LED 켜기
            printf("9 on\n");
            sleep_ms(500);
        }
        else{}

        if(!gpio_get(10))
        {
            printf("10 on\n");
            sleep_ms(500);
        }
        else{} 

        if(!gpio_get(11))
        {
            printf("11 on\n");
            sleep_ms(500);
        }
        else{} 

        if(!gpio_get(12))
        {
            printf("12 on\n");
            sleep_ms(500);
        }
        else{} 
        
        if(!gpio_get(15))
        {
            // GPIO 레지스터를 통해 LED 켜기
            printf("15 on\n");
            sleep_ms(500);
        }
        else{}
        
        if(!gpio_get(16))
        {
            printf("16 on\n");
            sleep_ms(500);
        }
        else{}
        
        //if(!gpio_get(15))
        //{
        //    printf("15 on\n");
        //    sleep_ms(500);
        //}
        //else{}
        //
        //if(!gpio_get(16))
        //{
        //    printf("16 on\n");
        //    sleep_ms(500);
        //}
        //else{}
        
        if(!gpio_get(17))
        {
            // GPIO 레지스터를 통해 LED 켜기
            printf("17 on\n");
            sleep_ms(500);
        }
        else{}
        
        if(!gpio_get(18))
        {
            printf("18 on\n");
            sleep_ms(500);
        }
        else{}

        if(!gpio_get(19))
        {
            printf("19 on\n");
            sleep_ms(500);
        }
        else{}

        if(!gpio_get(20))
        {
            printf("20 on\n");
            sleep_ms(500);
        }
        else{
            //printf("off");
            //sleep_ms(500);
        }
        
    }
    return 0;
}
