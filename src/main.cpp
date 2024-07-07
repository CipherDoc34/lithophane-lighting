#include <Arduino.h>
#include <ArduinoWebsockets.h>

#ifdef ESP32
    #include <WiFi.h>
    #define BLUE 18
    #define GREEN 17
    #define RED 5
    // #define WHITE_TOP 16
    // #define WHITE_BOTTOM 19
    #define BAUD_RATE 115200

#elif ESP12
    #include <ESP8266WiFi.h>
    #define BLUE_TOP 15
    #define GREEN_TOP 13
    #define RED_TOP 12

    #define BLUE_BOT 0
    #define GREEN_BOT 2
    #define RED_BOT 14
    // #define WHITE_TOP 15
    // #define WHITE_BOTTOM 15
    #define BAUD_RATE 9600
#endif

#include <ESPAsyncWebServer.h>

#include "config.h"
#include "web.h"
#include "addressable_leds.hpp"


#if (defined(ARDUINO) && ARDUINO >= 155) || defined(ESP8266)
    #define YIELD yield();
#else
    #define YIELD
#endif

// the number of the LED pin
const int blue_top = BLUE_TOP;
const int green_top = GREEN_TOP;
const int red_top = RED_TOP;

const int blue_bot = BLUE_BOT;
const int green_bot = GREEN_BOT;
const int red_bot = RED_BOT;
// const int white_top = WHITE_TOP;
// const int white_bot = WHITE_BOTTOM; 

int all_leds[] = {blue_top, green_top, red_top, blue_bot, green_bot, red_bot};

const float blue_max_brightness = 0.7;
const float red_max_brightness = 0.7;
const float green_max_brightness = 1;
const float white_top_max_brightness = 0.5;
const float white_bot_max_brightness = 0.5;

AddressableLEDS * leds;


void handle_message_HTTP(AsyncWebServerRequest * msg);

void handle_change(AsyncWebServerRequest * msg);

void get_current_status(AsyncWebServerRequest * msg);

void change_mode(AsyncWebServerRequest * msg);

// Generic template
template<class T> 
inline Print &operator <<(Print &stream, T arg) 
{ stream.print(arg); return stream; }


int blink;

void change(int r, int g, int b, int h, int s, int v){
    // float saturation;
    // #if RGBLED
    //     saturation = 1;
    // #else
    //     saturation = s/100;
    // #endif
    // #if COMMON_CATHODE
    //     analogWrite(red, round(255 - r * saturation));
    //     analogWrite(green, round(255 - g * saturation));
    //     analogWrite(blue, round(255 - b * saturation));
    //     // analogWrite(white_bot, 255 - v);
    //     // analogWrite(white_top, 255 - v);
    // #else
    //     analogWrite(red_top, round(r * saturation));
    //     analogWrite(green_top, round(g * saturation));
    //     analogWrite(blue_top, round(b * saturation));

    //     analogWrite(red_bot, round(r * saturation));
    //     analogWrite(green_bot, round(g * saturation));
    //     analogWrite(blue_bot, round(b * saturation));
    // #endif

    leds->changeboth(rgbval_t{r, g, b});
}

void turn_off(int led){
    #if COMMON_CATHODE
        digitalWrite(led, HIGH);
    #else
        digitalWrite(led, LOW);
    #endif
    
}

void all_off(int leds[], int size_){
    for (int i = 0; i < size_; i++){
        Serial.print("turning off: ");
        Serial.println(leds[i]);
        turn_off(leds[i]);
        // delay(20);
    }
}

void init(int leds[], int size_){
    for (int i = 0; i < size_; i++)
        pinMode(leds[i], OUTPUT);
}

void setup() {
    leds = new AddressableLEDS(rgbled_t{red_top, green_top, blue_top, 1}, rgbled_t{red_bot, green_bot, blue_bot, 1});

    Serial.begin(BAUD_RATE);


    // blink = 1;
    // // set the LED as an output
    // init(all_leds, sizeof(all_leds)/sizeof(all_leds[0]));

    // all_off(all_leds, sizeof(all_leds)/sizeof(all_leds[0]));

    // Create AP
    // WiFi.softAP(ssid, password);
    // IPAddress IP = WiFi.softAPIP();
    // Serial.print("AP IP address: ");
    // Serial.println(IP);
    Serial << ssid << " " << password;
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    // HTTP handler assignment
    webserver.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html_gz, sizeof(index_html_gz));
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    // webserver.on("/off", HTTP_POST, [](AsyncWebServerRequest * req){
    //     AsyncWebServerResponse *response = req->beginResponse_P(200, "text/html", "SUCCESS");
    //     all_off(all_leds, 5);
    //     req->send(response);
    // });

    webserver.on("/colour", HTTP_GET, handle_message_HTTP);

    webserver.on("/status", HTTP_GET, get_current_status);

    webserver.on("/change_mode", HTTP_GET, change_mode);

    // start server
    webserver.begin();
    server.listen(82);
    Serial.print("Is server live? ");
    Serial.println(server.available());
}

void change_mode(AsyncWebServerRequest * msg){
    if (!(msg->hasParam("mode") && msg->hasParam("led"))){
        msg->send(401, "application/json", "{\"error\" : \"need to have param \'mode\' and \'led\'\"}");
        return;
    }
    leds->change_mode((Modes_t)atoi(msg->getParam("mode")->value().c_str()), atoi(msg->getParam("led")->value().c_str()));  
    msg->send(200);
}

void get_current_status(AsyncWebServerRequest * msg){
    String current_status;
    leds->get_status(current_status);
    Serial << current_status;
    msg->send(200, "application/json", current_status);
}

void handle_change(AsyncWebServerRequest * msg){
    RValue = round(atof(msg->getParam("r")->value().c_str()));
    GValue = round(atof(msg->getParam("g")->value().c_str()));
    BValue = round(atof(msg->getParam("b")->value().c_str()));
    HValue = round(atof(msg->getParam("h")->value().c_str()));
    SValue = round(atof(msg->getParam("s")->value().c_str()));
    VValue = round(atof(msg->getParam("v")->value().c_str()));
    if (msg->hasParam("led")){
        which_led = atoi(msg->getParam("led")->value().c_str());
        // 1 = top
        // 0 = bottom
        // 2 = both
        if (which_led == 2)
            leds->changeboth(rgbval_t{RValue, GValue, BValue});
        else
            leds->changesingle(rgbval_t{RValue, GValue, BValue}, which_led);
    } else
        leds->changeboth(rgbval_t{RValue, GValue, BValue});

    msg->send(200);
}

void handle_message(WebsocketsMessage msg) {
    // msg should be formatted R, G, B, H, S, V
    commaIndex1 = msg.data().indexOf(',');
    RValue = msg.data().substring(0, commaIndex1).toInt();

    commaIndex2 = msg.data().indexOf(',', commaIndex1 + 1);
    GValue = msg.data().substring(commaIndex1 + 1, commaIndex2).toInt();

    commaIndex1 = msg.data().indexOf(',', commaIndex2 + 1);
    BValue = msg.data().substring(commaIndex2 + 1, commaIndex1).toInt();

    commaIndex2 = msg.data().indexOf(',', commaIndex1+1);
    HValue = msg.data().substring(commaIndex1 + 1, commaIndex2).toInt();

    commaIndex1 = msg.data().indexOf(',', commaIndex2 + 1);
    SValue = msg.data().substring(commaIndex2 + 1, commaIndex1).toInt();

    VValue = msg.data().substring(commaIndex1 + 1).toInt();

    Serial << "R: " << RValue << " G: " << GValue << " B: " << BValue << " H: " << HValue << " S: " << SValue << " V: " << VValue << "\n";

    change(RValue, GValue, BValue, HValue, SValue, VValue);

}

void handle_message_HTTP(AsyncWebServerRequest * msg) {
    // int args = msg->args();
    // for(int i=0;i<args;i++){
    //     Serial.printf("ARG[%s]: %s\n", msg->argName(i).c_str(), msg->arg(i).c_str());
    // }

    RValue = round(atof(msg->getParam("r")->value().c_str()));
    GValue = round(atof(msg->getParam("g")->value().c_str()));
    BValue = round(atof(msg->getParam("b")->value().c_str()));
    HValue = round(atof(msg->getParam("h")->value().c_str()));
    SValue = round(atof(msg->getParam("s")->value().c_str()));
    VValue = round(atof(msg->getParam("v")->value().c_str()));

    change(RValue, GValue, BValue, HValue, SValue, VValue);
    
    msg->send(200);
}


void loop(){
    leds->run();
    // auto client = server.accept();
    // client.onMessage(handle_message);
    // while (client.available()) {
    //     client.poll();
    // }
}