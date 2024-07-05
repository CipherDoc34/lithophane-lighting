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
    analogWrite(this->redpin, this->redval);
    analogWrite(this->greenpin, this->greenval);
    analogWrite(this->bluepin, this->blueval);
    this->state = true;
}

void RgbLed::turnoff(){
    analogWrite(this->redpin, 0);
    analogWrite(this->greenpin, 0);
    analogWrite(this->bluepin, 0);
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
