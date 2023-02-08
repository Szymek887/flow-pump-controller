// include all needed libraries
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <secrets.h>
#include <ArduinoOTA.h>

#include <ESP8266WebServer.h>

// variable declaring ip address
IPAddress localIP(10, 0, 8, 71);
IPAddress gateway(10, 0, 8, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(10, 0, 8, 10);
IPAddress secondaryDNS(10, 0, 8, 1);

const char* hostname = "flow-pump-controller";

// variable declaring time offset
int timeZone = 1;

// variables declaring NTP server
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", timeZone * 3600);

// variable declaring api server
ESP8266WebServer rest_server(8080);

// variables declaring pin numbers
const int relay = 5;
const int buttonPin = 4;

// variables declaring button states
int buttonState = 0;

// variables declaring time
int currentHour;
int currentMinute;
int currentDay;

// variables used instead of delay()
unsigned long actualTime = 0;
unsigned long savedTime = 0;
unsigned long lastTimeTurnedOn = 0;

// check if pump is on and if is ready to turn on (delay between uses)
bool pumpOn = false;
bool readyToTurnOn = true;

// function used to turn pump on when button is pressed
void turnOn()
{
    if(pumpOn == false && readyToTurnOn == true)
    {
        digitalWrite(relay, LOW);
        savedTime = actualTime;
        lastTimeTurnedOn = actualTime;
        pumpOn = true;
        readyToTurnOn = false;
    }
}

void turnOff()
{
    digitalWrite(relay, HIGH);
    pumpOn = false;
}

void setup()
{   
    Serial.begin(9600);
    
    // establish Wi-Fi connection
    WiFi.config(localIP, gateway, subnet, primaryDNS, secondaryDNS);
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.println("Connecting to the Internet...");
    }
    Serial.println("Connected!");
    
    // start NTP service
    timeClient.begin();

    // declare pinmodes
    pinMode(relay, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);

    // turn relay off by default
    digitalWrite(relay, HIGH);

    // initialize remote updates via ArduinoOTA
    ArduinoOTA.setHostname(hostname);
    ArduinoOTA.setPassword(ota_password);

    ArduinoOTA.begin();

    // initialize api server
    rest_server.on("/turnon", HTTP_GET, []() {
        rest_server.send(200, F("text/html"),
            F("Pump turned on!"));
        turnOn();
    });
    rest_server.begin();
}

void loop()
{
    // check for ArduinoOTA updates
    ArduinoOTA.handle();
    
    // sync time for delay()
    actualTime = millis();

    // sync real world time
    timeClient.update();
    currentHour = timeClient.getHours();
    currentMinute = timeClient.getMinutes();
    currentDay = timeClient.getDay();
    //0-niedziela 1-poniedzialek 2-wtorek 3-sroda 4-czwartek 5-piatek 6-sobota
    
    // logic for buttons for manual control
    buttonState = digitalRead(buttonPin);
    
    // check physical button
    if(buttonState == LOW)
    {
        turnOn();
    }

    // check for api requests
    rest_server.handleClient();

    // if manually turned on count to turn off
    if(actualTime - savedTime > 180000 && pumpOn == true)
    {
        turnOff();
    }

    if(actualTime - lastTimeTurnedOn > 1800000 && readyToTurnOn == false) {
        readyToTurnOn = true;
    }
    
    //logic for time checking and automatic control of pump
    if(currentDay >= 1 && currentDay <= 5)
    {  
        if(currentHour == 5 && currentMinute == 45)
        {
            turnOn();
        } else if (currentHour == 13 && currentMinute == 0)
        {
            turnOn();
        } else if (currentHour == 18 && currentMinute == 30)
        {
            turnOn();
        }
    } else if(currentDay == 0 || currentDay == 6)
    {
        if(currentHour == 7 && currentMinute == 0)
        {
            turnOn();
        } else if (currentHour == 12 && currentMinute == 0)
        {
            turnOn();
        } else if (currentHour == 18 && currentMinute == 30)
        {
            turnOn();
        } else if (currentHour == 20 && currentMinute == 0)
        {
            turnOn();
        }
    } 
}
