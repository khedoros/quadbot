#include<stdint.h>
int wiringPiI2CSetup(uint8_t address);
void wiringPiI2CWriteReg8(int pwm_fd, uint8_t reg, uint8_t val);
uint8_t wiringPiI2CReadReg8(int pwm_fd, uint8_t reg);
