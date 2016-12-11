#include <Adafruit_NeoPixel.h>
#include <Adafruit_CircuitPlayground.h>

#include <Wire.h>
#include <SPI.h>

#define radius .0635 // 2.5 inches is .0635 meters
#define circumference .0398982267 // 2pir = circumference
#define LEDwidth .005 // 5 mm / 5 mm
#define numWaves 3
#define PIN 9

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN);

float period, accelX, accelY, accelXY;

const int a[8][7] = {
      {0,0,1,1,1,0,0},
      {0,1,0,0,0,1,0},
      {0,1,0,0,0,1,0},
      {0,1,1,1,1,1,0},
      {0,1,0,0,0,1,0},
      {0,1,0,0,0,1,0},
      {0,1,0,0,0,1,0},
      {0,1,0,0,0,1,0}};
const int b[8][7] = {
      {1,1,1,1,1,1,1},
      {1,0,0,0,0,0,1},
      {1,0,0,0,0,0,1},
      {1,1,1,1,1,1,0},
      {1,1,1,1,1,1,0},
      {1,0,0,0,0,0,1},
      {1,0,0,0,0,0,0},
      {1,1,1,1,1,1,1}};
const int c[8][7] = {
      {1,1,1,1,1,1,1},
      {1,0,0,0,0,0,0},
      {1,0,0,0,0,0,0},
      {1,0,0,0,0,0,0},
      {1,0,0,0,0,0,0},
      {1,0,0,0,0,0,0},
      {1,0,0,0,0,0,0},
      {1,1,1,1,1,1,1}};
const int d[8][7] = {
      {1,1,1,1,0,0,0},
      {1,0,0,0,1,0,0},
      {1,0,0,0,0,1,0},
      {1,0,0,0,0,0,1},
      {1,0,0,0,0,0,1},
      {1,0,0,0,0,1,0},
      {1,0,0,0,1,0,0},
      {1,1,1,1,0,0,0}};
const int e[8][7] = {
      {1,1,1,1,1,1,1},
      {1,0,0,0,0,0,0},
      {1,0,0,0,0,0,0},
      {1,1,1,1,1,1,1},
      {1,0,0,0,0,0,0},
      {1,0,0,0,0,0,0},
      {1,0,0,0,0,0,0},
      {1,1,1,1,1,1,1}};
const int f[8][7] = {
      {1,1,1,1,1,1,1},
      {1,0,0,0,0,0,0},
      {1,0,0,0,0,0,0},
      {1,1,1,1,1,1,1},
      {1,0,0,0,0,0,0},
      {1,0,0,0,0,0,0},
      {1,0,0,0,0,0,0},
      {1,0,0,0,0,0,0}};
const int g[8][7] = {
      {1,1,1,1,1,1,1},
      {1,0,0,0,0,0,0},
      {1,0,0,0,0,0,0},
      {1,0,0,1,1,1,1},
      {1,0,0,0,0,0,1},
      {1,0,0,0,0,0,1},
      {1,0,0,0,0,0,1},
      {1,1,1,1,1,1,1}};
const int h[8][7] = {
      {1,0,0,0,0,0,1},
      {1,0,0,0,0,0,1},
      {1,0,0,0,0,0,1},
      {1,1,1,1,1,1,1},
      {1,0,0,0,0,0,1},
      {1,0,0,0,0,0,1},
      {1,0,0,0,0,0,1},
      {1,0,0,0,0,0,1}
      };
const int i[8][7] = {
      {1,1,1,1,1,1,1},
      {0,0,0,1,0,0,0},
      {0,0,0,1,0,0,0},
      {0,0,0,1,0,0,0},
      {0,0,0,1,0,0,0},
      {0,0,0,1,0,0,0},
      {0,0,0,1,0,0,0},
      {1,1,1,1,1,1,1}
      };
const int j[8][7] = {
      {1,1,1,1,1,1,1},
      {0,0,0,1,0,0,0},
      {0,0,0,1,0,0,0},
      {0,0,0,1,0,0,0},
      {0,0,0,1,0,0,0},
      {0,0,0,1,0,0,0},
      {0,0,0,1,0,0,0},
      {1,1,1,1,0,0,0}
      };
const int n[8][7] = {
      {1,1,0,0,0,0,1},
      {1,1,0,0,0,0,1},
      {1,0,1,0,0,0,1},
      {1,0,1,1,0,0,1},
      {1,0,0,1,1,0,1},
      {1,0,0,0,1,0,1},
      {1,0,0,0,0,1,1},
      {1,0,0,0,0,0,1}
      };


const int heart[8][7]= {
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
      Serial.print("accelXY: ");Serial.println(accelXY);
      Serial.print("period: ");Serial.println(period);
      
      drawLetter(i, period);
      delayMicroseconds(1000000*period);
      drawLetter(heart, period);
      delayMicroseconds(1000000*period);
      drawLetter(e, period);
      delayMicroseconds(1000000*period);
      drawLetter(n, period);
      delayMicroseconds(1000000*period);
      drawLetter(g, period);
      
      
      
  }
  
}

double getPeriod(float accelXY) {
  // v^2/r = a ==> v = sqrt(ar)
  if(accelXY > .5) accelXY-=.5;
  float linVelocity = sqrt(accelXY*radius);
  // v = c/T ==> T = c/v    
  return (circumference/linVelocity);
}
void drawLetter(const int let[8][7], float period) {
  float LEDtime;
  LEDtime = 1000000*period*LEDwidth/circumference;
  for(int j = 0; j<7; j++) {
    for(int i = 0; i<8; i++) {
      if(let[i][j] == 1) {
        strip.setPixelColor(7-i, 150, 0, 0);
        
      }
    }
    strip.show();
    //delayMicroseconds((int)(LEDtime/100));
    for(int i = 0; i< 8; i++) {
    strip.setPixelColor(i, 0, 0, 0);
    strip.show();
      }
  
  
  }
  delayMicroseconds((int)(LEDtime));
}

