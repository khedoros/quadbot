#include<stdint.h>

class servo {
private:
    uint16_t min_tick;
    uint16_t max_tick;
    float arc_range;

    float angle;
    uint16_t pulseticks;

public:
    uint16_t get_tick();
    float get_angle();
    bool set_angle(float angle);
    servo(float min, float max, float range);
    ~servo();
};
