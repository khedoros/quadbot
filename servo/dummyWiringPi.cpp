#include "dummyWiringPi.h"
#include<iostream>
int wiringPiI2CSetup(uint8_t address) {
    std::cout<<std::hex<<"Open I2C interface ("<<address<<") to fd 0"<<std::endl;
    return 0;
}

void wiringPiI2CWriteReg8(int pwm_fd, uint8_t reg, uint8_t val) {
    std::cout<<std::hex<<"I2C write: fd "<<pwm_fd<<" reg "<<reg<<" val "<<val<<std::endl;
}

uint8_t wiringPiI2CReadReg8(int pwm_fd, uint8_t reg) {
    std::cout<<std::hex<<"I2C read: fd "<<pwm_fd<<" reg "<<reg<<std::endl;
    return 0;
}
