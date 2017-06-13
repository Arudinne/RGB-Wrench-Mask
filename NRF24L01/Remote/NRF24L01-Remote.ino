/* Wrench Mask Remote - NRF24L01 Version
 *  
 *  by Brandon C. Allen
 *  
 *  Hardware Used:
 *  Adafruit Feather M0 Proto
 *  https://www.adafruit.com/product/2772
 *  Eight Adafruit DotStar High Density 8x8 Grid - 64 RGB LED Pixel Matrices
 *  https://www.adafruit.com/product/3444
 *  NRF24L01 2.4GHz Transciever
 *  https://www.amazon.com/gp/product/B016PP62QO
 *  5mm NeoPixel
 *  https://www.adafruit.com/product/1938
 *  74AHCT125 - Quad Level-Shifter
 *  https://www.adafruit.com/product/1787
 *  
 * Based on the following Sketches:
 * --------------------------------
 * 
 *  Joe Young Custom Keypad - MC17
 *  https://github.com/joeyoung/arduino_keypads/tree/master/Keypad_MC17
 *  
 *  Arduino Wireless Communication Tutorial
 *       Example 1 - Receiver Code
 * by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
*/

#include <SPI.h>
#include "RF24.h"
#include <Keypad_MC17.h>
#include <Keypad.h>        // GDY120705
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define PIN A0
#define I2CADDR 0x20

const byte ROWS = 7; //four rows
const byte COLS = 5; //five columns
//define the cymbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  {'0','0','0','C','3'},
  {'0','J','9','H','L'},
  {'B','K','D','4','7'},
  {'G','W','8','A','O'},
  {'R','N','5','0','6'},
  {'0','M','I','F','2'},
  {'0','0','0','E','1'}
};
byte rowPins[ROWS] = {0, 1, 2, 3, 4, 5, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {12, 11, 10, 9, 8}; //connect to the column pinouts of the keypad

/*
1: X X - Face001
2: > < - Face002
3: ^ ^ - Face003
4: @ @ - Face004
5: / \ - Face005-
6: ? ? - Face006
7: O o - Face007
8: \ / - Face008
9: Z Z - Face009
0: # # - Face010
A: = = - Face011
C: ~ ^ - Face012
D: 9 9 - Face013
E: ! ! - Face014
F: * * - Face015
H: ` ` - Face016
I: ; ; - Face017
J: .) .) - Face018
K: (. (. - Face019
L: o O - Face020
M: . . - Face021
N: ⌐ ⌐ - Face022
O: <3 <3 - Face023
*/



//initialize an instance of class NewKeypad
Keypad_MC17 customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR);

 /****************** User Config ***************************/
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 11 & 10 */
RF24 radio(11, 10);
/**********************************************************/
byte address[6] = "01337";

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

void setup(){
  char StartCheck1 = 'W';
  char StartCheck2 = '1';
  
  Wire.begin();
  customKeypad.begin( );        // GDY120705
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  radio.write(&StartCheck1, sizeof(StartCheck1));
  radio.write(&StartCheck2, sizeof(StartCheck2));
  strip.begin();
  strip.setPixelColor(0, 10, 10, 10);
  strip.show();
}

  
void loop(){
  char key = customKeypad.getKey();
  
  if (key != NO_KEY){
    Serial.println(key);
    radio.write(&key, sizeof(key));
   if (key == 'R'){
      strip.setPixelColor(0, 0, 10, 0);
      strip.show();
   }
   else if (key == 'G'){
      strip.setPixelColor(0, 10, 0, 0);
      strip.show();
   }
   else if (key == 'B'){
      strip.setPixelColor(0, 0, 0, 10);
      strip.show();
   }
   else if (key == 'W'){
      strip.setPixelColor(0, 10, 10, 10);
      strip.show();
   }
 }
}
