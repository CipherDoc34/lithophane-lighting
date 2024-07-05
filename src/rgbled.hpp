#include <Arduino.h>

class RgbLed{
    private:
        const int redpin, bluepin, greenpin;
        int redval, greenval, blueval;
        bool state;
    public:
        RgbLed(int red, int green, int blue) : redpin(red), greenpin(green), bluepin(blue), state(false) {
            redval = 0; greenval = 0; blueval = 0;
        };

        void setRGB(int red, int green, int blue);

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
};
