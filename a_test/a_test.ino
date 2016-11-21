#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <SPI.h>

#define radius .0635 // 2.5 inches is .0635 meters
#define circumference .0398982267 // 2pir = circumference
#define LEDwidth .005 // 5 mm / 5 mm
#define numWaves 3
#define PIN 9

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN);

float period, accelX, accelY, accelXY;

int a[8][7] = {
{0,0,1,1,1,0,0},
{0,1,0,0,0,1,0},
{0,1,0,0,0,1,0},
{0,1,1,1,1,1,0},
{0,1,0,0,0,1,0},
{0,1,0,0,0,1,0},
{0,1,0,0,0,1,0},
{0,1,0,0,0,1,0}};

int h[8][7] = {
{1,0,0,0,0,0,1},
{1,0,0,0,0,0,1},
{1,0,0,0,0,0,1},
{1,1,1,1,1,1,1},
{1,0,0,0,0,0,1},
{1,0,0,0,0,0,1},
{1,0,0,0,0,0,1},
{1,0,0,0,0,0,1}
};

int i[8][7] = {
{1,1,1,1,1,1,1},
{0,0,0,1,0,0,0},
{0,0,0,1,0,0,0},
{0,0,0,1,0,0,0},
{0,0,0,1,0,0,0},
{0,0,0,1,0,0,0},
{0,0,0,1,0,0,0},
{1,1,1,1,1,1,1}
};

int heart[8][7] = {
{0,1,1,0,1,1,0},
{0,1,1,1,1,1,0},
{0,1,1,1,1,1,0},
{0,0,1,1,1,0,0},
{0,0,1,1,1,0,0},
{0,0,0,1,0,0,0},
{0,0,0,1,0,0,0},
{0,0,0,0,0,0,0}
};
void setup() {
  // put your setup code here, to run once:
  CircuitPlayground.begin();
  strip.begin();
  strip.show();
  pinMode(PIN, OUTPUT);
  Serial.begin(9600);

}


void loop() {
  // put your main code here, to run repeatedly:
  //for(int 
  accelX = CircuitPlayground.motionX();
  accelY = CircuitPlayground.motionY();
  accelXY = sqrt(pow(accelX, 2) + pow(accelY, 2));
  if(accelXY != 0) {
      period = getPeriod(accelXY);
      //drawLetter(h, period);
      //drawLetter(i, period);
      drawLetter(heart, period);
      
  }
  
}

double getPeriod(float accelXY) {
  // v^2/r = a ==> v = sqrt(ar)
  float linVelocity = sqrt(accelXY*9.8*radius);
  // v = c/T ==> T = c/v    
  return (circumference/linVelocity);
}
void drawLetter(int let[8][7], float period) {
  float LEDtime;
  LEDtime = 1000000*period*LEDwidth/circumference;
  for(int j = 0; j<7; j++) {
    for(int i = 0; i<8; i++) {
      if(let[i][j] == 1) {
        strip.setPixelColor(7-i, 150, 0, 0);
      }
      strip.show();
    }
    for(int i = 0; i< 8; i++) {
    strip.setPixelColor(i, 0, 0, 0);
    strip.show();
    
  }
  delayMicroseconds(LEDtime/5);
  
  }
  delayMicroseconds((1000000*period));
}

