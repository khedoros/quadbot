#include<stdint.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<linux/joystick.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>

#include<iostream>
#include<chrono>
#include<thread>
#include<vector>
#include<tuple>

#include "Adafruit_PWM_Servo_Driver.h"

static const char *ctypes[] = {"button", "axis", "init_button", "init_axis"};

const char * type_str(unsigned char ctype) {
    if(0x80 & ctype) ctype -= 0x7e;
    return ctypes[ctype - 1];
}

double valp(short value) {
    return (double)value / 32767.0;
}

int main(int argc, char **argv) {
    PWM servo_control=PWM();
    servo_control.setPrescale(99); //4uS per tick, about 61Hz
    servo_control.setAllPWM(0,4096); //Set all PWM channels to off (bit 12 of the LEDn_OFF register)

    char jsname[128] = {0};
    
    int fd = -1;
    while(fd < 0) {
        fd = open("/dev/input/js0", O_RDONLY|O_NONBLOCK);
        if(fd < 0) {
            printf("Didn't open\n");
        }
        usleep(100000);
    }

    if(ioctl(fd, JSIOCGNAME(sizeof(jsname)), jsname) < 0) strncpy(jsname, "Unknown", sizeof(jsname));
    printf("Opened joystick at /dev/input/js0, named %s\n", jsname);


    struct js_event e;
    const int JS_TO_TICK = 65536 / 4096;
    const int JS_TO_PWM = 65536 / 500;
    bool wait_for_button = true;  
    int retval = 0;
    for(int i=2;i<16;++i) servo_control.setPWM(i,0,32768/JS_TO_PWM + 125);
    while(wait_for_button) {
        while(read(fd, &e, sizeof(e)) > 0) {
            if((e.type == 1 || e.type == 0x81) && !e.value) {
                //printf("Time: %d, status: released, type: %s, number: %d\n", e.time, type_str(e.type), e.number);
            }
            else if((e.type == 1 || e.type == 0x81) && e.value) {
                //printf("Time: %d, status:  pressed, type: %s, number: %d\n", e.time, type_str(e.type), e.number);
                wait_for_button = false;
            }
            else if(e.number < 2) {
                servo_control.setPWM(e.number,0,(e.value + 32768) / JS_TO_PWM + 125);
            }
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        if(errno != EAGAIN) { printf("Some real error! (%d)\n", errno);}
        usleep(1000);
    }

    return 0;

    /*
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
    */

    close(fd);
    return 0;
}
