#include "rgbled.hpp"
#include <cmath>

#include "AsyncJson.h"
#include "ArduinoJson.h"

typedef struct rgbval_t{
    int red;
    int green;
    int blue;
};

typedef struct rgbled_t{
    int red;
    int green;
    int blue;
    int type; // 1 = common cathode, 0 = common_anode
};

typedef enum Modes_t {
    STATIC = 0,
    CYCLECOLOUR = 1,
    BREATH = 2,
    
};

typedef struct hsvval_t{
    float h;
    float s;
    float v;
};

enum which_led {
    BOTTOM = 0,
    TOP = 1,
    BOTH = 2,
};

class AddressableLEDS{
    private:
        // const RGBpin top, bot;

        RgbLed top;
        RgbLed bottom;

        Modes_t mode_top;
        Modes_t mode_bot;

        // float brightness;

        unsigned long start_cycle, start_breath;

        float breath_step;
        int breath_interval;

        float cur_step;
        int cycle_interval;
    
    public:
        AddressableLEDS(rgbled_t topled, rgbled_t botled):
            top(topled.red, topled.green, topled.blue, topled.type), bottom{botled.red, botled.green, botled.blue, botled.type},
            breath_interval(100), breath_step(0.01f), cur_step(0), cycle_interval(100), mode_bot(Modes_t::STATIC), mode_top(Modes_t::STATIC)
        {
            this->init();
        };

        void init(); 

        void changeboth(rgbval_t value);

        // which:
        //      1 = top
        //      0 = bot
        void changesingle(rgbval_t value, int which);

        void change(rgbval_t value, int which);

        void setboth();

        // which:
        //      1 = top
        //      0 = bot
        void setsingle(int which);

        // which:
        //      1 = top
        //      0 = bot
        //      2 = both
        void change_mode(Modes_t mode, int which);

        void cycle(int which);

        void run();

        void run_mode(int led);

        void breath(int which);

        rgbval_t hue_to_rgb(float h, float s, float v);

        size_t get_status(String& serialized);

        void update_breath(float step, int interval);
        void update_cycle(float step, int interval);
};
