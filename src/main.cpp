// include all needed libraries
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <secrets.h>
#include <ArduinoOTA.h>

// WebSerial libraries
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

// variable declaring ip address
IPAddress localIP(10, 0, 8, 71);
IPAddress gateway(10, 0, 8, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(10, 0, 8, 10);
IPAddress secondaryDNS(10, 0, 8, 1);

const char* hostname = "flow-pump-controller";

// variables declaring NTP server
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600);

// variable declaring WebSerial server
AsyncWebServer server(80);

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


bool pumpOn = false;
// function used to turn pump on when button is pressed
void turnOn()
{
    if(pumpOn == false)
    {
        digitalWrite(relay, LOW);
        WebSerial.print("Pump turned on at: ");
        WebSerial.print(currentHour);
        WebSerial.print(":");
        if(currentMinute < 10)
        {
            WebSerial.print("0");
        }
        WebSerial.println(currentMinute);
        savedTime = actualTime;
        pumpOn = true;
    }
} 

void turnOff()
{
    digitalWrite(relay, HIGH);
    WebSerial.print("Pump turned off at: ");
    WebSerial.print(currentHour);
    WebSerial.print(":");
    if(currentMinute < 10)
    {
        WebSerial.print("0");
    }
    WebSerial.println(currentMinute);
    pumpOn = false;
}

void setup()
{   
    Serial.begin(115200);
    
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

    // initialize WebSerial server
    WebSerial.begin(&server);
    server.begin();
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

    // if manually turned on count to turn off
    if(actualTime - savedTime > 180000 && pumpOn == true)
    {
        turnOff();
    }
    
    //logic for time checking and automatic control of pump
    if(currentDay >= 1 && currentDay <= 5)
    {  
        if(currentHour == 5 && currentMinute == 45)
        {
            turnOn();
        } else if (currentHour == 7 && currentMinute == 15)
        {
            turnOn();
        } else if (currentHour == 15 && currentMinute == 0)
        {
            turnOn();
        } else if (currentHour == 18 && currentMinute == 0)
        {
            turnOn();
        } else if (currentHour == 18 && currentMinute == 30)
        {
            turnOn();
        } else if (currentHour == 18 && currentMinute == 45)
        {
            turnOn();
        } else if (currentHour == 20 && currentMinute == 30)
        {
            turnOn();
        } else if (currentHour == 22 && currentMinute == 45)
        {
            turnOn();
        } 
    } else if(currentDay == 0 || currentDay == 6)
    {
        if(currentHour == 6 && currentMinute == 45)
        {
            turnOn();
        } else if (currentHour == 10 && currentMinute == 0)
        {
            turnOn();
        } else if (currentHour == 12 && currentMinute == 0)
        {
            turnOn();
        } else if (currentHour == 15 && currentMinute == 0)
        {
            turnOn();
        } else if (currentHour == 18 && currentMinute == 0)
        {
            turnOn();
        } else if (currentHour == 18 && currentMinute == 30)
        {
            turnOn();
        } else if (currentHour == 18 && currentMinute == 45)
        {
            turnOn();
        } else if (currentHour == 20 && currentMinute == 0)
        {
            turnOn();
        } else if (currentHour == 22 && currentMinute == 0)
        {
            turnOn();
        }
    } 
}
