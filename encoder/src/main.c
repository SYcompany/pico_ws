#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include <stdio.h>

const uint PIN_NUMBER = 3; // 사용할 핀 번호 설정

const uint PWM_PIN = 16;

int main() {
    // GPIO 핀 초기화
    stdio_init_all();
    gpio_init(PIN_NUMBER);
    gpio_set_dir(PIN_NUMBER, GPIO_IN); // 핀을 입력으로 설정
    gpio_pull_up(PIN_NUMBER); // 내부 풀업 저항을 활성화
    
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);
    pwm_set_clkdiv(slice_num, 125.0);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 65535);
    pwm_set_enabled(slice_num, true);


    bool last_state = false; // 이전 상태를 저장할 변수
    bool current_state;
    uint64_t last_time = 0; // 이전 시간을 저장할 변수
    uint64_t current_time;
    uint16_t pwm_duty = 0;


    while (true) {
        current_state = gpio_get(PIN_NUMBER); // 현재 핀 상태 읽기
        current_time = time_us_64();
        // 라이징 엣지 검출
        if (!last_state && current_state) {
            // 라이징 엣지가 발생했음을 감지
            // 여기에 원하는 동작을 추가하세요 (예: LED 켜기, 메시지 출력 등)
            float rps =  0.5f/(0.000001 * (current_time-last_time)); 
            //printf("Speed : %f rps PWM Duty : %d\n" , rps , pwm_duty);
            pwm_duty = 300 * (50 - rps);
            pwm_set_chan_level(slice_num, PWM_CHAN_A, pwm_duty);
            last_time = current_time;
        }
	printf("%d\n",current_state);
        last_state = current_state; // 상태 업데이트
        sleep_ms(10); // 불필요한 리소스 사용을 줄이기 위해 잠시 대기
    }

    return 0;
}
