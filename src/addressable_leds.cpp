#include "addressable_leds.hpp"

void AddressableLEDS::init(){
    this->start = micros();
}

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
    // if (micros() - this->start >= this->breath_interval){
    //     if ((this->top.getBrightness() + this->breath_step) >= 1.0f)
    //         this->breath_step = -0.1f;
    //     else if ((this->top.getBrightness() + this->breath_step) <= 0.0f)
    //         this->breath_step = 0.1f;
    //     float brightness = fmod(this->top.getBrightness() + this->breath_step, 1);
    //     this->top.setBrightness(brightness);
    //     this->bottom.setBrightness(brightness);
    //     start = micros();
    // }

    if (micros() - this->start >= this->cycle_interval){
        cur_step = fmod((cur_step + 0.1f), 361);
        this->changesingle(this->hue_to_rgb(cur_step, 1, 1), 1);
        // Serial.print(this->top.getred());
        // Serial.print("  ");
        // Serial.print(this->top.getgreen());
        // Serial.print("  ");
        // Serial.println(this->top.getred());
    }
    this->setboth();
}


rgbval_t AddressableLEDS::hue_to_rgb(float h, float s, float v){
    float c, x, m;
    hsvval_t prime;
    rgbval_t ret;

    c = v * s;
    x = c * (1- abs((fmod((h/60), 2) - 1)));
    m = v - c;

    if (h >= 0 && h < 60){
        prime = hsvval_t{c, x, 0};
    } else if (h >= 60 && h < 120){
        prime = hsvval_t{x, c, 0};
    } else if (h >= 120 && h < 180){
        prime = hsvval_t{0, c, x};
    } else if (h >= 180 && h < 240){    
        prime = hsvval_t{0, x, c};
    } else if (h >= 240 && h < 300){
        prime = hsvval_t{x, 0, c};
    } else /*(h >= 300 && h < 360)*/ {
        prime = hsvval_t{c, 0, x};
    }
    ret = rgbval_t{(int)round((prime.h + m)*255), (int)round((prime.s + m)*255), (int)round((prime.v + m)*255)};
    // Serial.print("hue ");
    // Serial.print(ret.red);
    // Serial.print("  ");
    // Serial.print(ret.green);
    // Serial.print("  ");
    // Serial.println(ret.blue);
    return ret;

}
