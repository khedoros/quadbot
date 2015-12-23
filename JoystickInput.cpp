#include<stdint.h>
#include<stdio.h>
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
#include<string>

#include "JoystickInput.h"


const char *ctypes[] = {"button", "axis", "init_button", "init_axis"};
const char * JoystickInput::type_str(unsigned char ctype) {
    if(0x80 & ctype) ctype -= 0x7e;
    return ctypes[ctype - 1];
}

//Separate thread to process events from the Linux joystick API
void JoystickInput::process_events(JoystickInput *o) {
    struct js_event e;
    std::cout<<"Processing events"<<std::endl;
    o->status = processing;
    int count = 0;
    while(o->status == processing) {
        count++;
        if(count % 100 == 0) std::cout<<"Looped "<<count<<" times"<<std::endl;
        int eventcount = 0;
        while(read(o->js_fd, &e, sizeof(e)) > 0) {
            eventcount++;
            e.type &= 0x7f;
            if(e.type == JS_EVENT_BUTTON) {
                o->buttons[e.number] = e.value;
                //printf("Time: %d, status: released, type: %s, number: %d\n", e.time, type_str(e.type), e.number);
            }
            else if(e.type == JS_EVENT_AXIS) {
                o->axes[e.number] = e.value;
                //printf("Time: %d, value: %0.2f, type: %s, number: %d\n", e.time, valp(e.value), type_str(e.type), e.number);
            }
            if(errno != EAGAIN) { printf("Some real error! (%d)\n", errno);}
            o->events.push_back(e);
        }
        if(eventcount > 0) std::cout<<"Saw "<<eventcount<<" events."<<std::endl;
        //Process events at 100Hz
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout<<"Ending event processing."<<std::endl;
}

JoystickInput::JoystickInput() : js_fd(-1), axes(0), buttons(0), jsname("not open"), status(unopened), events(0), monitor() {
    axes.reserve(8);
    buttons.reserve(32);
}

bool JoystickInput::init(std::string filename /*usually /dev/input/js0*/, bool blocking /*= false*/) {
    //Wait for joystick to be available to open
    while(js_fd < 0) {
        js_fd = open(filename.c_str(), O_RDONLY|(blocking ? 0 : O_NONBLOCK));
        if(js_fd < 0) {
            printf("Waiting for joystick at %s\n", filename.c_str());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    status = valid;

    //Get joystick name, in case I want to use it for something
    char cjsname[128] = {0};
    if(ioctl(js_fd, JSIOCGNAME(sizeof(cjsname)), cjsname) < 0) strncpy(cjsname, "Unknown", sizeof(cjsname));
    jsname = std::string(cjsname);

    //Get a count of axes and buttons, and read the initial joystick state
    int a = 0; int b = 0; //axis and button count
    bool axes_seen[8] = {false};
    bool buttons_seen[32] = {false};
    struct js_event e;

    //I assume that buttons and axes come in order; otherwise, this code will be problematic for the initial controller state ;-)
    while(read(js_fd, &e, sizeof(e)) > 0) {
        if(e.type == (JS_EVENT_BUTTON | JS_EVENT_INIT) && !buttons_seen[e.number]) {
            b++;
            buttons.resize(b);
            buttons_seen[e.number] = true;
            buttons[b-1] = e.value;
        }
        else if(e.type == (JS_EVENT_AXIS | JS_EVENT_INIT) && !axes_seen[e.number]) {
            a++;
            axes.resize(a);
            axes_seen[e.number] = true;
            axes[a-1] = e.value;
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if(errno != EAGAIN) { printf("Some real error! (%d)\n", errno);}
    }
        
    printf("Starting the event-processing thread\n");
    //std::thread temp(process_events, std::ref(status));
    monitor = std::thread(process_events, this);
}

bool JoystickInput::uninit() {
    if(status != closed && status != done) status = shutdown;
    monitor.join();

    close(js_fd);
    status = closed;
    jsname=std::string("closed");
    axes.resize(0);
    buttons.resize(0);
}

JoystickInput::~JoystickInput() {
    uninit();
}

bool JoystickInput::get_event(js_event &e) {
    if(events.size() < 1) return false;
    e = events.front();
    events.pop_front();
    return true;
}
