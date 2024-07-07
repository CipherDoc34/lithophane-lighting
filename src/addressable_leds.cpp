#include "addressable_leds.hpp"

void AddressableLEDS::init(){
    this->start_breath = micros();
    this->start_cycle = micros();
}

void AddressableLEDS::change_mode(Modes_t mode, int which){
    switch(which){
        case 0:
            this->mode_bot = mode;
            break;
        case 1:
            this->mode_top = mode;
            break;
        case 2:
        default:
            this->mode_bot = mode;
            this->mode_top = mode;
    }
    Serial.print(this->mode_bot);
    Serial.print(" ");
    Serial.print(this->mode_top);
    Serial.print(" ");
    Serial.print(mode);
    Serial.print(" ");
    Serial.println(which);
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

    // if (micros() - this->start >= this->cycle_interval){
    //     cur_step = fmod((cur_step + 0.1f), 361);
    //     this->changesingle(this->hue_to_rgb(cur_step, 1, 1), 1);
    //     // Serial.print(this->top.getred());
    //     // Serial.print("  ");
    //     // Serial.print(this->top.getgreen());
    //     // Serial.print("  ");
    //     // Serial.println(this->top.getred());
    // }
    if (this->mode_bot == this->mode_top)
        this->run_mode(which_led::BOTH);
    else {
        this->run_mode(which_led::TOP);
        this->run_mode(which_led::BOTTOM);
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

size_t AddressableLEDS::get_status(String& serialized){
    StaticJsonDocument<100> data;
    data["top_red"] = this->top.getred();
    data["top_green"] = this->top.getgreen();
    data["top_blue"] = this->top.getblue();
    data["top_mode"] = this->mode_top;

    data["bot_red"] = this->bottom.getred();
    data["bot_green"] = this->bottom.getgreen();
    data["bot_blue"] = this->bottom.getblue();
    data["bot_mode"] = this->mode_bot;

    return serializeJson(data, serialized);
}

void AddressableLEDS::update_breath(float step, int interval){
    // this->breath_step = step;
    this->breath_interval = interval;
}
void AddressableLEDS::update_cycle(float step, int interval){
    this->cycle_interval = interval;
    // this->cur_step = step;
}


void AddressableLEDS::breath(int which){
    if (micros() - this->start_breath >= this->breath_interval){
        RgbLed * led;
        switch(which){
            case TOP:
                led = &this->top;
                break;
            case BOTTOM:
                led = &this->bottom;
                break;
        }
        if ((led->getBrightness() + this->breath_step) >= 1.0f)
            this->breath_step = -0.01f;
        else if ((led->getBrightness() + this->breath_step) <= 0.0f)
            this->breath_step = 0.01f;
        
        // float brightness = fmod(led->getBrightness() + this->breath_step, 1);
        // led->setBrightness(brightness);

        led->setBrightness(fmod(led->getBrightness() + this->breath_step, 1));
        start_breath = micros();
    }
}

void AddressableLEDS::cycle(int which){
    if (micros() - this->start_cycle >= this->breath_interval){
        cur_step = fmod((cur_step + 0.1f), 361);
        this->change(this->hue_to_rgb(cur_step, 1, 1), which);
        this->start_cycle = micros();
    }
}


void AddressableLEDS::run_mode(int led){
    if (led == which_led::BOTH){
        switch(this->mode_bot){
            case Modes_t::STATIC:
                this->top.setBrightness(1);
                this->bottom.setBrightness(1);
                break;
            case Modes_t::CYCLECOLOUR:
                this->cycle(led);
                break;
            case Modes_t::BREATH:
                if (micros() - this->start_breath >= this->breath_interval){
                    if ((this->top.getBrightness() + this->breath_step) >= 1.0f)
                        this->breath_step = -0.01f;
                    else if ((this->top.getBrightness() + this->breath_step) <= 0.0f)
                        this->breath_step = 0.01f;
                    float brightness = fmod(this->top.getBrightness() + this->breath_step, 1);
                    this->top.setBrightness(brightness);
                    this->bottom.setBrightness(brightness);
                    start_breath = micros();
                }
        }
    } else {
        Modes_t mode;
        if (led == which_led::TOP)
            mode = this->mode_top;
        else
            mode = this->mode_bot;
        switch(mode){
            case Modes_t::STATIC:
                RgbLed * this_led;
                if (led == TOP)
                    this_led = &this->top;
                else
                    this_led = &this->bottom;
                this_led->setBrightness(1);
                break;
            case Modes_t::CYCLECOLOUR:
                this->cycle(led);
                break;
            case Modes_t::BREATH:
                this->breath(led);
        }
    }
}

void AddressableLEDS::change(rgbval_t value, int which){
    switch(which){
        case which_led::TOP:
            this->changesingle(value, which_led::TOP);
            break;
        case which_led::BOTTOM:
            this->changesingle(value, which_led::BOTTOM);
            break;
        case which_led::BOTH:
        default:
            this->changeboth(value);
    }
}
