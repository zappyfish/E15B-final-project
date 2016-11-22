#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, 9);


float accelX, accelY, accelXY;
int addr = 0, maxAccel;

void setup() {
  // put your setup code here, to run once:
  CircuitPlayground.begin();
  strip.begin();
  strip.show();
  maxAccel = EEPROM.read(addr);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("max: ");Serial.println(maxAccel);
  accelX = CircuitPlayground.motionX();
  accelY = CircuitPlayground.motionY();
  accelXY = sqrt(pow(accelX, 2) + pow(accelY, 2));
  Serial.print("accelXY: ");Serial.println(accelXY);
  if((int)accelXY > maxAccel) {
    EEPROM.write(addr, (int)accelXY);
    maxAccel = accelXY;
    for(int i = 0; i<8; i++) {
      strip.setPixelColor(i, 100, 100, 100);
      strip.show();
    }
    delay(500);
    for(int i = 0; i<8; i++) {
      strip.setPixelColor(i, 0, 0, 0);
      strip.show();
    }
  }
}
