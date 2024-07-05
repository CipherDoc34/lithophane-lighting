#include "addressable_leds.hpp"

void AddressableLEDS::change_mode(Modes_t mode){
    this->mode = mode;
}

void AddressableLEDS::changeboth(rgbval_t value){
    this->top.setRGB(value.red, value.green, value.blue);
    this->bottom.setRGB(value.red, value.green, value.blue);
}

void AddressableLEDS::changesingle(rgbval_t value, int which){
    if (which){
        this->top.setRGB(value.red, value.green, value.blue);
    }
    else{
        this->bottom.setRGB(value.red, value.green, value.blue);
    }
}

void AddressableLEDS::setboth(){
    this->top.writeRGB();
    this->bottom.writeRGB();
}

void AddressableLEDS::setsingle(int which){
    if (which)
        this->top.writeRGB();
    else
        this->bottom.writeRGB();
}


void AddressableLEDS::run(){
    float brightness = fmod(this->top.getBrightness() + 0.1f, 1);
    this->top.setBrightness(brightness);
    this->bottom.setBrightness(brightness);

    this->setboth();
}
