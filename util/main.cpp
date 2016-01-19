#include<stdint.h>
#include<stdio.h>
#include<math.h>

#include<cstdlib>
#include<iostream>
#include<chrono>
#include<thread>

#include "Adafruit_PWM_Servo_Driver.h"

int main(int argc, char **argv) {
    uint16_t min,max;
    uint16_t safe_min = 150; //600 microseconds, minimum servo extension
    uint16_t safe_max = 600; //2400 microseconds, maximum servo extension
    uint16_t safe_mid = (safe_min + safe_max) / 2;
    uint16_t safe_off = 4096;
    PWM servo_control=PWM();
    servo_control.setPrescale(99); //4uS per tick, about 61Hz
    servo_control.setAllPWM(0,safe_off); //Set all PWM channels to off (bit 12 of the LEDn_OFF register)

    if(argc == 2 && argv[1][0] == 'r') { //Emergency servo "reset" action, to get the servos all into a roughly neutral position without IK
        for(int i=0;i<16;++i) {
            servo_control.setPWM(i,0,safe_mid); //Normal servo settings range from 1000ms to 2000ms, with 1500ms being "neutral"
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
        servo_control.setAllPWM(0,safe_off); //Set all PWM channels to off (bit 12 of the LEDn_OFF register)
        return 0;
    }
    else if(argc==3) {
        min = std::atoi(argv[1]);
        max = std::atoi(argv[2]);
        if(min == max||max<safe_min||min<safe_min||max>safe_max||min>safe_max) {
            printf("min and max were equal or out of the range of %u and %u (%u is middle). Exiting.\n", safe_min, safe_max, safe_mid);
            return 1;
        }
        else if(min > max) std::swap(min,max);
        while(1==1) {
            for(int i=min;i<max;++i) {
                for(int s=0;s<16;++s) {
                    servo_control.setPWM(s,0,i);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
            for(int i=max;i>=min;--i) {
                for(int s=0;s<16;++s) {
                    servo_control.setPWM(s,0,i);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }
    while(1==1) {
        int16_t servo = 0;
        std::cout<<"Which servo (0-15, -1 to exit): ";
        std::cin>>servo;
        if(servo < 0) break;
        std::cout<<"Give position ("<<safe_min<<"-"<<safe_max<<"):";
        std::cin>>max;
        if(servo >= 0 && servo < 16 && max >= safe_min && max <= safe_max) {
            servo_control.setPWM(servo,0,max);
            std::cout<<servo<<"\t"<<max<<std::endl;
        }
        else printf("Input servo: %u Val: %u is invalid.\n",servo,max);
    }
    return 0;
}
