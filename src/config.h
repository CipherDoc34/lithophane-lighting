#include "defs.hpp"
// const char* ssid = SSID; //Enter SSID
// const char* password = PASS; //Enter Password

using namespace websockets;
WebsocketsServer server;
AsyncWebServer webserver(80);

int RValue, GValue, BValue, HValue, SValue, VValue, commaIndex1, commaIndex2, which_led;