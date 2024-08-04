#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>

#define I2C_PORT i2c0
#define SDA_PIN 16
#define SCL_PIN 17
#define MPU6050_ADDR 0x68

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

void calculate_raw_acc(int16_t* accel,float* analog_accel)
{
    
    for (int i = 0 ; i < 3 ;i++)
    {
        analog_accel[i] = (float)accel[i] * 2.f * 9.80665f / 32767.f;
    }
}


void calculate_raw_gyro(int16_t* gyro,float* analog_gyro)
{
    
    for (int i = 0 ; i < 3 ;i++)
    {
        analog_gyro[i] = (float)gyro[i] * 250.f / 32767.f;
    }
}

void low_pass_filter(float* analog_accel, float* filtered_accel)
{
    for(int i = 0 ; i < 3; i++)
    {
        filtered_accel[i] += 5.f * 0.01f * (analog_accel[i] - filtered_accel[i]);
    }

}

int main() {
    stdio_init_all();

    // I2C 초기화
    i2c_init(I2C_PORT, 100 * 1000);  // 100kHz
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // MPU6050 초기화
    mpu6050_init();

    int16_t accel[3], gyro[3];
    float analog_accel[3], analog_gyro[3];
    float filter_accel[3] ={0,0,0};
    float filter_gyro[3] ={0,0,0};

    while (1) {
        mpu6050_read_raw(accel, gyro);
        calculate_raw_acc(accel, analog_accel);
        calculate_raw_gyro(gyro, analog_gyro);
        low_pass_filter(analog_accel,filter_accel);
        printf("Accel: X=%f, Y=%f, Z=%f\n", filter_accel[0], filter_accel[1], filter_accel[2]);
        printf("Gyro:  X=%f, Y=%f, Z=%f\n", analog_gyro[0], analog_gyro[1], analog_gyro[2]);

        sleep_ms(100);
    }

    return 0;
}
