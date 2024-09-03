#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include <string.h>

#define Forward 10
#define Backward 11

#define SeatForeWard 3
#define SeatBackWard 6

int main() {

    gpio_init(SeatForeWard);
    gpio_set_dir(SeatForeWard, GPIO_IN);
    gpio_pull_up(SeatForeWard);

    gpio_init(SeatBackWard);
    gpio_set_dir(SeatBackWard, GPIO_IN);
    gpio_pull_up(SeatBackWard);

    stdio_init_all();

    gpio_init(Forward);
    gpio_set_dir(Forward, GPIO_OUT);
    gpio_init(Backward);
    gpio_set_dir(Backward, GPIO_OUT);

    bool button_pressed_f = false;
    bool button_pressed_b = false;

    char input_buffer[32]; // 입력 버퍼 생성
    int buffer_index = 0;

    while (true) {
        int ch = getchar_timeout_us(0); // 한 문자씩 읽기
        if (ch != PICO_ERROR_TIMEOUT) {
            if (ch == '\n' || ch == '\r') { // 엔터를 눌렀을 때
                input_buffer[buffer_index] = '\0'; // 문자열 종료
                printf("Received via USB: %s\n", input_buffer);
                
                // 명령 처리
                if (strcmp(input_buffer, "a") == 0) {
                    gpio_put(Forward, 1);
                    gpio_put(Backward, 0);
                    printf("seat forward\n");
                } else if (strcmp(input_buffer, "b") == 0) {
                    gpio_put(Backward, 1);
                    gpio_put(Forward, 0);
                    printf("seat backward\n");
                } else if (strcmp(input_buffer, "c") == 0) {
                    gpio_put(Forward, 0);
                    gpio_put(Backward, 0);
                    printf("stop\n");
                }

                buffer_index = 0; // 버퍼 초기화
            } else if (buffer_index < sizeof(input_buffer) - 1) {
                input_buffer[buffer_index++] = ch; // 버퍼에 문자 추가
            }
        }

        button_pressed_f = !gpio_get(SeatForeWard);
        button_pressed_b = !gpio_get(SeatBackWard);

        if(button_pressed_f) {
            gpio_put(Forward, 1);
            printf("seat forward\n");
        } else {
            gpio_put(Forward, 0);
        }

        if(button_pressed_b) {
            gpio_put(Backward, 1);
            printf("seat backward\n");
        } else {
            gpio_put(Backward, 0);
        }
    }
    return 0;
}
