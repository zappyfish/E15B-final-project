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

void setup() {
  // put your setup code here, to run once:
  CircuitPlayground.begin();
  strip.begin();
  strip.show();
  pinMode(PIN, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
    
    accelX = CircuitPlayground.motionX();
    accelY = CircuitPlayground.motionY();
    accelXY = sqrt(pow(accelX, 2) + pow(accelY, 2));
    if(accelXY != 0) {
      period = getPeriod(accelXY);
      // ledFraction = period*(LEDwidth/circumference)
      drawSinusoid(period);
    }
    
}

double getPeriod(float accelXY) {
  // v^2/r = a ==> v = sqrt(ar)
  float linVelocity = sqrt(accelXY*radius);
  // v = c/T ==> T = c/v
  return (circumference/linVelocity);
  
}

void drawSinusoid(float period) {
  /* int numLEDs = circumference/LEDwidth; // this is the number of LED places in the whole circumference i.e.
  // how many LEDs could you fit within the circumference 
  int ledFraction = 1000000 * period/numLEDs; // this is the amount of time an LED should stay lit i.e. the period
  // of time for which a wait for a new action (in uSecs)
  */
  long start_time = micros();
  long end_time = start_time + (period*1000000);
  float periodperwave = 1000000 * period/numWaves; // this is the period of each of our waves in microseconds
  float coefficient = (6.28318530718)/(periodperwave); // divide periodperwave by 2pi in order to get the correct coefficient for micros() when we pass it to sin. This is because w = 2pif = 2pi/T. Thus, if we want a period of periodperwave, we must have the
  // coefficient be 2pi/T
  while(micros() <end_time) {
      strip.setPixelColor((int)(4+4*sin(micros()*coefficient)), 255, 255, 255);
  }
 
  
  
}

