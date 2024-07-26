#include <math.h>
#include <Wire.h>
#include "RichShieldLightSensor.h"
#include "RichShieldTM1637.h"
#include "RichShieldDHT.h"
#include "RichShieldNTC.h"
#include "RichShieldPassiveBuzzer.h"

#define CLK 10 // CLK of the TM1637 IC connect to D10 of OPEN-SMART UNO R3
#define DIO 11 // DIO of the TM1637 IC connect to D11 of OPEN-SMART UNO R3

#define LED_RED 4
#define LED_GREEN 5
#define LED_BLUE 6
#define LED_YELLOW 7
#define BUTTON_K1 8
#define BUTTON_K2 9
#define PassiveBuzzerPin 3
PassiveBuzzer buz(PassiveBuzzerPin);
void playTone(long note, long millisecond);

TM1637 disp(CLK, DIO); // create TM1637 class object disp and initialize its pins for clock and data
DHT dht; // create DHT class object dht with data pin set to pin 12 by default in the DHT class

#define NTC_PIN A1 // SIG pin of NTC module connect to A1 of IO Shield, that is pin A1 of OPEN-SMART UNO R3
NTC temper(NTC_PIN);

#define LIGHTSENSOR_PIN A2 // SIG pin of Rocker Switch module connect to A0 of IO Shield, that is pin A2 of OPEN-SMART UNO R3
LightSensor lightsensor(LIGHTSENSOR_PIN);

void setup() {
  Serial.begin(9600); // Set baud rate of serial port to be 9600bps
  disp.init(); // The initialization of the display
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(BUTTON_K1, INPUT_PULLUP);
  pinMode(BUTTON_K2, INPUT_PULLUP);
  dht.begin(); // Initialize the DHT Sensor
}

void displayTemperature(int8_t temperature) {
  int8_t temp[4];
  if (temperature < 0) {
    temp[0] = INDEX_NEGATIVE_SIGN;
    temperature = abs(temperature);
  } else if (temperature < 100) {
    temp[0] = INDEX_BLANK;
  } else {
    temp[0] = temperature / 100;
  }
  temperature %= 100;
  temp[1] = temperature / 10;
  temp[2] = temperature % 10;
  temp[3] = 12; // index of 'C' for celsius degree symbol.
  disp.display(temp);
}

void loop() {
  if (digitalRead(BUTTON_K1) == LOW) {
    delay(300);
    float Rsensor = lightsensor.getRes(); // Get resistance of the light sensor
    Serial.print("The resistance of the Light sensor is ");
    Serial.print(Rsensor, 1);
    Serial.println(" KOhm");

    float lux = 325 * pow(Rsensor, -1.4); // Calculate illuminance
    disp.display(lux); // Display illuminance

    Serial.print("Illuminance is almost ");
    Serial.print(lux, 1);
    Serial.println(" lux");
    delay(1000);

    if (lux > 40) {
      blink(LED_GREEN, 200, 5);
    } else {
      blink(LED_RED, 200, 5);
    }
  }

  if (digitalRead(BUTTON_K2) == LOW) {
    delay(300);
    float celsius = temper.getTemperature(); // Get temperature
    displayTemperature((int8_t)celsius); // Display temperature
    delay(1000);
    if(celsius>31)
    {
      blink(LED_YELLOW, 200, 5);
    for(int i=0; i<10; i++){
      buz.playTone(392,100);
      delay(100);
      buz.playTone(1392,100);
      delay(50);}
      }
    else{
      blink(LED_BLUE, 200, 5);
    }   
  }
}
void blink(int pinNum, int blinkPeriod, int blinkTimes)
{
  for(int i=0; i<blinkTimes; i++){
    digitalWrite(pinNum, HIGH);
    delay (blinkPeriod/2);
    digitalWrite (pinNum, LOW);
    delay (blinkPeriod/2);
  }
}