#include <Arduino.h>

#define MAX_ANALOG 255

class RgbLed{
    private:
        const int redpin, bluepin, greenpin;
        int redval, greenval, blueval; // these will always correspond with standard R G B values
        bool state;
        const int type_led; // 1 = common cathode, 0 = common_anode
        float brightness;
    public:
        RgbLed(int red, int green, int blue, int type_led = 1) : redpin(red), greenpin(green), bluepin(blue), type_led(type_led), brightness(1){
            redval = 0; greenval = 0; blueval = 0;
            init();
        };

        void setRGB(int red, int green, int blue);

        void init();
        
        void setred(int val);
        void setgreen(int val);
        void setblue(int val);

        int getred();
        int getgreen();
        int getblue();

        void writeRGB();

        void turnoff();

        bool getState();

        void switchState();

        void setBrightness(float brightness);

        float getBrightness();
};
