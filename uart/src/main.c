#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

// UART0 핀 설정 (기본 핀 사용)
#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1

int main() {
    // 표준 입출력 초기화
    stdio_init_all();

    // UART 초기화
    uart_init(UART_ID, BAUD_RATE);
    
    // GPIO 핀을 UART 기능으로 설정
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // 데이터 수신을 위한 버퍼
    char buffer[256];
    int idx = 0;

    // 메인 루프
    while (true) {
        // 데이터가 도착했는지 확인
        if (1) {
            //char c = uart_getc(UART_ID); // 한 바이트 읽기
            scanf("%s",&buffer);
            // 버퍼에 저장
            //buffer[idx++] = c;

            // 개행 문자를 받으면 출력하고 버퍼 초기화
            if (1) {
                //buffer[idx] = '\0';  // Null-terminate
                printf("Received: %s\n", buffer);
                //idx = 0;  // 버퍼 인덱스 초기화
            }
        }
    }

    return 0;
}

