/*
Code developed for Arduino Nano using Atom/PIO
Temperature sensor: DHT11
RTC module: DS3231
LCD 16x2
*/

#include <Arduino.h> //Library created for NANO model
//Libraries
#include <LiquidCrystal.h>
#include "DHT.h"
#include <Wire.h>
#include "RTClib.h"

//Definitions
#define LUMIN 11
LiquidCrystal lcd(5,6,7,8,9,10);
int pgCounter = 0;
unsigned long prMillis = 0;
unsigned long interval = 10000;

#define DHTTYPE DHT11
#define DHTPIN 12
DHT dht(DHTPIN, DHTTYPE);

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void setup(){
    Serial.begin(9600);

    //LCD
    lcd.begin(16,2);
    pinMode(LUMIN, OUTPUT);

    //DHT
    dht.begin();

    //RTC
    delay(3000);
    if(!rtc.begin()){
        Serial.println("RTC lost power. Lets set time");
        rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
    }
    //rtc.adjust(DateTime(2019, 2, 20, 14, 11, 0));
}

void date(){
    DateTime now = rtc.now();
    int year = 0;

    if (now.year()>99) {
      year = now.year() - 2000;
    }

    char dateBuffer[12];
    sprintf(dateBuffer, "%02d/%02d/%02d", now.day(), now.month(), year);
    lcd.setCursor(2,1);
    lcd.print(dateBuffer);
    lcd.print(" ");
    lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);

    char timeBuffer[10];
    sprintf(timeBuffer, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    lcd.setCursor(4,0);
    lcd.print(timeBuffer);
}

void temp(){
    float h = dht.readHumidity();
    float c = dht.readTemperature();

    if(isnan(h) || isnan(c)){
        lcd.print("Error");
        return;
    }

    lcd.setCursor(0,0);
    lcd.print("Temp: ");
    lcd.print(c, 1);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Humid: ");
    lcd.print(h, 1);
    lcd.print("%");
}

void loop(){
    analogWrite(LUMIN, 100);

    switch(pgCounter){

      case 1:{
        date();
      }
      break;

      case 2:{
        temp();
      }
      break;
    }

    unsigned long currentMillis = millis();       //call current millis

    if (currentMillis - prMillis > interval) {    //If interval is reached, scroll page
      prMillis = currentMillis;                   //replace previous millis with current millis as new start point
      lcd.clear();                                //lcd clear if page is changed.
      if (pgCounter <2){                          //Page counter never higher than 3 (total of pages)
        pgCounter = pgCounter +1;                 //Go to next page
      }
      else{
        pgCounter=1;                              //if counter higher than 3 (last page) return to page 1
      }
    }

    //date();
    //temp();
}
