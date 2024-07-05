#include "rgbled.hpp"
#include <cmath>

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

class AddressableLEDS{
    private:
        // const RGBpin top, bot;

        RgbLed top;
        RgbLed bottom;

        Modes_t mode;

        float brightness;

        unsigned long start;

        float breath_step;
        int breath_interval;

        float cur_step;
        int cycle_interval;
    
    public:
        AddressableLEDS(rgbled_t topled, rgbled_t botled):
            top(topled.red, topled.green, topled.blue, topled.type), bottom{botled.red, botled.green, botled.blue, botled.type},
            breath_interval(100000), breath_step(0.1f), cur_step(0), cycle_interval(1000)
        {};

        void init(); 

        void changeboth(rgbval_t value);

        // which:
        //      1 = top
        //      0 = bot
        void changesingle(rgbval_t value, int which);

        void setboth();

        // which:
        //      1 = top
        //      0 = bot
        void setsingle(int which);

        
        void change_mode(Modes_t mode);

        void run();

        void breath();

        rgbval_t hue_to_rgb(float h, float s, float v);
};
