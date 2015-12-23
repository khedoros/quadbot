#ifndef JOYSTICKINPUT_H
#define JOYSTICKINPUT_H

//#include<stdint.h>
//#include<stdio.h>
//#include<math.h>
//#include<stdlib.h>
#include<linux/joystick.h>
//#include<fcntl.h>
//#include<errno.h>
//#include<unistd.h>
//#include<string.h>

//#include<iostream>
//#include<chrono>
//#include<thread>
#include<vector>
//#include<tuple>
#include<string>
#include<list>

class JoystickInput {

private:
    const char *ctypes[];
    const char * type_str(unsigned char ctype);

    enum stat {
        unopened,
        valid,
        processing,
        shutdown,
        done,
        closed
    };
    stat status; //0: never opened, 1: opened and valid, 2: opened and processing, 3: 2, but shutdown initiated 4: opened but not processing, 5: closed

    int js_fd;

    std::vector<int16_t> axes;
    std::vector<int8_t> buttons;
    std::list<js_event> events;
    std::string jsname;

    std::thread monitor;

    static void process_events(JoystickInput* o);

public:
    JoystickInput();
    bool init(std::string filename /*usually /dev/input/js0*/, bool blocking = false);
    bool uninit();
    ~JoystickInput();
    bool get_event(js_event &e);
};

#endif
