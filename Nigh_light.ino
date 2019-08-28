#include <SPI.h>
#include <Wire.h>
#include <Sodaq_DS3231.h>
int ledPin = 5; 
#include "FastLED.h"
#define NUM_LEDS 12
CRGB leds[NUM_LEDS];
#define PIN A3

void setup()   
{                
 Serial.begin(9600);
 rtc.begin();
 //setRTCTime();
 FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  while (!Serial) {
       delay(1);
  }
}
void loop() 
{
    meteorRain(0, 127, 255,2, 150, true, 70);
    DateTime now = rtc.now(); //get the current date-time
    Serial.print("Time->"); 
    Serial.print( now.hour() ,DEC);
    Serial.print(":"); 
    Serial.println( now.minute() ,DEC); 
    if(06<=now.hour()&&now.hour()<=21)
     {
        analogWrite(ledPin, 0);
     }
    else
     {
    
          for (int fadeValue = 100 ; fadeValue <= 200; fadeValue += 5) {
            // sets the value (range from 0 to 255):
            analogWrite(ledPin, fadeValue);
            delay(160);
          }
        
          // fade out from max to min in increments of 5 points:
          for (int fadeValue = 200 ; fadeValue >= 100; fadeValue -= 5) {
            // sets the value (range from 0 to 255):
            analogWrite(ledPin, fadeValue);
            delay(140);
         }
    }
}
void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
 
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
   
   
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        setPixel(i-j, red, green, blue);
      }
    }
   
    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    strip.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}
void setRTCTime()
{
  DateTime dt(2019, 8, 26, 22, 30 , 30, 7); // Year, Month, Day, Hour, Minutes, Seconds, Day of Week
  rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above 
}
