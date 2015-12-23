#include<stdint.h>
#include<stdio.h>
#include<math.h>

#include<chrono>
#include<thread>

#include "Adafruit_PWM_Servo_Driver.h"

int main(int argc, char **argv) {
    if(argc == 2 && argv[1][0] == 'r') { //Emergency servo "reset" action, to get the servos all into a roughly neutral position without IK
        PWM sc=PWM();
        sc.setPrescale(99); //4uS per tick, about 61Hz refresh
        sc.setAllPWM(0,(125+625)/2); //roughly center all the channels
        std::this_thread::sleep_for(std::chrono::seconds(2));
        sc.setAllPWM(0,4096); //Set all PWM channels to off (bit 12 of the LEDn_OFF register)
        return 0;
    }
    PWM servo_control=PWM();
    //servo_control.setPWMFreq(60);
    servo_control.setPrescale(99); //4uS per tick, about 61Hz
    servo_control.setAllPWM(0,4096); //Set all PWM channels to off (bit 12 of the LEDn_OFF register)
    while(1==1) {
        for(int i=125;i<625;++i) {
            for(int s=0;s<3;++s) {
                servo_control.setPWM(s,0,i);
            }
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        for(int i=625;i>=125;--i) {
            for(int s=0;s<3;++s) {
                servo_control.setPWM(s,0,i);
            }
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}
