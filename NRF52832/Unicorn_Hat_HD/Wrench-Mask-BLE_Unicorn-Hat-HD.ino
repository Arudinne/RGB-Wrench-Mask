/*  Wrench Mask - BLE Version (NRF52832)
 *  
 *  Hardware Used:
 *  Adafruit Feather NRF52
 *  Pimoroni Unicorn Hat HD x2
 *  
 *  by Brandon C. Allen 
 *  
 *  BLE Functions based on the bleuart example from AdaFruit:
 *  https://github.com/adafruit/Adafruit_nRF52_Arduino/blob/master/libraries/Bluefruit52Lib/examples/Peripheral/bleuart/bleuart.ino
 *  Additional portions of the code are based on the matrixdraw example from gguuss's branch of the AdaFruit NeoPixel library:
 *  https://github.com/gguuss/Adafruit_NeoPixel/blob/master/examples/matrixdraw/matrixdraw.ino
 *  My Library for controlling the Unicorn Hat HD with the Arduino can found here:
 *  https://github.com/Arudinne/Unicorn-Hat-HD-Arduino
*/

#include "Byte_Masks.h"
#include <bluefruit.h>
#include <SPI.h>
#include <Unicorn_Hat_HD.h>

// BLE Service
BLEDis  bledis;
BLEUart bleuart;
BLEBas  blebas;

// Define the matrix CS pins
const int matrixAChipSelectPin = 7;
const int matrixBChipSelectPin = 27;

// Define the matrices
Unicorn_Hat_HD lefteye = Unicorn_Hat_HD(matrixAChipSelectPin, 90);
Unicorn_Hat_HD righteye = Unicorn_Hat_HD(matrixBChipSelectPin, 90);

// Define preset color variables
uint32_t colorRed = 0xFF0000;
uint32_t colorGreen = 0x00FF00;
uint32_t colorBlue = 0x0000FF;
uint32_t colorWhite = 0xFFFFFF;

// Define initial color
uint32_t color = colorWhite;

// Define variables for blinking the LEDs
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
int ledState = 0; //
long OnTime = 6000; // milliseconds of on-time
long OffTime = 70; // milliseconds of off-time

// Set default brightness level
byte eyeBrightness = 50;

bool drawGivenMask(int row, int col, byte mask[][ROW_SIZE])
{
  col = col % ROW_SIZE;
  if (mask[row][col] & 1){
    return true;
  }
  return false;
}

void setup()
{
  lefteye.begin();
  righteye.begin();
  lefteye.setBrightness(eyeBrightness);
  righteye.setBrightness(eyeBrightness);
  lefteye.show(); 
  righteye.show();
  
  Serial.begin(115200); // Enable Serial for debugging purposes
  Serial.println("=======================================");
  Serial.println("||   Begin RGB Wrnech Mask Program   ||");
  Serial.println("|| Opening Serial port for Debugging ||");
  Serial.println("=======================================");

  // Configure BLE
  Bluefruit.begin();
  Bluefruit.setTxPower(4);
  Bluefruit.setName("Wrench Mask");
  Bluefruit.setConnectCallback(connect_callback);
  Bluefruit.setDisconnectCallback(disconnect_callback);
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();
  bleuart.begin();
  blebas.begin();
  blebas.write(100);

  // Set up and start advertising
  startAdv();

  // Draw Initial Face Pattern
  drawFace(colorWhite, face001_Left, face001_Right);
}

void startAdv(void)
{
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(bleuart);
  Bluefruit.ScanResponse.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

void loop()
{ 
  // Respond to command and forward from BLEUART to HW Serial for Debugging
  while ( bleuart.available() )
  {
    uint8_t ch;
    ch = (uint8_t) bleuart.read();
    Serial.print("Received Command: ");
    Serial.write(ch);
    Serial.println("");
    switch (ch) 
   {
    case 0x2B: incrementBrightness(); break;
    case 0x2D: decrementBrightness(); break;
    case 0x30: drawFace(color, face010_Left, face010_Right); break; // Draw Face010
    case 0x31: drawFace(color, face001_Left, face001_Right); break; // Draw Face001
    case 0x32: drawFace(color, face002_Left, face002_Right); break; // Draw Face002
    case 0x33: drawFace(color, face003_Left, face003_Right); break; // Draw Face003
    case 0x34: drawFace(color, face004_Left, face004_Right); break; // Draw Face004
    case 0x35: drawFace(color, face005_Left, face005_Right); break; // Draw Face005
    case 0x36: drawFace(color, face006_Left, face006_Right); break; // Draw Face006
    case 0x37: drawFace(color, face007_Left, face007_Right); break; // Draw Face007
    case 0x38: drawFace(color, face008_Left, face008_Right); break; // Draw Face008
    case 0x39: drawFace(color, face009_Left, face009_Right); break; // Draw Face009
    case 0x41: drawFace(color, face011_Left, face011_Right); break; // Draw Face011
    case 0x42: drawFace(color, face012_Left, face012_Right); break; // Draw Face012
    case 0x43: drawFace(color, face013_Left, face013_Right); break; // Draw Face013
    case 0x44: drawFace(color, face014_Left, face014_Right); break; // Draw Face014
    case 0x45: drawFace(color, face015_Left, face015_Right);break; // Draw Face015
    case 0x46: drawFace(color, face016_Left, face016_Right); break; // Draw Face016
    case 0x47: drawFace(color, face017_Left, face017_Right); break; // Draw Face017
    case 0x48: drawFace(color, face018_Left, face018_Right); break; // Draw Face018
    case 0x49: drawFace(color, face019_Left, face019_Right); break; // Draw Face019
    case 0x4A: drawFace(color, face020_Left, face020_Right); break; // Draw Face020
    case 0x4B: drawFace(color, face021_Left, face021_Right); break; // Draw Face021
    case 0x4C: drawFace(color, face022_Left, face022_Right); break; // Draw Face022
    case 0x4D: drawFace(colorRed, colorWhite, face023_Red, face023_White); break; // Draw Face023
    case 0x4E: drawFace(colorRed, colorWhite, face024_Red, face024_White); break; // Draw Umbrella Corp logo
    case 0x57: setFaceColor(colorRed); break;
    case 0x58: setFaceColor(colorGreen); break;
    case 0x59: setFaceColor(colorBlue); break;
    case 0x5A: setFaceColor(colorWhite); break;
   }
  }
  blinkFunction();
}

void connect_callback(uint16_t conn_handle)
{
  char central_name[32] = { 0 };
  Bluefruit.Gap.getPeerName(conn_handle, central_name, sizeof(central_name));

  Serial.print("Connected to ");
  Serial.println(central_name);
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.println();
  Serial.println("Disconnected");
}
void rtos_idle_callback(void)
{
  waitForEvent();
}

void drawFace(uint32_t color, byte leftEye[][ROW_SIZE], byte rightEye[][ROW_SIZE])
{
maskedColorWipeEye("leftEye", color, leftEye);
maskedColorWipeEye("rightEye", color, rightEye);
}

void drawFace(uint32_t color, uint32_t color2, byte mask[][ROW_SIZE], byte mask2[][ROW_SIZE])
{
dualMaskedColorWipeEye("leftEye", color, color2, mask, mask2);
dualMaskedColorWipeEye("rightEye", color, color2, mask, mask2);
}

void incrementBrightness()
{
  if (eyeBrightness < 255){
    eyeBrightness += 1;
    Serial.println("Incrementing Brightness by 1");
    Serial.print("Brightness:");
    Serial.println(eyeBrightness);
    lefteye.setBrightness(eyeBrightness);
    righteye.setBrightness(eyeBrightness);
    lefteye.show(); 
    righteye.show();
  }
  else if(eyeBrightness = 255){
    Serial.println("Brightness already at maximum");
  }  
}

void decrementBrightness()
{
  if (eyeBrightness > 1){
    eyeBrightness -= 1;
    Serial.println("Decrementing Brightness by 1");
    Serial.print("Brightness:");
    Serial.println(eyeBrightness);
    lefteye.setBrightness(eyeBrightness);
    righteye.setBrightness(eyeBrightness);
    lefteye.show(); 
    righteye.show();
  }
  else if(eyeBrightness = 1){
    Serial.println("Brightness already at minimum");
  }  
}

void maskedColorWipeEye(String eye, uint32_t color, byte mask[][ROW_SIZE])
{
  uint16_t i, j;
  if(eye == "leftEye"){
    for(i=0; i<256; i++) {
      if (drawGivenMask(i / ROW_SIZE, i % ROW_SIZE, mask)){
        lefteye.setPixelColor(i, color);
      }
      else{
        lefteye.setPixelColor(i, 0);
      }
    }
    lefteye.show();
  }
  else if(eye == "rightEye"){
    for(i=0; i<256; i++) {
      if (drawGivenMask(i / ROW_SIZE, i % ROW_SIZE, mask)){
        righteye.setPixelColor(i, color);
      }
      else{
        righteye.setPixelColor(i, 0);
      }
    }
    righteye.show();
  }
  delay(1);
}

void dualMaskedColorWipeEye(String eye, uint32_t color, uint32_t color2, byte mask[][ROW_SIZE], byte mask2[][ROW_SIZE])
{
  uint16_t i, j;
  if(eye == "leftEye"){
    for(i=0; i<256; i++) {
      if (drawGivenMask(i / ROW_SIZE, i % ROW_SIZE, mask)){
        lefteye.setPixelColor(i, color);
      }
      else if(drawGivenMask(i / ROW_SIZE, i % ROW_SIZE, mask2)){
        lefteye.setPixelColor(i, color2);
      }
    else{
      lefteye.setPixelColor(i, 0);
      }
    }
    lefteye.show();
  }
  else if(eye == "rightEye"){
    for(i=0; i<256; i++) {
      if (drawGivenMask(i / ROW_SIZE, i % ROW_SIZE, mask)){
        righteye.setPixelColor(i, color);
      }
      else if(drawGivenMask(i / ROW_SIZE, i % ROW_SIZE, mask2)){
        righteye.setPixelColor(i, color2);
      }
    else{
      righteye.setPixelColor(i, 0);
      }
    }
    righteye.show();
  }

  delay(1);
}

void blinkFunction()
{
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1; // Set the LEDs status to on
    previousMillis = currentMillis;   // Remember the time
    Serial.println("Blink - LEDs ON"); // Debug output
    lefteye.setBrightness(eyeBrightness);
    righteye.setBrightness(eyeBrightness);
    lefteye.show(); 
    righteye.show();
  }
  else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
  {
    ledState = 0; // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
    Serial.println("Blink - LEDs OFF");
    lefteye.setBrightness(0);
    righteye.setBrightness(0);
    lefteye.show(); 
    righteye.show();
    
  }
  delay(10);
}

void setFaceColor(uint32_t colorVariable)
{
  color=colorVariable;
}
