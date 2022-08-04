// include all needed libraries
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUDP.h>
#include <secrets.h>
#include <ArduinoOTA.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

// variable declaring ip address
IPAddress localIP(192, 168, 1, 201);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192, 168, 1, 9);
IPAddress secondaryDNS(192, 168, 1, 1);

const char* hostname = "flow-pump-controller";

// variables declaring NTP server
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7200);

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

// function used to turn pump on when button is pressed
void manualTurnOn()
{
    WebSerial.print(currentHour);
    WebSerial.print(":");
    WebSerial.print(currentMinute);
    WebSerial.println("  ->  Pump manually turned on!");
    digitalWrite(relay, LOW);

    delay(180000);
    WebSerial.print(currentHour);
    WebSerial.print(":");
    WebSerial.print(currentMinute);
    WebSerial.println("  ->  Pump manually turned off!");
    digitalWrite(relay, HIGH);
} 

// function used to log time of pump being turned on
void logTurningOn()
{
    WebSerial.print(currentHour);
    WebSerial.print(":");
    WebSerial.print(currentMinute);
    WebSerial.println("  ->  Pump is on!");
}

// function used to log time of pump being turned off
void logTurningOff()
{
    WebSerial.print(currentHour);
    WebSerial.print(":");
    WebSerial.print(currentMinute);
    WebSerial.println("  ->  Pump is off!");
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

    // initialize WebSerial
    WebSerial.begin(&server);
    server.begin();
}

void loop()
{
    // check for ArduinoOTA updates
    ArduinoOTA.handle();
    
    // sync time for delay()
    actualTime = millis();
    
    // logic for buttons for manual control
    buttonState = digitalRead(buttonPin);
    
    // check physical button
    if(buttonState == LOW)
    {
        manualTurnOn();
    }
 
    // sync real world time
    timeClient.update();
    currentHour = timeClient.getHours();
    currentMinute = timeClient.getMinutes();
    currentDay = timeClient.getDay();
    //0-niedziela 1-poniedzialek 2-wtorek 3-sroda 4-czwartek 5-piatek 6-sobota
    
    //logic for time checking and automatic control of pump
        if(currentDay >= 1 && currentDay <= 5)
        {  
            if(currentHour == 5 && currentMinute == 45)
            {
                logTurningOn();
                digitalWrite(relay, LOW);
            } else if (currentHour == 5 && currentMinute == 48)
            {
                logTurningOff();
                digitalWrite(relay, HIGH);
            } else if (currentHour == 7 && currentMinute == 15)
            {
                logTurningOn();
                digitalWrite(relay, LOW);
            } else if (currentHour == 7 && currentMinute == 18)
            {
                logTurningOff();
                digitalWrite(relay, HIGH);
            } else if (currentHour == 15 && currentMinute == 0)
            {
                logTurningOn();
                digitalWrite(relay, LOW);
            } else if (currentHour == 15 && currentMinute == 5)
            {
                logTurningOff();
                digitalWrite(relay, HIGH);
            } else if (currentHour == 18 && currentMinute == 0)
            {
                logTurningOn();
                digitalWrite(relay, LOW);
            } else if (currentHour == 18 && currentMinute == 3)
            {
                logTurningOff();
                digitalWrite(relay, HIGH);
            } else if (currentHour == 20 && currentMinute == 30)
            {
                logTurningOn();
                digitalWrite(relay, LOW);
            } else if (currentHour == 20 && currentMinute == 33)
            {
                logTurningOff();
                digitalWrite(relay, HIGH);
            } else if (currentHour == 22 && currentMinute == 0)
            {
                logTurningOn();
                digitalWrite(relay, LOW);
            } else if (currentHour == 22 && currentMinute == 3)
            {
                logTurningOff();
                digitalWrite(relay, HIGH);
            } 
        } else if(currentDay == 0 || currentDay == 6)
        {
            if(currentHour == 6 && currentMinute == 45)
            {
                logTurningOn();
                digitalWrite(relay, LOW);
            } else if (currentHour == 6 && currentMinute == 48)
            {
                logTurningOff();
                digitalWrite(relay, HIGH);
            } else if (currentHour == 10 && currentMinute == 0)
            {
                logTurningOn();
                digitalWrite(relay, LOW);
            } else if (currentHour == 10 && currentMinute == 3)
            {
                logTurningOff();
                digitalWrite(relay, HIGH);
            } else if (currentHour == 12 && currentMinute == 0)
            {
                logTurningOn();
                digitalWrite(relay, LOW);
            } else if (currentHour == 12 && currentMinute == 3)
            {
                logTurningOff();
                digitalWrite(relay, HIGH);
            } else if (currentHour == 15 && currentMinute == 0)
            {
                logTurningOn();
                digitalWrite(relay, LOW);
            } else if (currentHour == 15 && currentMinute == 3)
            {
                logTurningOff();
                digitalWrite(relay, HIGH);
            } else if (currentHour == 18 && currentMinute == 0)
            {
                logTurningOn();
                digitalWrite(relay, LOW);
            } else if (currentHour == 18 && currentMinute == 3)
            {
                logTurningOff();
                digitalWrite(relay, HIGH);
            } else if (currentHour == 20 && currentMinute == 0)
            {
                logTurningOn();
                digitalWrite(relay, LOW);
            } else if (currentHour == 20 && currentMinute == 3)
            {
                logTurningOff();
                digitalWrite(relay, HIGH);
            } else if (currentHour == 22 && currentMinute == 0)
            {
                logTurningOn();
                digitalWrite(relay, LOW);
            } else if (currentHour == 22 && currentMinute == 3)
            {
                logTurningOff();
                digitalWrite(relay, HIGH);
            }
        } 
}