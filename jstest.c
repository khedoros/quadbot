#include<stdio.h>
#include<stdlib.h>
#include<linux/joystick.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>

static const char *ctypes[] = {"button", "axis", "init_button", "init_axis"};


const char * type_str(unsigned char ctype) {
    if(0x80 & ctype) ctype -= 0x7e;
    return ctypes[ctype - 1];
}

double valp(short value) {
    return (double)value / 32767.0;
}

int main() {
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
    while(1) {
        while(read(fd, &e, sizeof(e)) > 0) {
            if((e.type == 1 || e.type == 0x81) && !e.value) {
                printf("Time: %d, status: released, type: %s, number: %d\n", e.time, type_str(e.type), e.number);
            }
            else if((e.type == 1 || e.type == 0x81) && e.value) {
                printf("Time: %d, status:  pressed, type: %s, number: %d\n", e.time, type_str(e.type), e.number);
            }
            else
                printf("Time: %d, value: %0.2f, type: %s, number: %d\n", e.time, valp(e.value), type_str(e.type), e.number);
        }
        if(errno != EAGAIN) { printf("Some real error! (%d)\n", errno);}
        usleep(1000);
    }
    close(fd);
    return 0;
}
