#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>

// I2C 포트 및 핀 설정
#define I2C_PORT i2c1
#define SDA_PIN 6
#define SCL_PIN 7

// VL53L0X 주소
#define VL53L0X_ADDR 0x29

// VL53L0X 레지스터 및 명령
#define VL53L0X_REG_IDENTIFICATION_MODEL_ID 0xC0
#define VL53L0X_REG_SYSRANGE_START 0x00
#define VL53L0X_REG_RESULT_RANGE_STATUS 0x1E

void vl53l0x_write_byte(uint8_t reg, uint8_t value) {
    uint8_t buf[2] = {reg, value};
    i2c_write_blocking(I2C_PORT, VL53L0X_ADDR, buf, 2, false);
}

uint8_t vl53l0x_read_byte(uint8_t reg) {
    uint8_t value;
    i2c_write_blocking(I2C_PORT, VL53L0X_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, VL53L0X_ADDR, &value, 1, false);
    return value;
}

uint16_t vl53l0x_read_word(uint8_t reg) {
    uint8_t buf[2];
    i2c_write_blocking(I2C_PORT, VL53L0X_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, VL53L0X_ADDR, buf, 2, false);
    return (buf[0] << 8) | buf[1];
}

void vl53l0x_init() {
    // 초기화 절차는 VL53L0X 데이터 시트를 참조하여 수행해야 합니다.
    // 여기서는 간단한 예제로 초기화를 수행합니다.
    vl53l0x_write_byte(0x88, 0x00);
    sleep_ms(100);
}

uint16_t vl53l0x_read_range() {
    // 측정 시작
    vl53l0x_write_byte(VL53L0X_REG_SYSRANGE_START, 0x01);
    sleep_ms(10);  // 측정 대기 시간

    // 결과 읽기
    return vl53l0x_read_word(VL53L0X_REG_RESULT_RANGE_STATUS);
}

int main() {
    stdio_init_all();

    // I2C 초기화
    i2c_init(I2C_PORT, 400 * 1000);  // 400kHz로 변경
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // VL53L0X 초기화
    vl53l0x_init();

    while (1) {
        uint16_t distance = vl53l0x_read_range();
        if(distance > 20 && distance < 8190)
        {printf( "%f", (float)distance);}
        else{}
        sleep_ms(50);
    }

    return 0;
}
