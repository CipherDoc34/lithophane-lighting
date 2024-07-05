#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include "config.h"
#include "web.h"

/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp32-pwm-arduino-ide/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.  
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

// the number of the LED pin
const int blue = 16;  // 16 corresponds to GPIO 16
const int green = 17;
const int red = 5;
const int white_top = 18;
const int white_bot = 19; 

int all_leds[] = {blue, red, green, white_bot, white_top};

const float blue_max_brightness = 0.7;
const float red_max_brightness = 0.7;
const float green_max_brightness = 1;
const float white_top_max_brightness = 0.5;
const float white_bot_max_brightness = 0.5;

void turn_off(int led){
    digitalWrite(led, HIGH);
}

void all_off(int leds[], int size_){
    for (int i = 0; i < size_; i++){
        // Serial.println(leds[i]);s
        turn_off(leds[i]);
        // delay(20);
    }
}

void init(int leds[], int size_){
    for (int i = 0; i < size_; i++)
        pinMode(leds[i], OUTPUT);
}

void setup() {
    // set the LED as an output
    init(all_leds, sizeof(all_leds)/sizeof(all_leds[0]));

    Serial.begin(115200);
    all_off(all_leds, sizeof(all_leds)/sizeof(all_leds[0]));

    // Create AP
    // WiFi.softAP(ssid, password);
    // IPAddress IP = WiFi.softAPIP();
    // Serial.print("AP IP address: ");
    // Serial.println(IP);

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

    // start server
    webserver.begin();
    server.listen(82);
    Serial.print("Is server live? ");
    Serial.println(server.available());
}

void handle_message(WebsocketsMessage msg) {
    commaIndex = msg.data().indexOf(',');
    LValue = msg.data().substring(0, commaIndex).toInt();
    RValue = msg.data().substring(commaIndex + 1).toInt();
    Serial.print("L: ");
    Serial.print(LValue);
    Serial.print(" R: ");
    Serial.println(RValue);
}

void loop(){
    analogWrite(white_bot, 255);
    analogWrite(white_top, 255);
    // auto client = server.accept();
    // client.onMessage(handle_message);
    // while (client.available()) {
    //     client.poll();
    // }
    // increase the LED brightness
    // for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    //     // changing the LED brightness with PWM

    //     // analogWrite(blue, dutyCycle);
    //     // analogWrite(green, dutyCycle);
    //     // analogWrite(red, dutyCycle);
    //     analogWrite(white_top, dutyCycle);
    //     analogWrite(white_bot, dutyCycle);
    //     delay(15);
    // }

    // // decrease the LED brightness
    // for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    //     // changing the LED brightness with PWM

    //     // analogWrite(blue, dutyCycle);
    //     // analogWrite(green, dutyCycle);
    //     // analogWrite(red, dutyCycle);
    //     analogWrite(white_top, dutyCycle);
    //     analogWrite(white_bot, dutyCycle);
    //     delay(15);
    // }
}