#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

const uint FORWARD_BUTTON = 14;

const uint FORWARD_PWM = 2;
const uint BACKWARD_PWM = 3;

int main() {
    stdio_init_all();
    adc_init();

    gpio_init(FORWARD_BUTTON);
    gpio_set_dir(FORWARD_BUTTON, GPIO_OUT);
    gpio_pull_up(FORWARD_BUTTON);


    gpio_init(FORWARD_PWM);
    gpio_set_dir(FORWARD_PWM, GPIO_OUT);


    gpio_init(BACKWARD_PWM);
    gpio_set_dir(BACKWARD_PWM, GPIO_OUT);


    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    // GPIO 26을 ADC 입력으로 설정
    adc_gpio_init(26);

    // ADC 입력 채널 선택 (GPIO 26은 ADC 채널 0에 해당)
    adc_select_input(0);
    float desired_length = (float)adc_read()*600.f/4095.f;
    float real_length = (float)adc_read()*600.f/4095.f;  
    char input_buffer[32]; // 입력 버퍼 생성
    int buffer_index = 0;
    uint16_t result;
    float err;
    printf("setting is finish\n");
    while (true) {
        int ch = getchar_timeout_us(0); // 한 문자씩 읽기
        if (ch != PICO_ERROR_TIMEOUT) {
            if (ch == '\n' || ch == '\r') { // 엔터를 눌렀을 때
                input_buffer[buffer_index] = '\0'; // 문자열 종료
                desired_length = atof(input_buffer); // 문자열을 실수로 변환
                printf("Received via USB: %f\n", desired_length);
                buffer_index = 0; // 버퍼 초기화
            } else if (buffer_index < sizeof(input_buffer) - 1) {
                input_buffer[buffer_index++] = ch; // 버퍼에 문자 추가
            }
        }
        //result = adc_read();
        real_length = (float)( adc_read()*600.f/4095.f );
        err =(desired_length-real_length);
        printf("err: %f\n", err);

        if( err < -20 )
        {
            gpio_put(FORWARD_BUTTON, 1);
            gpio_put(FORWARD_PWM, 0);
            gpio_put(BACKWARD_PWM, 1);
            printf("backward\n");
        }
        else if(err > 20)
        {
            
            gpio_put(FORWARD_BUTTON, 1);
            gpio_put(FORWARD_PWM, 1);
            gpio_put(BACKWARD_PWM, 0);
            printf("forward\n");
        }
        else
        {
            gpio_put(FORWARD_BUTTON, 0);
            gpio_put(FORWARD_PWM, 0);
            gpio_put(BACKWARD_PWM, 0);
        }
        sleep_ms(100);
    }

    return 0;
}
