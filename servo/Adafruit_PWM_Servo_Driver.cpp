#include<stdint.h>
#include<stdio.h>
#include<math.h>

#include<chrono>
#include<thread>

#ifdef armv6l
#include<wiringPi.h>
#include<wiringPiI2C.h>
#else
#include "dummyWiringPi.h"
#endif
#include "Adafruit_PWM_Servo_Driver.h"

PWM::PWM(uint8_t address/*=0x40*/, bool debug/*=false*/) {
    pwm_fd = wiringPiI2CSetup(address);
    this->debug = debug;
    this->address = address;
    if(debug) printf("Resetting PCA9685 MODE1 (without SLEEP) and MODE2\n");
    setAllPWM(0, 0);
    wiringPiI2CWriteReg8(pwm_fd, __MODE2, __OUTDRV);
    wiringPiI2CWriteReg8(pwm_fd, __MODE1, __ALLCALL);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    uint32_t mode1 = wiringPiI2CReadReg8(pwm_fd, __MODE1);
    mode1 = mode1 & ~__SLEEP;
    wiringPiI2CWriteReg8(pwm_fd, __MODE1, mode1);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

void PWM::setPWMFreq(uint32_t freq) {
    double prescaleval = 25000000.0; //25MHz
    prescaleval /= 4096.0;           //12 bits
    prescaleval /= double(freq);
    prescaleval -= 1.0;
    if(debug) {
        printf("Setting PWM frequency to %d Hz\n", freq);
        printf("Estimated pre-scale: %d\n", int(prescaleval));
    }
    prescaleval = floor(prescaleval + 0.5);
    if(debug) printf("Final pre-scale: %d\n", int(prescaleval));

    uint32_t old_mode = wiringPiI2CReadReg8(pwm_fd, __MODE1);
    uint32_t new_mode = ((old_mode & 0x7F) | __SLEEP);
    wiringPiI2CWriteReg8(pwm_fd, __MODE1, new_mode);
    wiringPiI2CWriteReg8(pwm_fd, __PRESCALE, int(floor(prescaleval)));
    wiringPiI2CWriteReg8(pwm_fd, __MODE1, old_mode);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    wiringPiI2CWriteReg8(pwm_fd, __MODE1, old_mode | 0x80);
}

void PWM::setPrescale(uint32_t scale) {
    uint32_t old_mode = wiringPiI2CReadReg8(pwm_fd, __MODE1);
    uint32_t new_mode = ((old_mode & 0x7F) | __SLEEP);
    wiringPiI2CWriteReg8(pwm_fd, __MODE1, new_mode);
    wiringPiI2CWriteReg8(pwm_fd, __PRESCALE, scale);
    wiringPiI2CWriteReg8(pwm_fd, __MODE1, old_mode);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    wiringPiI2CWriteReg8(pwm_fd, __MODE1, old_mode | 0x80);
}

void PWM::setPWM(uint8_t channel, uint32_t on, uint32_t off) {
    wiringPiI2CWriteReg8(pwm_fd, __LED0_ON_L+4*channel, on & 0xFF);
    wiringPiI2CWriteReg8(pwm_fd, __LED0_ON_H+4*channel, on>>(8));
    wiringPiI2CWriteReg8(pwm_fd, __LED0_OFF_L+4*channel, off & 0xFF);
    wiringPiI2CWriteReg8(pwm_fd, __LED0_OFF_H+4*channel, off>>(8));
}

void PWM::setAllPWM(uint32_t on, uint32_t off) {
    wiringPiI2CWriteReg8(pwm_fd, __ALL_LED_ON_L, on & 0xFF);
    wiringPiI2CWriteReg8(pwm_fd, __ALL_LED_ON_H, on>>(8));
    wiringPiI2CWriteReg8(pwm_fd, __ALL_LED_OFF_L, off & 0xFF);
    wiringPiI2CWriteReg8(pwm_fd, __ALL_LED_OFF_H, off>>(8));
}
