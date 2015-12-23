#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include "JoystickInput.h"
 
// "busy sleep" while suggesting that other threads run 
// for a small amount of time
void little_sleep(std::chrono::microseconds us)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + us;
    do {
        std::this_thread::yield();
    } while (std::chrono::high_resolution_clock::now() < end);
}
 
int main()
{
    //auto start = std::chrono::high_resolution_clock::now();
    //auto elapsed = std::chrono::high_resolution_clock::now() - start;
    //std::cout << "waited for "
    //          << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
    //          << " microseconds\n";
    JoystickInput bob;
    bob.init("/dev/input/js0");
    //bob.init("/dev/input/event0");
    while(1) {
        js_event e;
        bool got = bob.get_event(e);
        if(got) std::cout<<int(e.type)<<"\t"<<int(e.value)<<"\t"<<int(e.number)<<std::endl;
        else usleep(1000);
    }
    bob.uninit();
}
