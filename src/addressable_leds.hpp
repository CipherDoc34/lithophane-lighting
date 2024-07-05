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

class AddressableLEDS{
    private:
        // const RGBpin top, bot;

        RgbLed top;
        RgbLed bottom;

        Modes_t mode;

        float brightness;
    
    public:
        AddressableLEDS(rgbled_t topled, rgbled_t botled):
            top(topled.red, topled.green, topled.blue, topled.type), bottom{botled.red, botled.green, botled.blue, botled.type} 
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
};
