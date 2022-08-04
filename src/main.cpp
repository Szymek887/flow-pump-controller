// include all needed libraries
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUDP.h>
#include <secrets.h>

// variables declaring NTP server
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7200);

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
    digitalWrite(relay, LOW);

    delay(180000);
    digitalWrite(relay, HIGH);
} 

void setup()
{   
    Serial.begin(115200);
    
    // establish Wi-Fi connection
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
}

void loop()
{
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
                digitalWrite(relay, LOW);
            } else if (currentHour == 5 && currentMinute == 48)
            {
                digitalWrite(relay, HIGH);
            } else if (currentHour == 6 && currentMinute == 30)
            {
                digitalWrite(relay, LOW);
            } else if (currentHour == 6 && currentMinute == 33)
            {
                digitalWrite(relay, HIGH);
            } else if (currentHour == 7 && currentMinute == 15)
            {
                digitalWrite(relay, LOW);
            } else if (currentHour == 7 && currentMinute == 18)
            {
                digitalWrite(relay, HIGH);
            } else if (currentHour == 15 && currentMinute == 30)
            {
                digitalWrite(relay, LOW);
            } else if (currentHour == 15 && currentMinute == 35)
            {
                digitalWrite(relay, HIGH);
            } else if (currentHour == 18 && currentMinute == 0)
            {
                digitalWrite(relay, LOW);
            } else if (currentHour == 18 && currentMinute == 3)
            {
                digitalWrite(relay, HIGH);
            } else if (currentHour == 19 && currentMinute == 0)
            {
                digitalWrite(relay, LOW);
            } else if (currentHour == 19 && currentMinute == 3)
            {
                digitalWrite(relay, HIGH);
            } else if (currentHour == 20 && currentMinute == 30)
            {
                digitalWrite(relay, LOW);
            } else if (currentHour == 20 && currentMinute == 33)
            {
                digitalWrite(relay, HIGH);
            } else if (currentHour == 21 && currentMinute == 30)
            {
                digitalWrite(relay, LOW);
            } else if (currentHour == 21 && currentMinute == 33)
            {
                digitalWrite(relay, HIGH);
            } 
        } else if(currentDay == 0 || currentDay == 6)
        {
            if(currentHour == 6 && currentMinute == 45)
            {
                digitalWrite(relay, LOW);
            } else if (currentHour == 6 && currentMinute == 48)
            {
                digitalWrite(relay, HIGH);
            } else if (currentHour == 7 && currentMinute == 30)
            {
                digitalWrite(relay, LOW);
            } else if (currentHour == 7 && currentMinute == 33)
            {
                digitalWrite(relay, HIGH);
            } else if (currentHour == 8 && currentMinute == 30)
            {
                digitalWrite(relay, LOW);
            } else if (currentHour == 8 && currentMinute == 33)
            {
                digitalWrite(relay, HIGH);
            } else if (currentHour == 10 && currentMinute == 0)
            {
                digitalWrite(relay, LOW);
            } else if (currentHour == 10 && currentMinute == 3)
            {
                digitalWrite(relay, HIGH);
            } else if (currentHour == 12 && currentMinute == 0)
            {
                digitalWrite(relay, LOW);
            } else if (currentHour == 12 && currentMinute == 3)
            {
                digitalWrite(relay, HIGH);
            } else if (currentHour == 14 && currentMinute == 0)
            {
                digitalWrite(relay, LOW);
            } else if (currentHour == 14 && currentMinute == 3)
            {
                digitalWrite(relay, HIGH);
            } else if (currentHour == 18 && currentMinute == 0)
            {
                digitalWrite(relay, LOW);
            } else if (currentHour == 18 && currentMinute == 3)
            {
                digitalWrite(relay, HIGH);
            } else if (currentHour == 20 && currentMinute == 0)
            {
                digitalWrite(relay, LOW);
            } else if (currentHour == 20 && currentMinute == 3)
            {
                digitalWrite(relay, HIGH);
            } else if (currentHour == 21 && currentMinute == 30)
            {
                digitalWrite(relay, LOW);
            } else if (currentHour == 21 && currentMinute == 33)
            {
                digitalWrite(relay, HIGH);
            }
        } 
}