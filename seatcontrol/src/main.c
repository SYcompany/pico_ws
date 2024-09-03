#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"
//#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>



#define I2C_PORT i2c1
#define SDA_PIN 26
#define SCL_PIN 27
#define MPU6050_ADDR 0x68
#define R2D 180/3.141592

const uint HALL1_NUMBER = 3; // 사용할 핀 번호 설정
const uint HALL2_NUMBER = 4;

const uint FORWARD_BUTTON = 6;

const uint FORWARD_PWM = 7;
const uint BACKWARD_PWM = 8;




void mpu6050_init() {
    uint8_t buf[2];

    // Wake up the MPU6050 as it starts in sleep mode
    buf[0] = 0x6B; // PWR_MGMT_1 register
    buf[1] = 0x00; // Set to 0 (wakes up the MPU6050)
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);

    buf[0] = 0x1B;
    buf[1] = 0x00;
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);

    buf[0] = 0x1C;
    buf[1] = 0x00;
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, buf, 2, false);
}

void mpu6050_read_raw(int16_t* accel, int16_t* gyro) {
    uint8_t buffer[14];
    uint8_t reg = 0x3B;

    // Request to read from register 0x3B
    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, MPU6050_ADDR, buffer, 14, false);

    // Accelerometer values
    accel[0] = (buffer[0] << 8) | buffer[1];  // ACCEL_XOUT_H and ACCEL_XOUT_L
    accel[1] = (buffer[2] << 8) | buffer[3];  // ACCEL_YOUT_H and ACCEL_YOUT_L
    accel[2] = (buffer[4] << 8) | buffer[5];  // ACCEL_ZOUT_H and ACCEL_ZOUT_L

    // Gyroscope values
    gyro[0] = (buffer[8] << 8) | buffer[9];   // GYRO_XOUT_H and GYRO_XOUT_L
    gyro[1] = (buffer[10] << 8) | buffer[11]; // GYRO_YOUT_H and GYRO_YOUT_L
    gyro[2] = (buffer[12] << 8) | buffer[13]; // GYRO_ZOUT_H and GYRO_ZOUT_L
}

void calculate_raw_acc(int16_t* accel, float* analog_accel) {
    for (int i = 0; i < 3; i++) {
        analog_accel[i] = (float)accel[i] * 2.f * 9.80665f / 32767.f;
    }
}

void calculate_raw_gyro(int16_t* gyro, float* analog_gyro) {
    for (int i = 0; i < 3; i++) {
        analog_gyro[i] = (float)gyro[i] * 250.f / 32767.f;
    }
}

void low_pass_filter(float* analog_accel, float* filtered_accel) {
    for (int i = 0; i < 3; i++) {
        filtered_accel[i] += 5.f * 0.01f * (analog_accel[i] - filtered_accel[i]);
    }
}

void cal_angle(float* filtered_accel, float* filtered_gyro, float* angle) {
    angle[0] = R2D * atan2(filtered_accel[1], sqrt(pow(filtered_accel[0], 2) + pow(filtered_accel[2], 2))) + filtered_gyro[0] * 0.01;
    angle[1] = R2D * atan2(filtered_accel[0], sqrt(pow(filtered_accel[1], 2) + pow(filtered_accel[2], 2))) + filtered_gyro[1] * 0.01;
}



int main() {
    // GPIO 핀 초기화
    stdio_init_all();
    gpio_init(HALL1_NUMBER);
    gpio_set_dir(HALL1_NUMBER, GPIO_IN); // 핀을 입력으로 설정
    gpio_pull_up(HALL1_NUMBER); // 내부 풀업 저항을 활성화

    gpio_init(HALL2_NUMBER);
    gpio_set_dir(HALL2_NUMBER, GPIO_IN); // 핀을 입력으로 설정
    gpio_pull_up(HALL2_NUMBER); // 내부 풀업 저항을 활성화

    //gpio_init(FORWARD_BUTTON);
    //gpio_set_dir(FORWARD_BUTTON, GPIO_OUT);
    //gpio_pull_up(FORWARD_BUTTON);


    gpio_init(FORWARD_BUTTON);
    gpio_set_dir(FORWARD_BUTTON, GPIO_OUT);


    gpio_init(FORWARD_PWM);
    gpio_set_dir(FORWARD_PWM, GPIO_OUT);


    gpio_init(BACKWARD_PWM);
    gpio_set_dir(BACKWARD_PWM, GPIO_OUT);

    // I2C 초기화
    i2c_init(I2C_PORT, 100 * 1000);  // 100kHz
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // MPU6050 초기화
    mpu6050_init();

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    bool last_hall1_state = false; // 이전 상태를 저장할 변수
    bool current_hall1_state;
    bool last_hall2_state = false; // 이전 상태를 저장할 변수
    bool current_hall2_state;
    bool button_state;

    int16_t accel[3], gyro[3];
    float analog_accel[3], analog_gyro[3];
    float filter_accel[3] = {0, 0, 0};
    float filter_gyro[3] = {0, 0, 0};
    float angles[3] = {0, 0, 0};
    float desired_angle = 0;
    
    char input_buffer[32]; // 입력 버퍼 생성
    int buffer_index = 0;

    printf("setting is finish\n");

    while (true) {

        // 입력 처리
        int ch = getchar_timeout_us(0); // 한 문자씩 읽기
        if (ch != PICO_ERROR_TIMEOUT) {
            if (ch == '\n' || ch == '\r') { // 엔터를 눌렀을 때
                input_buffer[buffer_index] = '\0'; // 문자열 종료
                desired_angle = atof(input_buffer); // 문자열을 실수로 변환
                printf("Received via USB: %f\n", desired_angle);
                buffer_index = 0; // 버퍼 초기화
            } else if (buffer_index < sizeof(input_buffer) - 1) {
                input_buffer[buffer_index++] = ch; // 버퍼에 문자 추가
            }
        }
        mpu6050_read_raw(accel, gyro);
        calculate_raw_acc(accel, analog_accel);
        calculate_raw_gyro(gyro, analog_gyro);
        low_pass_filter(analog_accel, filter_accel);
        cal_angle(filter_accel, filter_gyro, angles);

        printf("Err: %f\n", desired_angle - angles[0]);


        current_hall1_state = !gpio_get(HALL1_NUMBER); // 현재 핀 상태 읽기
        current_hall2_state = !gpio_get(HALL2_NUMBER); // 현재 핀 상태 읽기
        //button_state = !gpio_get(FORWARD_BUTTON); // 버튼 눌림 상태 확인

        if( desired_angle - angles[0] < -10 )
        {
            gpio_put(FORWARD_BUTTON, 1);
            gpio_put(FORWARD_PWM, 1);
            gpio_put(BACKWARD_PWM, 0);
            printf("forward\n");
        }
        else if(desired_angle - angles[0] > 10)
        {
            gpio_put(FORWARD_BUTTON, 1);
            gpio_put(FORWARD_PWM, 0);
            gpio_put(BACKWARD_PWM, 1);
            printf("backward\n");
        }
        else
        {
            gpio_put(FORWARD_PWM, 0);
            gpio_put(BACKWARD_PWM, 0);
        }
        //if(button_state) { // 버튼이 눌렸을 때
        //    gpio_put(FORWARD_PWM, 1); // FORWARD_PWM 핀을 HIGH로 설정
        //    //printf("Button pressed\n");
        //} else {
        //    gpio_put(FORWARD_PWM, 0); // FORWARD_PWM 핀을 LOW로 설정
        //}

        printf(" %d,  %d\n", current_hall1_state, current_hall2_state);

        last_hall1_state = current_hall1_state; // 상태 업데이트
        last_hall2_state = current_hall2_state; // 상태 업데이트

        

        sleep_ms(10); // 불필요한 리소스 사용을 줄이기 위해 잠시 대기
    }

    return 0;
}