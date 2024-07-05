#include "rgbled.hpp"

void RgbLed::setRGB(int red, int green, int blue){
    this->redval = red;
    this->blueval = blue;
    this->greenval = green;
}

void RgbLed::setred(int val){
    this->redval = val;
}

void RgbLed::setgreen(int val){
    this->greenval = val;
}

void RgbLed::setblue(int val){
    this->blueval = val;
}

int RgbLed::getred(){
    return this->redval;
}

int RgbLed::getgreen(){
    return this->greenval;
}

int RgbLed::getblue(){
    return this->blueval;
}

void RgbLed::writeRGB(){
    if (this->type_led){
        analogWrite(this->redpin, round((float)this->redval * brightness));
        analogWrite(this->greenpin, round((float)this->greenval * brightness));
        analogWrite(this->bluepin, round((float)this->blueval * brightness));
    } else {
        analogWrite(this->redpin, MAX_ANALOG - round((float)this->redval * brightness));
        analogWrite(this->greenpin, MAX_ANALOG - round((float)this->greenval * brightness));
        analogWrite(this->bluepin, MAX_ANALOG - round((float)this->blueval * brightness));
    }
    
    this->state = true;
}

void RgbLed::turnoff(){
    if (this->type_led) {
        analogWrite(this->redpin, 0);
        analogWrite(this->greenpin, 0);
        analogWrite(this->bluepin, 0);
    } else {
        analogWrite(this->redpin, MAX_ANALOG);
        analogWrite(this->greenpin, MAX_ANALOG);
        analogWrite(this->bluepin, MAX_ANALOG);
    }
    this->state = false;
}

bool RgbLed::getState(){
    return this->state;
}

void RgbLed::switchState(){
    if (this->getState())
        this->turnoff();
    else
        this->writeRGB();
}

void RgbLed::init(){
    
    if (this->type_led){
        digitalWrite(this->bluepin, LOW);
        digitalWrite(this->redpin, LOW);
        digitalWrite(this->greenpin, LOW);
    } else {
        digitalWrite(this->bluepin, HIGH);
        digitalWrite(this->redpin, HIGH);
        digitalWrite(this->greenpin, HIGH);
    }
    
    this->writeRGB();
    this->turnoff();
}

void RgbLed::setBrightness(float brightness){
    this->brightness = brightness;
}

float RgbLed::getBrightness(){
    return this->brightness;
}

