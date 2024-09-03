#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>

int main() {
    stdio_init_all();  // 표준 입출력 초기화

    for(int pincnt = 0; pincnt < 16 ; pincnt++) {
        gpio_init(pincnt);
        gpio_set_dir(pincnt, GPIO_IN);
        gpio_pull_up(pincnt);
    }
    printf("GPIO settings initialized\n");

    while (true) {
        for(int pincnt = 0; pincnt < 16; pincnt++) {
            if (!gpio_get(pincnt)) {
                printf("GPIO %d signal detected\n", pincnt);
            }
        }
        sleep_ms(500);  // CPU 사용률을 줄이기 위해 약간의 지연을 추가
    }

    return 0;
}
