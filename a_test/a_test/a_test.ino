#include <Adafruit_NeoPixel.h>
#include <Adafruit_CircuitPlayground.h>
#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>

#define radius .0635 // 2.5 inches is .0635 meters
#define circumference .0398982267 // 2pir = circumference
#define LEDwidth .005 // 5 mm / 5 mm
#define numWaves 3
#define PIN 9

#define CS 8 //chip select on pin 8
#define READ 0x80 //8th bit high for R/~W
#define WRITE 0x00

#define MBIT 0x40
#define DUMMY 0x00
#define XLOWREG 0x28 //address of X low register

#define CTRL1 0x20 //address for ctrl reg 1
#define LPEN0 0x00 // for ctrl reg 1
#define DATARATE 0x70 //for normal mode/400 hz on ctrl reg 1
#define XY_EN 0x03   //to enable X and Y axes in ctrl reg 1

#define CTRL4 0x23 //address for ctrl reg 4
#define FULLSCALE 0x00 //number for readings to full scale in ctrl reg 4
#define SIXTEENGS 0X30   // setting scale to +/- 4g
#define HR 0x08    //to enable hi res (normal) mode on ctrl reg 4

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN);


float period, accelX, accelY, accelXY;

byte accelVals[4];

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




  int red, blue, green;
  

  char readChar = 'a';
  
void setup() {
  // put your setup code here, to run once:
  CircuitPlayground.begin();
  strip.begin();
  strip.show();
  pinMode(PIN, OUTPUT);
  Serial.begin(9600);

  SPI.begin();            //initialize SPI bus
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3); //CPOL = 1, CPHA = 1
  
  //writeReg(CTRL1, (LPEN0 | DATARATE | XY_EN)); //set ctrl reg 1 to 0111 0011
 // writeReg(CTRL4, (SIXTEENGS | HR));             //set ctrl reg 4 to 0011 1000

  writeReg(0x20, 0x73);    //0111 0011
  writeReg(0x23, 0x38);    //0011 1000

  red = 150;
  blue = 0;
  green = 0;
}


void loop() {


  if(Serial.available()) {
    readChar = (char) Serial.read();
    if(red==150) red = 0;
    else if(red==0)   red = 150;
  
    if(readChar=='a') {
      red = 150;
      blue = 0;
      green = 0;
    }
    else if (readChar=='b') {
      red = 0;
      blue = 150;
      green = 0;
    }
    else if (readChar=='c') {
      red = 0;
      blue = 0;
      green = 150;
    }

    
  }
  
  mBitRead(XLOWREG, 4);

  accelX = (float) combine(accelVals[1], accelVals[0]); //combine high and low bytes into one accel value
  accelY = (float) combine(accelVals[3], accelVals[2]);

  
  accelX = accelX/100; //what you need to divide by to get ans in m/s
  accelY = accelY/100;


  accelXY = sqrt(pow(accelX, 2) + pow(accelY, 2));
    



  if(accelXY != 0) {
        period = getPeriod(accelXY);
        
        drawLetter(i, period);
        delayMicroseconds(1000000*period);
        drawLetter(heart, period);
        delayMicroseconds(1000000*period);
        drawLetter(e, period);
        delayMicroseconds(700000*period);
        drawLetter(n, period);
        delayMicroseconds(700000*period);
        drawLetter(g, period);
        delayMicroseconds(1000000*period);
        
        
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
        strip.setPixelColor(7-i, red, green, blue);
        
      }
    }
    strip.show();
    //delayMicroseconds((int)(LEDtime));
    for(int i = 0; i< 8; i++) {
    strip.setPixelColor(i, 0, 0, 0);
    strip.show();
  
  
  }
//  delayMicroseconds((int)(LEDtime));
    delayMicroseconds(700);  //arbitrary delay so that letters are not spread out too much yet do not move too quickly
}
}


void mBitRead (byte address, byte numToRead) {
  int i;
  digitalWrite(CS, LOW); //pull CS low to begin transfer
  SPI.transfer(READ| MBIT | address); //read mode, multi bit, address of reg to read
  for (i = 0; i < numToRead; i++) {
    accelVals[i] = SPI.transfer(DUMMY); //master tranfers dummy while reading into data[i]
  }
  digitalWrite(CS, HIGH);
}

int combine(byte high, byte low) { //for combining high and low bits 
  return ((((int) high)<<8) + low);
}

void writeReg(byte address, byte value) {
  digitalWrite(CS, LOW);
  SPI.transfer(WRITE | address);  //write mode and address or register to be written
  SPI.transfer(value);            //write value to register
  digitalWrite(CS, HIGH);         //pull CS high to end transfer
      }
      
