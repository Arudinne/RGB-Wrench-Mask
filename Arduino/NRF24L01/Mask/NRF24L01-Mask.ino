/* Wrench Mask - NRF24L01 Version
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
 *  74AHCT125 - Quad Level-Shifter
 *  https://www.adafruit.com/product/1787
 *  
 * Based on the following Sketches:
 * --------------------------------
 * DotStar Belly Dance Fans
 *  by Erin St Blaine
 * 
 * Blink without Delay
 *  created 2005
 *  by David A. Mellis
 *  modified 8 Feb 2010
 *  by Paul Stoffregen
 *  
 *  Arduino Wireless Communication Tutorial
 *       Example 1 - Receiver Code
 * by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
*/

// Call required libraries
#include <Adafruit_DotStar.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define the number of pixels
#define NUMPIXELS 512

// Define Data & Clock Pins
#define DATAPIN    6
#define CLOCKPIN   5

// Build the strip
Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

// Required for Serial on Zero based boards
#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif

// Configurre the NRF24L01 SPI pins
RF24 radio(11, 10);

// Configure the reciever channel
byte address[6] = "01337";

/*
1: X X - Face001
2: > < - Face002
3: ^ ^ - Face003
4: @ @ - Face004
5: / \ - Face005
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

// Begin Setup
void setup() 
{
  Serial.begin(9600); // Enable Serial for debugging purposes
  delay(1000); // Delay to allow Serial Debug to initalize if being used
  Serial.println("=======================================");
  Serial.println("|| Opening Serial port for Debugging ||");
  Serial.println("||     Begin Wrnech Mask Program     ||");
  Serial.println("=======================================");
  radio.begin(); // Start the NRF24L01 Radio
  radio.openReadingPipe(1, address); // Configure the Listening Pipe
  radio.setPALevel(RF24_PA_HIGH); // Set Power Level - might be unnecessary for RX
  radio.startListening(); // Start Listening for Commands from the Remote
  strip.begin(); // Initialize strip pins for output
  strip.show();  // Turn all LEDs off ASAP
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

// Define LED colors in one place so they can be easily changed
// Colors are in decimal format
uint32_t white = 1052688;
uint32_t green = 1048576;
uint32_t red = 4096;
uint32_t blue = 16;
uint32_t color = white; // Define initial color
int colorsetting = 0; // Define Variable to toggle LED colors

// Initial boolean variables for face status checks when switching faces
// Face001
bool Face001RunningCheck = false;
bool Face001BlinkCheck = false;
// Face002 
bool Face002RunningCheck = false;
bool Face002BlinkCheck = false;
// Face003
bool Face003RunningCheck = false;
bool Face003BlinkCheck = false; 
// Face004
bool Face004RunningCheck = false;
bool Face004BlinkCheck = false;
// Face005
bool Face005RunningCheck = false;
bool Face005BlinkCheck = false;
// Face006 
bool Face006RunningCheck = false;
bool Face006BlinkCheck = false;
// Face007
bool Face007RunningCheck = false;
bool Face007BlinkCheck = false; 
// Face008
bool Face008RunningCheck = false;
bool Face008BlinkCheck = false;
// Face009
bool Face009RunningCheck = false;
bool Face009BlinkCheck = false; 
// Face010
bool Face010RunningCheck = false;
bool Face010BlinkCheck = false;
// Face011
bool Face011RunningCheck = false;
bool Face011BlinkCheck = false;
// Face012
bool Face012RunningCheck = false;
bool Face012BlinkCheck = false;
// Face013
bool Face013RunningCheck = false;
bool Face013BlinkCheck = false;
// Face014
bool Face014RunningCheck = false;
bool Face014BlinkCheck = false;
// Face015
bool Face015RunningCheck = false;
bool Face015BlinkCheck = false;
// Face016
bool Face016RunningCheck = false;
bool Face016BlinkCheck = false;
// Face017
bool Face017RunningCheck = false;
bool Face017BlinkCheck = false;
// Face018
bool Face018RunningCheck = false;
bool Face018BlinkCheck = false;
// Face019
bool Face019RunningCheck = false;
bool Face019BlinkCheck = false;
// Face020
bool Face020RunningCheck = false;
bool Face020BlinkCheck = false;
// Face021
bool Face021RunningCheck = false;
bool Face021BlinkCheck = false;
// Face022
bool Face022RunningCheck = false;
bool Face022BlinkCheck = false;
// Face023
bool Face023RunningCheck = false;
bool Face023BlinkCheck = false;

// Define Variables for blinking the LEDs
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
int ledState = 0; //
long OnTime = 6000; // milliseconds of on-time
long OffTime = 50; // milliseconds of off-time

char FaceMode = 0;  //First active face

// Main Loop ------------------------------------------------------------------------------------------------
void loop() 
{
  if (radio.available()) // Check if a command has been recieved
  {
    char text[32] = ""; // Define character array to recieve the command
    radio.read(&text, sizeof(text)); // Read command into character array
    String textstring(text); // Converting character array to resolve issue with garbled input - for some reason this fixess it. ¯\_(ツ)_/¯
    // Debug output
    Serial.print("Command Received. Button: "); 
    Serial.println(textstring);
    // Convert String back to char for switch command. ¯\_(ツ)_/¯
    if(textstring == "1") // Draw Face001
    {
    FaceMode=0;
    }
    else if(textstring == "2") // Draw Face002
    { 
    FaceMode=1; 
    }
    else if(textstring == "3") // Draw Face003
    { 
    FaceMode=2; 
    }
    else if(textstring == "4") // Draw Face004
    { 
    FaceMode=3; 
    }
    else if(textstring == "5") // Draw Face005
    { 
    FaceMode=4; 
    }
    else if(textstring == "6") // Draw Face006
    { 
    FaceMode=5; 
    }
    else if(textstring == "7") // Draw Face007
    { 
    FaceMode=6; 
    }
    else if(textstring == "8") // Draw Face008
    { 
    FaceMode=7; 
    }
    else if(textstring == "9") // Draw Face009
    { 
    FaceMode=8; 
    }
    else if(textstring == "0") // Draw Face010
    { 
    FaceMode=9;
    }
    else if(textstring == "A") // Draw Face011
    { 
    FaceMode='A';
    } 
    else if(textstring == "I") // Draw Face012
    { 
    FaceMode='I';
    } 
    else if(textstring == "C") // Draw Face013
    { 
    FaceMode='C';
    } 
    else if(textstring == "D") // Draw Face014
    { 
    FaceMode='D';
    } 
    else if(textstring == "E") // Draw Face015
    { 
    FaceMode='E';
    } 
    else if(textstring == "F") // Draw Face016
    { 
    FaceMode='F';  
    }
    else if(textstring == "H") // Draw Face017
    { 
    FaceMode='H';  
    }
    else if(textstring == "J") // Draw Face018
    { 
    FaceMode='J';  
    }
    else if(textstring == "K") // Draw Face019
    { 
    FaceMode='K';  
    }
    else if(textstring == "L") // Draw Face020
    { 
    FaceMode='L';  
    }
    else if(textstring == "M") // Draw Face021
    { 
    FaceMode='M';  
    }
    else if(textstring == "N") // Draw Face022
    { 
    FaceMode='N';  
    }
    else if(textstring == "O") // Draw Face023
    { 
    FaceMode='O';  
    }
    else if(textstring == "R") // Change the color to Red
    {
      color = red;
      Serial.println("Color Changed to: Red"); 
    }
    else if(textstring == "G") // Change the color to Red
    {
      color = green;
      Serial.println("Color Changed to: Green"); 
    }
    else if(textstring == "B") // Change the color to Red
    {
      color = blue;
      Serial.println("Color Changed to: Blue"); 
    }
    else if(textstring == "W") // Change the color to Red
    {
      color = white;
      Serial.println("Color Changed to: White"); 
    }
   }      
   
   switch (FaceMode) 
   {
       case 0: Face001(); break; // Draw Face001
       case 1: Face002(); break; // Draw Face002
       case 2: Face003(); break; // Draw Face003
       case 3: Face004(); break; // Draw Face004
       case 4: Face005(); break; // Draw Face005
       case 5: Face006(); break; // Draw Face006
       case 6: Face007(); break; // Draw Face007
       case 7: Face008(); break; // Draw Face008
       case 8: Face009(); break; // Draw Face009
       case 9: Face010(); break; // Draw Face010
       case 'A': Face011(); break; // Draw Face011
       case 'I': Face012(); break; // Draw Face012
       case 'C': Face013(); break; // Draw Face013
       case 'D': Face014(); break; // Draw Face014
       case 'E': Face015(); break; // Draw Face015
       case 'F': Face016(); break; // Draw Face016
       case 'H': Face017(); break; // Draw Face017
       case 'J': Face018(); break; // Draw Face018
       case 'K': Face019(); break; // Draw Face019
       case 'L': Face020(); break; // Draw Face020
       case 'M': Face021(); break; // Draw Face021
       case 'N': Face022(); break; // Draw Face022
       case 'O': Face023(); break; // Draw Face023
       
   }
}
// Draw Face001 ---------------------------------------------------------------------------------------------
void Face001()
{
  if (Face001RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    // Debug output
    Serial.println("Switching to Face001");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face
    strip.setPixelColor(19, color); 
    strip.setPixelColor(20, color); 
    strip.setPixelColor(21, color); 
    strip.setPixelColor(28, color); 
    strip.setPixelColor(29, color); 
    strip.setPixelColor(30, color); 
    strip.setPixelColor(36, color); 
    strip.setPixelColor(37, color); 
    strip.setPixelColor(38, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(54, color); 
    strip.setPixelColor(55, color); 
    strip.setPixelColor(62, color); 
    strip.setPixelColor(63, color); 
    strip.setPixelColor(71, color); 
    strip.setPixelColor(78, color); 
    strip.setPixelColor(79, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(86, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(93, color); 
    strip.setPixelColor(94, color); 
    strip.setPixelColor(100, color); 
    strip.setPixelColor(101, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(107, color); 
    strip.setPixelColor(108, color); 
    strip.setPixelColor(109, color);
    strip.setPixelColor(147, color); 
    strip.setPixelColor(148, color); 
    strip.setPixelColor(149, color); 
    strip.setPixelColor(154, color); 
    strip.setPixelColor(155, color); 
    strip.setPixelColor(156, color); 
    strip.setPixelColor(162, color); 
    strip.setPixelColor(163, color); 
    strip.setPixelColor(164, color); 
    strip.setPixelColor(169, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(176, color); 
    strip.setPixelColor(177, color); 
    strip.setPixelColor(178, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(185, color); 
    strip.setPixelColor(192, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(217, color); 
    strip.setPixelColor(218, color); 
    strip.setPixelColor(219, color); 
    strip.setPixelColor(226, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(228, color); 
    strip.setPixelColor(234, color); 
    strip.setPixelColor(235, color); 
    strip.setPixelColor(236, color);
    strip.setPixelColor(275, color); 
    strip.setPixelColor(276, color); 
    strip.setPixelColor(277, color); 
    strip.setPixelColor(284, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(294, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(303, color); 
    strip.setPixelColor(310, color); 
    strip.setPixelColor(311, color); 
    strip.setPixelColor(319, color); 
    strip.setPixelColor(326, color); 
    strip.setPixelColor(327, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(335, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(348, color); 
    strip.setPixelColor(349, color); 
    strip.setPixelColor(350, color); 
    strip.setPixelColor(356, color); 
    strip.setPixelColor(357, color); 
    strip.setPixelColor(358, color); 
    strip.setPixelColor(363, color); 
    strip.setPixelColor(364, color); 
    strip.setPixelColor(365, color); 
    strip.setPixelColor(402, color); 
    strip.setPixelColor(403, color); 
    strip.setPixelColor(404, color); 
    strip.setPixelColor(410, color); 
    strip.setPixelColor(411, color); 
    strip.setPixelColor(412, color); 
    strip.setPixelColor(417, color); 
    strip.setPixelColor(418, color); 
    strip.setPixelColor(419, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(432, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(440, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(448, color); 
    strip.setPixelColor(449, color); 
    strip.setPixelColor(456, color); 
    strip.setPixelColor(457, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(465, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(473, color); 
    strip.setPixelColor(474, color); 
    strip.setPixelColor(475, color); 
    strip.setPixelColor(482, color); 
    strip.setPixelColor(483, color); 
    strip.setPixelColor(484, color); 
    strip.setPixelColor(491, color); 
    strip.setPixelColor(492, color); 
    strip.setPixelColor(493, color);
    strip.show(); // Turn on the LEDs 
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = true; // Face001 - this face
    Face002RunningCheck = false; // Face002 
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face001BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1; // Set the LEDs status to on
    previousMillis = currentMillis;   // Remember the time
    Serial.println("Face001 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face
    strip.setPixelColor(19, color); 
    strip.setPixelColor(20, color); 
    strip.setPixelColor(21, color); 
    strip.setPixelColor(28, color); 
    strip.setPixelColor(29, color); 
    strip.setPixelColor(30, color); 
    strip.setPixelColor(36, color); 
    strip.setPixelColor(37, color); 
    strip.setPixelColor(38, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(54, color); 
    strip.setPixelColor(55, color); 
    strip.setPixelColor(62, color); 
    strip.setPixelColor(63, color); 
    strip.setPixelColor(71, color); 
    strip.setPixelColor(78, color); 
    strip.setPixelColor(79, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(86, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(93, color); 
    strip.setPixelColor(94, color); 
    strip.setPixelColor(100, color); 
    strip.setPixelColor(101, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(107, color); 
    strip.setPixelColor(108, color); 
    strip.setPixelColor(109, color);
    strip.setPixelColor(147, color); 
    strip.setPixelColor(148, color); 
    strip.setPixelColor(149, color); 
    strip.setPixelColor(154, color); 
    strip.setPixelColor(155, color); 
    strip.setPixelColor(156, color); 
    strip.setPixelColor(162, color); 
    strip.setPixelColor(163, color); 
    strip.setPixelColor(164, color); 
    strip.setPixelColor(169, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(176, color); 
    strip.setPixelColor(177, color); 
    strip.setPixelColor(178, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(185, color); 
    strip.setPixelColor(192, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(217, color); 
    strip.setPixelColor(218, color); 
    strip.setPixelColor(219, color); 
    strip.setPixelColor(226, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(228, color); 
    strip.setPixelColor(234, color); 
    strip.setPixelColor(235, color); 
    strip.setPixelColor(236, color);
    strip.setPixelColor(275, color); 
    strip.setPixelColor(276, color); 
    strip.setPixelColor(277, color); 
    strip.setPixelColor(284, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(294, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(303, color); 
    strip.setPixelColor(310, color); 
    strip.setPixelColor(311, color); 
    strip.setPixelColor(319, color); 
    strip.setPixelColor(326, color); 
    strip.setPixelColor(327, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(335, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(348, color); 
    strip.setPixelColor(349, color); 
    strip.setPixelColor(350, color); 
    strip.setPixelColor(356, color); 
    strip.setPixelColor(357, color); 
    strip.setPixelColor(358, color); 
    strip.setPixelColor(363, color); 
    strip.setPixelColor(364, color); 
    strip.setPixelColor(365, color); 
    strip.setPixelColor(402, color); 
    strip.setPixelColor(403, color); 
    strip.setPixelColor(404, color); 
    strip.setPixelColor(410, color); 
    strip.setPixelColor(411, color); 
    strip.setPixelColor(412, color); 
    strip.setPixelColor(417, color); 
    strip.setPixelColor(418, color); 
    strip.setPixelColor(419, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(432, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(440, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(448, color); 
    strip.setPixelColor(449, color); 
    strip.setPixelColor(456, color); 
    strip.setPixelColor(457, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(465, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(473, color); 
    strip.setPixelColor(474, color); 
    strip.setPixelColor(475, color); 
    strip.setPixelColor(482, color); 
    strip.setPixelColor(483, color); 
    strip.setPixelColor(484, color); 
    strip.setPixelColor(491, color); 
    strip.setPixelColor(492, color); 
    strip.setPixelColor(493, color); 
    strip.show(); // Turn on the LEDs 
  }
  else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
  {
    ledState = 0; // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
    if(Face001BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face001 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face001BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
    }
  }  
}

// Draw Face002 ---------------------------------------------------------------------------------------------
void Face002()
{
  if (Face002RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face002");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face
    strip.setPixelColor(20, color); 
    strip.setPixelColor(28, color); 
    strip.setPixelColor(29, color); 
    strip.setPixelColor(30, color); 
    strip.setPixelColor(37, color); 
    strip.setPixelColor(38, color); 
    strip.setPixelColor(39, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(93, color); 
    strip.setPixelColor(94, color); 
    strip.setPixelColor(95, color); 
    strip.setPixelColor(101, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(103, color); 
    strip.setPixelColor(109, color); 
    strip.setPixelColor(160, color); 
    strip.setPixelColor(161, color); 
    strip.setPixelColor(168, color); 
    strip.setPixelColor(169, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(177, color); 
    strip.setPixelColor(178, color); 
    strip.setPixelColor(179, color); 
    strip.setPixelColor(180, color); 
    strip.setPixelColor(181, color); 
    strip.setPixelColor(188, color); 
    strip.setPixelColor(189, color); 
    strip.setPixelColor(190, color); 
    strip.setPixelColor(195, color); 
    strip.setPixelColor(196, color); 
    strip.setPixelColor(197, color); 
    strip.setPixelColor(198, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(203, color); 
    strip.setPixelColor(204, color); 
    strip.setPixelColor(205, color); 
    strip.setPixelColor(208, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(216, color); 
    strip.setPixelColor(217, color); 
    strip.setPixelColor(276, color); 
    strip.setPixelColor(284, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(292, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(294, color); 
    strip.setPixelColor(295, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(303, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(349, color); 
    strip.setPixelColor(350, color); 
    strip.setPixelColor(351, color); 
    strip.setPixelColor(356, color); 
    strip.setPixelColor(357, color); 
    strip.setPixelColor(358, color); 
    strip.setPixelColor(364, color); 
    strip.setPixelColor(416, color); 
    strip.setPixelColor(424, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(435, color); 
    strip.setPixelColor(436, color); 
    strip.setPixelColor(437, color); 
    strip.setPixelColor(443, color); 
    strip.setPixelColor(444, color); 
    strip.setPixelColor(445, color); 
    strip.setPixelColor(446, color); 
    strip.setPixelColor(452, color); 
    strip.setPixelColor(453, color); 
    strip.setPixelColor(454, color); 
    strip.setPixelColor(457, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(459, color); 
    strip.setPixelColor(460, color); 
    strip.setPixelColor(461, color); 
    strip.setPixelColor(464, color); 
    strip.setPixelColor(465, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(472, color); 
    strip.setPixelColor(473, color);
    strip.show(); // Turn on the LEDs 
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = true; // Face002 - this face
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face002BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face002 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face
    strip.setPixelColor(20, color); 
    strip.setPixelColor(28, color); 
    strip.setPixelColor(29, color); 
    strip.setPixelColor(30, color); 
    strip.setPixelColor(37, color); 
    strip.setPixelColor(38, color); 
    strip.setPixelColor(39, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(93, color); 
    strip.setPixelColor(94, color); 
    strip.setPixelColor(95, color); 
    strip.setPixelColor(101, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(103, color); 
    strip.setPixelColor(109, color); 
    strip.setPixelColor(160, color); 
    strip.setPixelColor(161, color); 
    strip.setPixelColor(168, color); 
    strip.setPixelColor(169, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(177, color); 
    strip.setPixelColor(178, color); 
    strip.setPixelColor(179, color); 
    strip.setPixelColor(180, color); 
    strip.setPixelColor(181, color); 
    strip.setPixelColor(188, color); 
    strip.setPixelColor(189, color); 
    strip.setPixelColor(190, color); 
    strip.setPixelColor(195, color); 
    strip.setPixelColor(196, color); 
    strip.setPixelColor(197, color); 
    strip.setPixelColor(198, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(203, color); 
    strip.setPixelColor(204, color); 
    strip.setPixelColor(205, color); 
    strip.setPixelColor(208, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(216, color); 
    strip.setPixelColor(217, color); 
    strip.setPixelColor(276, color); 
    strip.setPixelColor(284, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(292, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(294, color); 
    strip.setPixelColor(295, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(303, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(349, color); 
    strip.setPixelColor(350, color); 
    strip.setPixelColor(351, color); 
    strip.setPixelColor(356, color); 
    strip.setPixelColor(357, color); 
    strip.setPixelColor(358, color); 
    strip.setPixelColor(364, color); 
    strip.setPixelColor(416, color); 
    strip.setPixelColor(424, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(435, color); 
    strip.setPixelColor(436, color); 
    strip.setPixelColor(437, color); 
    strip.setPixelColor(443, color); 
    strip.setPixelColor(444, color); 
    strip.setPixelColor(445, color); 
    strip.setPixelColor(446, color); 
    strip.setPixelColor(452, color); 
    strip.setPixelColor(453, color); 
    strip.setPixelColor(454, color); 
    strip.setPixelColor(457, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(459, color); 
    strip.setPixelColor(460, color); 
    strip.setPixelColor(461, color); 
    strip.setPixelColor(464, color); 
    strip.setPixelColor(465, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(472, color); 
    strip.setPixelColor(473, color);
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face002BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face002 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face002BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face003 ---------------------------------------------------------------------------------------------
void Face003()
{
  if (Face003RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face003");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face
    strip.setPixelColor(59, color); 
    strip.setPixelColor(60, color); 
    strip.setPixelColor(67, color); 
    strip.setPixelColor(68, color); 
    strip.setPixelColor(69, color); 
    strip.setPixelColor(76, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(84, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(93, color); 
    strip.setPixelColor(94, color); 
    strip.setPixelColor(101, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(103, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(111, color); 
    strip.setPixelColor(118, color); 
    strip.setPixelColor(119, color); 
    strip.setPixelColor(186, color); 
    strip.setPixelColor(187, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(194, color); 
    strip.setPixelColor(195, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(208, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(216, color); 
    strip.setPixelColor(217, color); 
    strip.setPixelColor(224, color); 
    strip.setPixelColor(225, color); 
    strip.setPixelColor(232, color); 
    strip.setPixelColor(240, color); 
    strip.setPixelColor(270, color); 
    strip.setPixelColor(271, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(279, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(287, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(294, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(315, color); 
    strip.setPixelColor(316, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(323, color); 
    strip.setPixelColor(324, color); 
    strip.setPixelColor(392, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(408, color); 
    strip.setPixelColor(409, color); 
    strip.setPixelColor(416, color); 
    strip.setPixelColor(417, color); 
    strip.setPixelColor(424, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(442, color); 
    strip.setPixelColor(443, color); 
    strip.setPixelColor(450, color); 
    strip.setPixelColor(451, color); 
    strip.show(); // Turn on the LEDs 
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = true; // Face003 - this face
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face003BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face004 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face
    strip.setPixelColor(59, color); 
    strip.setPixelColor(60, color); 
    strip.setPixelColor(67, color); 
    strip.setPixelColor(68, color); 
    strip.setPixelColor(69, color); 
    strip.setPixelColor(76, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(84, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(93, color); 
    strip.setPixelColor(94, color); 
    strip.setPixelColor(101, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(103, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(111, color); 
    strip.setPixelColor(118, color); 
    strip.setPixelColor(119, color); 
    strip.setPixelColor(186, color); 
    strip.setPixelColor(187, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(194, color); 
    strip.setPixelColor(195, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(208, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(216, color); 
    strip.setPixelColor(217, color); 
    strip.setPixelColor(224, color); 
    strip.setPixelColor(225, color); 
    strip.setPixelColor(232, color); 
    strip.setPixelColor(240, color); 
    strip.setPixelColor(270, color); 
    strip.setPixelColor(271, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(279, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(287, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(294, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(315, color); 
    strip.setPixelColor(316, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(323, color); 
    strip.setPixelColor(324, color); 
    strip.setPixelColor(392, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(408, color); 
    strip.setPixelColor(409, color); 
    strip.setPixelColor(416, color); 
    strip.setPixelColor(417, color); 
    strip.setPixelColor(424, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(442, color); 
    strip.setPixelColor(443, color); 
    strip.setPixelColor(450, color); 
    strip.setPixelColor(451, color); 
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face003BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face003 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face003BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}   
// Draw Face004 ---------------------------------------------------------------------------------------------
void Face004()
{
  if (Face004RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face004");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face
    strip.setPixelColor(21, color); 
    strip.setPixelColor(22, color); 
    strip.setPixelColor(23, color); 
    strip.setPixelColor(28, color); 
    strip.setPixelColor(29, color); 
    strip.setPixelColor(35, color); 
    strip.setPixelColor(36, color); 
    strip.setPixelColor(38, color); 
    strip.setPixelColor(39, color); 
    strip.setPixelColor(42, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(50, color); 
    strip.setPixelColor(51, color); 
    strip.setPixelColor(52, color); 
    strip.setPixelColor(53, color); 
    strip.setPixelColor(58, color); 
    strip.setPixelColor(60, color); 
    strip.setPixelColor(61, color); 
    strip.setPixelColor(66, color); 
    strip.setPixelColor(69, color); 
    strip.setPixelColor(74, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(78, color); 
    strip.setPixelColor(82, color); 
    strip.setPixelColor(83, color); 
    strip.setPixelColor(86, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(91, color); 
    strip.setPixelColor(100, color); 
    strip.setPixelColor(109, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(111, color); 
    strip.setPixelColor(119, color); 
    strip.setPixelColor(144, color); 
    strip.setPixelColor(145, color); 
    strip.setPixelColor(146, color); 
    strip.setPixelColor(147, color); 
    strip.setPixelColor(155, color); 
    strip.setPixelColor(156, color); 
    strip.setPixelColor(161, color); 
    strip.setPixelColor(162, color); 
    strip.setPixelColor(168, color); 
    strip.setPixelColor(169, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(172, color); 
    strip.setPixelColor(177, color); 
    strip.setPixelColor(180, color); 
    strip.setPixelColor(185, color); 
    strip.setPixelColor(186, color); 
    strip.setPixelColor(189, color); 
    strip.setPixelColor(194, color); 
    strip.setPixelColor(197, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(205, color); 
    strip.setPixelColor(208, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(213, color); 
    strip.setPixelColor(216, color); 
    strip.setPixelColor(220, color); 
    strip.setPixelColor(221, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(228, color); 
    strip.setPixelColor(232, color); 
    strip.setPixelColor(233, color); 
    strip.setPixelColor(234, color); 
    strip.setPixelColor(235, color); 
    strip.setPixelColor(240, color); 
    strip.setPixelColor(270, color); 
    strip.setPixelColor(271, color); 
    strip.setPixelColor(276, color); 
    strip.setPixelColor(277, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(279, color); 
    strip.setPixelColor(283, color); 
    strip.setPixelColor(290, color); 
    strip.setPixelColor(295, color); 
    strip.setPixelColor(298, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(303, color); 
    strip.setPixelColor(306, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(314, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(322, color); 
    strip.setPixelColor(325, color); 
    strip.setPixelColor(330, color); 
    strip.setPixelColor(331, color); 
    strip.setPixelColor(333, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(339, color); 
    strip.setPixelColor(340, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(349, color); 
    strip.setPixelColor(355, color); 
    strip.setPixelColor(364, color); 
    strip.setPixelColor(365, color); 
    strip.setPixelColor(366, color); 
    strip.setPixelColor(367, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(401, color); 
    strip.setPixelColor(402, color); 
    strip.setPixelColor(410, color); 
    strip.setPixelColor(411, color); 
    strip.setPixelColor(419, color); 
    strip.setPixelColor(420, color); 
    strip.setPixelColor(424, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(428, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(436, color); 
    strip.setPixelColor(437, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(442, color); 
    strip.setPixelColor(444, color); 
    strip.setPixelColor(445, color); 
    strip.setPixelColor(450, color); 
    strip.setPixelColor(452, color); 
    strip.setPixelColor(453, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(460, color); 
    strip.setPixelColor(461, color); 
    strip.setPixelColor(464, color); 
    strip.setPixelColor(465, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(468, color); 
    strip.setPixelColor(472, color); 
    strip.setPixelColor(473, color); 
    strip.setPixelColor(475, color); 
    strip.setPixelColor(476, color); 
    strip.setPixelColor(482, color); 
    strip.setPixelColor(482, color); 
    strip.setPixelColor(488, color); 
    strip.setPixelColor(489, color); 
    strip.setPixelColor(490, color);  
    strip.show(); // Turn on the LEDs 
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = true; // Face004 - this face
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face004BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face004 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face
    strip.setPixelColor(21, color); 
    strip.setPixelColor(22, color); 
    strip.setPixelColor(23, color); 
    strip.setPixelColor(28, color); 
    strip.setPixelColor(29, color); 
    strip.setPixelColor(35, color); 
    strip.setPixelColor(36, color); 
    strip.setPixelColor(38, color); 
    strip.setPixelColor(39, color); 
    strip.setPixelColor(42, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(50, color); 
    strip.setPixelColor(51, color); 
    strip.setPixelColor(52, color); 
    strip.setPixelColor(53, color); 
    strip.setPixelColor(58, color); 
    strip.setPixelColor(60, color); 
    strip.setPixelColor(61, color); 
    strip.setPixelColor(66, color); 
    strip.setPixelColor(69, color); 
    strip.setPixelColor(74, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(78, color); 
    strip.setPixelColor(82, color); 
    strip.setPixelColor(83, color); 
    strip.setPixelColor(86, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(91, color); 
    strip.setPixelColor(100, color); 
    strip.setPixelColor(109, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(111, color); 
    strip.setPixelColor(119, color); 
    strip.setPixelColor(144, color); 
    strip.setPixelColor(145, color); 
    strip.setPixelColor(146, color); 
    strip.setPixelColor(147, color); 
    strip.setPixelColor(155, color); 
    strip.setPixelColor(156, color); 
    strip.setPixelColor(161, color); 
    strip.setPixelColor(162, color); 
    strip.setPixelColor(168, color); 
    strip.setPixelColor(169, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(172, color); 
    strip.setPixelColor(177, color); 
    strip.setPixelColor(180, color); 
    strip.setPixelColor(185, color); 
    strip.setPixelColor(186, color); 
    strip.setPixelColor(189, color); 
    strip.setPixelColor(194, color); 
    strip.setPixelColor(197, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(205, color); 
    strip.setPixelColor(208, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(213, color); 
    strip.setPixelColor(216, color); 
    strip.setPixelColor(220, color); 
    strip.setPixelColor(221, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(228, color); 
    strip.setPixelColor(232, color); 
    strip.setPixelColor(233, color); 
    strip.setPixelColor(234, color); 
    strip.setPixelColor(235, color); 
    strip.setPixelColor(240, color); 
    strip.setPixelColor(270, color); 
    strip.setPixelColor(271, color); 
    strip.setPixelColor(276, color); 
    strip.setPixelColor(277, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(279, color); 
    strip.setPixelColor(283, color); 
    strip.setPixelColor(290, color); 
    strip.setPixelColor(295, color); 
    strip.setPixelColor(298, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(303, color); 
    strip.setPixelColor(306, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(314, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(322, color); 
    strip.setPixelColor(325, color); 
    strip.setPixelColor(330, color); 
    strip.setPixelColor(331, color); 
    strip.setPixelColor(333, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(339, color); 
    strip.setPixelColor(340, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(349, color); 
    strip.setPixelColor(355, color); 
    strip.setPixelColor(364, color); 
    strip.setPixelColor(365, color); 
    strip.setPixelColor(366, color); 
    strip.setPixelColor(367, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(401, color); 
    strip.setPixelColor(402, color); 
    strip.setPixelColor(410, color); 
    strip.setPixelColor(411, color); 
    strip.setPixelColor(419, color); 
    strip.setPixelColor(420, color); 
    strip.setPixelColor(424, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(428, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(436, color); 
    strip.setPixelColor(437, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(442, color); 
    strip.setPixelColor(444, color); 
    strip.setPixelColor(445, color); 
    strip.setPixelColor(450, color); 
    strip.setPixelColor(452, color); 
    strip.setPixelColor(453, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(460, color); 
    strip.setPixelColor(461, color); 
    strip.setPixelColor(464, color); 
    strip.setPixelColor(465, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(468, color); 
    strip.setPixelColor(472, color); 
    strip.setPixelColor(473, color); 
    strip.setPixelColor(475, color); 
    strip.setPixelColor(476, color); 
    strip.setPixelColor(482, color); 
    strip.setPixelColor(482, color); 
    strip.setPixelColor(488, color); 
    strip.setPixelColor(489, color); 
    strip.setPixelColor(490, color);  
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face004BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face004 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face004BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face005 ---------------------------------------------------------------------------------------------
void Face005()
{
  if (Face005RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face005");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.show(); // Turn on the LEDs 
    strip.setPixelColor(10, color); 
    strip.setPixelColor(11, color); 
    strip.setPixelColor(12, color); 
    strip.setPixelColor(19, color); 
    strip.setPixelColor(20, color); 
    strip.setPixelColor(21, color); 
    strip.setPixelColor(27, color); 
    strip.setPixelColor(28, color); 
    strip.setPixelColor(29, color); 
    strip.setPixelColor(36, color); 
    strip.setPixelColor(37, color); 
    strip.setPixelColor(38, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(54, color); 
    strip.setPixelColor(55, color); 
    strip.setPixelColor(63, color); 
    strip.setPixelColor(71, color); 
    strip.setPixelColor(176, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(192, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(217, color); 
    strip.setPixelColor(218, color); 
    strip.setPixelColor(219, color); 
    strip.setPixelColor(226, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(228, color); 
    strip.setPixelColor(235, color); 
    strip.setPixelColor(236, color); 
    strip.setPixelColor(237, color); 
    strip.setPixelColor(244, color); 
    strip.setPixelColor(245, color); 
    strip.setPixelColor(319, color); 
    strip.setPixelColor(327, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(335, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(348, color); 
    strip.setPixelColor(349, color); 
    strip.setPixelColor(350, color); 
    strip.setPixelColor(356, color); 
    strip.setPixelColor(357, color); 
    strip.setPixelColor(363, color); 
    strip.setPixelColor(364, color); 
    strip.setPixelColor(365, color); 
    strip.setPixelColor(370, color); 
    strip.setPixelColor(371, color); 
    strip.setPixelColor(372, color); 
    strip.setPixelColor(396, color); 
    strip.setPixelColor(397, color); 
    strip.setPixelColor(403, color); 
    strip.setPixelColor(404, color); 
    strip.setPixelColor(405, color); 
    strip.setPixelColor(410, color); 
    strip.setPixelColor(411, color); 
    strip.setPixelColor(412, color); 
    strip.setPixelColor(417, color); 
    strip.setPixelColor(418, color); 
    strip.setPixelColor(419, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(432, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(440, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(448, color); 
    strip.setPixelColor(456, color);
    strip.show(); // Turn on the LEDs 
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = true; // Face005 - this face
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face005BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face005 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(10, color); 
    strip.setPixelColor(11, color); 
    strip.setPixelColor(12, color); 
    strip.setPixelColor(19, color); 
    strip.setPixelColor(20, color); 
    strip.setPixelColor(21, color); 
    strip.setPixelColor(27, color); 
    strip.setPixelColor(28, color); 
    strip.setPixelColor(29, color); 
    strip.setPixelColor(36, color); 
    strip.setPixelColor(37, color); 
    strip.setPixelColor(38, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(54, color); 
    strip.setPixelColor(55, color); 
    strip.setPixelColor(63, color); 
    strip.setPixelColor(71, color); 
    strip.setPixelColor(176, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(192, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(217, color); 
    strip.setPixelColor(218, color); 
    strip.setPixelColor(219, color); 
    strip.setPixelColor(226, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(228, color); 
    strip.setPixelColor(235, color); 
    strip.setPixelColor(236, color); 
    strip.setPixelColor(237, color); 
    strip.setPixelColor(244, color); 
    strip.setPixelColor(245, color); 
    strip.setPixelColor(319, color); 
    strip.setPixelColor(327, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(335, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(348, color); 
    strip.setPixelColor(349, color); 
    strip.setPixelColor(350, color); 
    strip.setPixelColor(356, color); 
    strip.setPixelColor(357, color); 
    strip.setPixelColor(363, color); 
    strip.setPixelColor(364, color); 
    strip.setPixelColor(365, color); 
    strip.setPixelColor(370, color); 
    strip.setPixelColor(371, color); 
    strip.setPixelColor(372, color); 
    strip.setPixelColor(396, color); 
    strip.setPixelColor(397, color); 
    strip.setPixelColor(403, color); 
    strip.setPixelColor(404, color); 
    strip.setPixelColor(405, color); 
    strip.setPixelColor(410, color); 
    strip.setPixelColor(411, color); 
    strip.setPixelColor(412, color); 
    strip.setPixelColor(417, color); 
    strip.setPixelColor(418, color); 
    strip.setPixelColor(419, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(432, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(440, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(448, color); 
    strip.setPixelColor(456, color); 
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face005BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face005 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face005BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face006 ---------------------------------------------------------------------------------------------
void Face006()
{
  if (Face006RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face006");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(6, color); 
    strip.setPixelColor(7, color); 
    strip.setPixelColor(14, color); 
    strip.setPixelColor(15, color); 
    strip.setPixelColor(22, color); 
    strip.setPixelColor(23, color); 
    strip.setPixelColor(38, color); 
    strip.setPixelColor(39, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(55, color); 
    strip.setPixelColor(63, color); 
    strip.setPixelColor(91, color); 
    strip.setPixelColor(92, color); 
    strip.setPixelColor(99, color); 
    strip.setPixelColor(100, color); 
    strip.setPixelColor(107, color); 
    strip.setPixelColor(108, color); 
    strip.setPixelColor(109, color); 
    strip.setPixelColor(116, color); 
    strip.setPixelColor(117, color); 
    strip.setPixelColor(118, color); 
    strip.setPixelColor(119, color); 
    strip.setPixelColor(125, color); 
    strip.setPixelColor(126, color); 
    strip.setPixelColor(127, color); 
    strip.setPixelColor(128, color); 
    strip.setPixelColor(136, color); 
    strip.setPixelColor(144, color); 
    strip.setPixelColor(160, color); 
    strip.setPixelColor(168, color); 
    strip.setPixelColor(176, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(185, color); 
    strip.setPixelColor(192, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(194, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(203, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(212, color); 
    strip.setPixelColor(218, color); 
    strip.setPixelColor(219, color); 
    strip.setPixelColor(220, color); 
    strip.setPixelColor(226, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(228, color); 
    strip.setPixelColor(233, color); 
    strip.setPixelColor(234, color); 
    strip.setPixelColor(235, color); 
    strip.setPixelColor(240, color); 
    strip.setPixelColor(241, color); 
    strip.setPixelColor(242, color); 
    strip.setPixelColor(243, color); 
    strip.setPixelColor(248, color); 
    strip.setPixelColor(249, color); 
    strip.setPixelColor(261, color); 
    strip.setPixelColor(262, color); 
    strip.setPixelColor(263, color); 
    strip.setPixelColor(268, color); 
    strip.setPixelColor(269, color); 
    strip.setPixelColor(270, color); 
    strip.setPixelColor(271, color); 
    strip.setPixelColor(275, color); 
    strip.setPixelColor(276, color); 
    strip.setPixelColor(277, color); 
    strip.setPixelColor(283, color); 
    strip.setPixelColor(284, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(291, color); 
    strip.setPixelColor(292, color); 
    strip.setPixelColor(299, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(307, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(316, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(318, color); 
    strip.setPixelColor(319, color); 
    strip.setPixelColor(325, color); 
    strip.setPixelColor(326, color); 
    strip.setPixelColor(327, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(335, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(351, color); 
    strip.setPixelColor(366, color); 
    strip.setPixelColor(367, color); 
    strip.setPixelColor(374, color); 
    strip.setPixelColor(375, color); 
    strip.setPixelColor(382, color); 
    strip.setPixelColor(383, color); 
    strip.setPixelColor(384, color); 
    strip.setPixelColor(385, color); 
    strip.setPixelColor(392, color); 
    strip.setPixelColor(393, color); 
    strip.setPixelColor(394, color); 
    strip.setPixelColor(401, color); 
    strip.setPixelColor(402, color); 
    strip.setPixelColor(403, color); 
    strip.setPixelColor(410, color); 
    strip.setPixelColor(411, color); 
    strip.setPixelColor(418, color); 
    strip.setPixelColor(419, color); 
    strip.setPixelColor(420, color); 
    strip.setPixelColor(456, color); 
    strip.setPixelColor(464, color); 
    strip.setPixelColor(472, color); 
    strip.setPixelColor(488, color); 
    strip.setPixelColor(496, color); 
    strip.setPixelColor(504, color);
    strip.show(); // Turn on the LEDs 
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = true; // Face006 - this face
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face006BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face006 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(6, color); 
    strip.setPixelColor(7, color); 
    strip.setPixelColor(14, color); 
    strip.setPixelColor(15, color); 
    strip.setPixelColor(22, color); 
    strip.setPixelColor(23, color); 
    strip.setPixelColor(38, color); 
    strip.setPixelColor(39, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(55, color); 
    strip.setPixelColor(63, color); 
    strip.setPixelColor(91, color); 
    strip.setPixelColor(92, color); 
    strip.setPixelColor(99, color); 
    strip.setPixelColor(100, color); 
    strip.setPixelColor(107, color); 
    strip.setPixelColor(108, color); 
    strip.setPixelColor(109, color); 
    strip.setPixelColor(116, color); 
    strip.setPixelColor(117, color); 
    strip.setPixelColor(118, color); 
    strip.setPixelColor(119, color); 
    strip.setPixelColor(125, color); 
    strip.setPixelColor(126, color); 
    strip.setPixelColor(127, color); 
    strip.setPixelColor(128, color); 
    strip.setPixelColor(136, color); 
    strip.setPixelColor(144, color); 
    strip.setPixelColor(160, color); 
    strip.setPixelColor(168, color); 
    strip.setPixelColor(176, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(185, color); 
    strip.setPixelColor(192, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(194, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(203, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(212, color); 
    strip.setPixelColor(218, color); 
    strip.setPixelColor(219, color); 
    strip.setPixelColor(220, color); 
    strip.setPixelColor(226, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(228, color); 
    strip.setPixelColor(233, color); 
    strip.setPixelColor(234, color); 
    strip.setPixelColor(235, color); 
    strip.setPixelColor(240, color); 
    strip.setPixelColor(241, color); 
    strip.setPixelColor(242, color); 
    strip.setPixelColor(243, color); 
    strip.setPixelColor(248, color); 
    strip.setPixelColor(249, color); 
    strip.setPixelColor(261, color); 
    strip.setPixelColor(262, color); 
    strip.setPixelColor(263, color); 
    strip.setPixelColor(268, color); 
    strip.setPixelColor(269, color); 
    strip.setPixelColor(270, color); 
    strip.setPixelColor(271, color); 
    strip.setPixelColor(275, color); 
    strip.setPixelColor(276, color); 
    strip.setPixelColor(277, color); 
    strip.setPixelColor(283, color); 
    strip.setPixelColor(284, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(291, color); 
    strip.setPixelColor(292, color); 
    strip.setPixelColor(299, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(307, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(316, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(318, color); 
    strip.setPixelColor(319, color); 
    strip.setPixelColor(325, color); 
    strip.setPixelColor(326, color); 
    strip.setPixelColor(327, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(335, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(351, color); 
    strip.setPixelColor(366, color); 
    strip.setPixelColor(367, color); 
    strip.setPixelColor(374, color); 
    strip.setPixelColor(375, color); 
    strip.setPixelColor(382, color); 
    strip.setPixelColor(383, color); 
    strip.setPixelColor(384, color); 
    strip.setPixelColor(385, color); 
    strip.setPixelColor(392, color); 
    strip.setPixelColor(393, color); 
    strip.setPixelColor(394, color); 
    strip.setPixelColor(401, color); 
    strip.setPixelColor(402, color); 
    strip.setPixelColor(403, color); 
    strip.setPixelColor(410, color); 
    strip.setPixelColor(411, color); 
    strip.setPixelColor(418, color); 
    strip.setPixelColor(419, color); 
    strip.setPixelColor(420, color); 
    strip.setPixelColor(456, color); 
    strip.setPixelColor(464, color); 
    strip.setPixelColor(472, color); 
    strip.setPixelColor(488, color); 
    strip.setPixelColor(496, color); 
    strip.setPixelColor(504, color); 
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face006BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face006 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face006BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face007 ---------------------------------------------------------------------------------------------
void Face007()
{
  if (Face007RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face007");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(21, color); 
    strip.setPixelColor(22, color); 
    strip.setPixelColor(23, color); 
    strip.setPixelColor(28, color); 
    strip.setPixelColor(29, color); 
    strip.setPixelColor(30, color); 
    strip.setPixelColor(31, color); 
    strip.setPixelColor(35, color); 
    strip.setPixelColor(36, color); 
    strip.setPixelColor(42, color); 
    strip.setPixelColor(43, color); 
    strip.setPixelColor(50, color); 
    strip.setPixelColor(51, color); 
    strip.setPixelColor(58, color); 
    strip.setPixelColor(59, color); 
    strip.setPixelColor(66, color); 
    strip.setPixelColor(67, color); 
    strip.setPixelColor(74, color); 
    strip.setPixelColor(75, color); 
    strip.setPixelColor(82, color); 
    strip.setPixelColor(83, color); 
    strip.setPixelColor(91, color); 
    strip.setPixelColor(92, color); 
    strip.setPixelColor(99, color); 
    strip.setPixelColor(100, color); 
    strip.setPixelColor(101, color); 
    strip.setPixelColor(108, color); 
    strip.setPixelColor(109, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(111, color); 
    strip.setPixelColor(118, color); 
    strip.setPixelColor(119, color); 
    strip.setPixelColor(144, color); 
    strip.setPixelColor(145, color); 
    strip.setPixelColor(152, color); 
    strip.setPixelColor(153, color); 
    strip.setPixelColor(154, color); 
    strip.setPixelColor(155, color); 
    strip.setPixelColor(162, color); 
    strip.setPixelColor(163, color); 
    strip.setPixelColor(164, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(172, color); 
    strip.setPixelColor(180, color); 
    strip.setPixelColor(181, color); 
    strip.setPixelColor(188, color); 
    strip.setPixelColor(189, color); 
    strip.setPixelColor(196, color); 
    strip.setPixelColor(197, color); 
    strip.setPixelColor(204, color); 
    strip.setPixelColor(205, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(212, color); 
    strip.setPixelColor(213, color); 
    strip.setPixelColor(219, color); 
    strip.setPixelColor(220, color); 
    strip.setPixelColor(226, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(228, color); 
    strip.setPixelColor(232, color); 
    strip.setPixelColor(233, color); 
    strip.setPixelColor(234, color); 
    strip.setPixelColor(235, color); 
    strip.setPixelColor(240, color); 
    strip.setPixelColor(241, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(287, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(294, color); 
    strip.setPixelColor(295, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(316, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(324, color); 
    strip.setPixelColor(325, color); 
    strip.setPixelColor(332, color); 
    strip.setPixelColor(333, color); 
    strip.setPixelColor(340, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(349, color); 
    strip.setPixelColor(350, color); 
    strip.setPixelColor(351, color); 
    strip.setPixelColor(359, color); 
    strip.setPixelColor(408, color); 
    strip.setPixelColor(409, color); 
    strip.setPixelColor(416, color); 
    strip.setPixelColor(417, color); 
    strip.setPixelColor(418, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(427, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(435, color); 
    strip.setPixelColor(443, color); 
    strip.setPixelColor(444, color); 
    strip.setPixelColor(451, color); 
    strip.setPixelColor(452, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(459, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(472, color); 
    strip.setPixelColor(473, color); 
    strip.setPixelColor(474, color); 
    strip.setPixelColor(480, color);
    strip.show(); // Turn on the LEDs 
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = true; // Face007 - this face
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face007BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face007 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(21, color); 
    strip.setPixelColor(22, color); 
    strip.setPixelColor(23, color); 
    strip.setPixelColor(28, color); 
    strip.setPixelColor(29, color); 
    strip.setPixelColor(30, color); 
    strip.setPixelColor(31, color); 
    strip.setPixelColor(35, color); 
    strip.setPixelColor(36, color); 
    strip.setPixelColor(42, color); 
    strip.setPixelColor(43, color); 
    strip.setPixelColor(50, color); 
    strip.setPixelColor(51, color); 
    strip.setPixelColor(58, color); 
    strip.setPixelColor(59, color); 
    strip.setPixelColor(66, color); 
    strip.setPixelColor(67, color); 
    strip.setPixelColor(74, color); 
    strip.setPixelColor(75, color); 
    strip.setPixelColor(82, color); 
    strip.setPixelColor(83, color); 
    strip.setPixelColor(91, color); 
    strip.setPixelColor(92, color); 
    strip.setPixelColor(99, color); 
    strip.setPixelColor(100, color); 
    strip.setPixelColor(101, color); 
    strip.setPixelColor(108, color); 
    strip.setPixelColor(109, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(111, color); 
    strip.setPixelColor(118, color); 
    strip.setPixelColor(119, color); 
    strip.setPixelColor(144, color); 
    strip.setPixelColor(145, color); 
    strip.setPixelColor(152, color); 
    strip.setPixelColor(153, color); 
    strip.setPixelColor(154, color); 
    strip.setPixelColor(155, color); 
    strip.setPixelColor(162, color); 
    strip.setPixelColor(163, color); 
    strip.setPixelColor(164, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(172, color); 
    strip.setPixelColor(180, color); 
    strip.setPixelColor(181, color); 
    strip.setPixelColor(188, color); 
    strip.setPixelColor(189, color); 
    strip.setPixelColor(196, color); 
    strip.setPixelColor(197, color); 
    strip.setPixelColor(204, color); 
    strip.setPixelColor(205, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(212, color); 
    strip.setPixelColor(213, color); 
    strip.setPixelColor(219, color); 
    strip.setPixelColor(220, color); 
    strip.setPixelColor(226, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(228, color); 
    strip.setPixelColor(232, color); 
    strip.setPixelColor(233, color); 
    strip.setPixelColor(234, color); 
    strip.setPixelColor(235, color); 
    strip.setPixelColor(240, color); 
    strip.setPixelColor(241, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(287, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(294, color); 
    strip.setPixelColor(295, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(316, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(324, color); 
    strip.setPixelColor(325, color); 
    strip.setPixelColor(332, color); 
    strip.setPixelColor(333, color); 
    strip.setPixelColor(340, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(349, color); 
    strip.setPixelColor(350, color); 
    strip.setPixelColor(351, color); 
    strip.setPixelColor(359, color); 
    strip.setPixelColor(408, color); 
    strip.setPixelColor(409, color); 
    strip.setPixelColor(416, color); 
    strip.setPixelColor(417, color); 
    strip.setPixelColor(418, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(427, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(435, color); 
    strip.setPixelColor(443, color); 
    strip.setPixelColor(444, color); 
    strip.setPixelColor(451, color); 
    strip.setPixelColor(452, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(459, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(472, color); 
    strip.setPixelColor(473, color); 
    strip.setPixelColor(474, color); 
    strip.setPixelColor(480, color); 
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face007BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face007 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face007BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face008 ---------------------------------------------------------------------------------------------
void Face008()
{
  if (Face008RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face008");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(71, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(78, color); 
    strip.setPixelColor(79, color); 
    strip.setPixelColor(84, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(86, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(91, color); 
    strip.setPixelColor(92, color); 
    strip.setPixelColor(93, color); 
    strip.setPixelColor(94, color); 
    strip.setPixelColor(98, color); 
    strip.setPixelColor(99, color); 
    strip.setPixelColor(100, color); 
    strip.setPixelColor(105, color); 
    strip.setPixelColor(106, color); 
    strip.setPixelColor(107, color); 
    strip.setPixelColor(150, color); 
    strip.setPixelColor(157, color); 
    strip.setPixelColor(158, color); 
    strip.setPixelColor(159, color); 
    strip.setPixelColor(164, color); 
    strip.setPixelColor(165, color); 
    strip.setPixelColor(166, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(172, color); 
    strip.setPixelColor(173, color); 
    strip.setPixelColor(177, color); 
    strip.setPixelColor(178, color); 
    strip.setPixelColor(179, color); 
    strip.setPixelColor(180, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(185, color); 
    strip.setPixelColor(186, color); 
    strip.setPixelColor(192, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(273, color); 
    strip.setPixelColor(274, color); 
    strip.setPixelColor(275, color); 
    strip.setPixelColor(282, color); 
    strip.setPixelColor(283, color); 
    strip.setPixelColor(284, color); 
    strip.setPixelColor(291, color); 
    strip.setPixelColor(292, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(294, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(303, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(310, color); 
    strip.setPixelColor(311, color); 
    strip.setPixelColor(319, color); 
    strip.setPixelColor(432, color); 
    strip.setPixelColor(440, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(448, color); 
    strip.setPixelColor(449, color); 
    strip.setPixelColor(450, color); 
    strip.setPixelColor(457, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(459, color); 
    strip.setPixelColor(460, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(468, color); 
    strip.setPixelColor(469, color); 
    strip.setPixelColor(476, color); 
    strip.setPixelColor(477, color); 
    strip.setPixelColor(478, color); 
    strip.setPixelColor(485, color); 
    strip.setPixelColor(486, color); 
    strip.setPixelColor(487, color); 
    strip.setPixelColor(494, color);
    strip.show(); // Turn on the LEDs 
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = true; // Face008 - this face
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face008BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face008 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(71, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(78, color); 
    strip.setPixelColor(79, color); 
    strip.setPixelColor(84, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(86, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(91, color); 
    strip.setPixelColor(92, color); 
    strip.setPixelColor(93, color); 
    strip.setPixelColor(94, color); 
    strip.setPixelColor(98, color); 
    strip.setPixelColor(99, color); 
    strip.setPixelColor(100, color); 
    strip.setPixelColor(105, color); 
    strip.setPixelColor(106, color); 
    strip.setPixelColor(107, color); 
    strip.setPixelColor(150, color); 
    strip.setPixelColor(157, color); 
    strip.setPixelColor(158, color); 
    strip.setPixelColor(159, color); 
    strip.setPixelColor(164, color); 
    strip.setPixelColor(165, color); 
    strip.setPixelColor(166, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(172, color); 
    strip.setPixelColor(173, color); 
    strip.setPixelColor(177, color); 
    strip.setPixelColor(178, color); 
    strip.setPixelColor(179, color); 
    strip.setPixelColor(180, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(185, color); 
    strip.setPixelColor(186, color); 
    strip.setPixelColor(192, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(273, color); 
    strip.setPixelColor(274, color); 
    strip.setPixelColor(275, color); 
    strip.setPixelColor(282, color); 
    strip.setPixelColor(283, color); 
    strip.setPixelColor(284, color); 
    strip.setPixelColor(291, color); 
    strip.setPixelColor(292, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(294, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(303, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(310, color); 
    strip.setPixelColor(311, color); 
    strip.setPixelColor(319, color); 
    strip.setPixelColor(432, color); 
    strip.setPixelColor(440, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(448, color); 
    strip.setPixelColor(449, color); 
    strip.setPixelColor(450, color); 
    strip.setPixelColor(457, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(459, color); 
    strip.setPixelColor(460, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(468, color); 
    strip.setPixelColor(469, color); 
    strip.setPixelColor(476, color); 
    strip.setPixelColor(477, color); 
    strip.setPixelColor(478, color); 
    strip.setPixelColor(485, color); 
    strip.setPixelColor(486, color); 
    strip.setPixelColor(487, color); 
    strip.setPixelColor(494, color); 
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face008BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face008 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face008BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face009 ---------------------------------------------------------------------------------------------
void Face009()
{
  if (Face009RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face009");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(19, color); 
    strip.setPixelColor(20, color); 
    strip.setPixelColor(21, color); 
    strip.setPixelColor(22, color); 
    strip.setPixelColor(23, color); 
    strip.setPixelColor(27, color); 
    strip.setPixelColor(28, color); 
    strip.setPixelColor(29, color); 
    strip.setPixelColor(30, color); 
    strip.setPixelColor(31, color); 
    strip.setPixelColor(35, color); 
    strip.setPixelColor(36, color); 
    strip.setPixelColor(37, color); 
    strip.setPixelColor(44, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(53, color); 
    strip.setPixelColor(54, color); 
    strip.setPixelColor(55, color); 
    strip.setPixelColor(62, color); 
    strip.setPixelColor(63, color); 
    strip.setPixelColor(71, color); 
    strip.setPixelColor(99, color); 
    strip.setPixelColor(100, color); 
    strip.setPixelColor(101, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(103, color); 
    strip.setPixelColor(107, color); 
    strip.setPixelColor(108, color); 
    strip.setPixelColor(109, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(111, color); 
    strip.setPixelColor(144, color); 
    strip.setPixelColor(145, color); 
    strip.setPixelColor(146, color); 
    strip.setPixelColor(147, color); 
    strip.setPixelColor(148, color); 
    strip.setPixelColor(152, color); 
    strip.setPixelColor(153, color); 
    strip.setPixelColor(154, color); 
    strip.setPixelColor(155, color); 
    strip.setPixelColor(156, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(192, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(218, color); 
    strip.setPixelColor(219, color); 
    strip.setPixelColor(220, color); 
    strip.setPixelColor(224, color); 
    strip.setPixelColor(225, color); 
    strip.setPixelColor(226, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(228, color); 
    strip.setPixelColor(232, color); 
    strip.setPixelColor(233, color); 
    strip.setPixelColor(234, color); 
    strip.setPixelColor(235, color); 
    strip.setPixelColor(236, color); 
    strip.setPixelColor(275, color); 
    strip.setPixelColor(276, color); 
    strip.setPixelColor(277, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(279, color); 
    strip.setPixelColor(283, color); 
    strip.setPixelColor(284, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(287, color); 
    strip.setPixelColor(291, color); 
    strip.setPixelColor(292, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(310, color); 
    strip.setPixelColor(311, color); 
    strip.setPixelColor(318, color); 
    strip.setPixelColor(319, color); 
    strip.setPixelColor(327, color); 
    strip.setPixelColor(355, color); 
    strip.setPixelColor(356, color); 
    strip.setPixelColor(357, color); 
    strip.setPixelColor(358, color); 
    strip.setPixelColor(359, color); 
    strip.setPixelColor(363, color); 
    strip.setPixelColor(364, color); 
    strip.setPixelColor(365, color); 
    strip.setPixelColor(366, color); 
    strip.setPixelColor(367, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(401, color); 
    strip.setPixelColor(402, color); 
    strip.setPixelColor(403, color); 
    strip.setPixelColor(404, color); 
    strip.setPixelColor(408, color); 
    strip.setPixelColor(409, color); 
    strip.setPixelColor(410, color); 
    strip.setPixelColor(411, color); 
    strip.setPixelColor(412, color); 
    strip.setPixelColor(440, color); 
    strip.setPixelColor(448, color); 
    strip.setPixelColor(449, color); 
    strip.setPixelColor(456, color); 
    strip.setPixelColor(457, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(465, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(474, color); 
    strip.setPixelColor(475, color); 
    strip.setPixelColor(476, color); 
    strip.setPixelColor(480, color); 
    strip.setPixelColor(481, color); 
    strip.setPixelColor(482, color); 
    strip.setPixelColor(483, color); 
    strip.setPixelColor(484, color); 
    strip.setPixelColor(488, color); 
    strip.setPixelColor(489, color); 
    strip.setPixelColor(490, color); 
    strip.setPixelColor(491, color); 
    strip.setPixelColor(492, color);
    strip.show(); // Turn on the LEDs
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = true; // Face009 - this face
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face009BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face009 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(19, color); 
    strip.setPixelColor(20, color); 
    strip.setPixelColor(21, color); 
    strip.setPixelColor(22, color); 
    strip.setPixelColor(23, color); 
    strip.setPixelColor(27, color); 
    strip.setPixelColor(28, color); 
    strip.setPixelColor(29, color); 
    strip.setPixelColor(30, color); 
    strip.setPixelColor(31, color); 
    strip.setPixelColor(35, color); 
    strip.setPixelColor(36, color); 
    strip.setPixelColor(37, color); 
    strip.setPixelColor(44, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(53, color); 
    strip.setPixelColor(54, color); 
    strip.setPixelColor(55, color); 
    strip.setPixelColor(62, color); 
    strip.setPixelColor(63, color); 
    strip.setPixelColor(71, color); 
    strip.setPixelColor(99, color); 
    strip.setPixelColor(100, color); 
    strip.setPixelColor(101, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(103, color); 
    strip.setPixelColor(107, color); 
    strip.setPixelColor(108, color); 
    strip.setPixelColor(109, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(111, color); 
    strip.setPixelColor(144, color); 
    strip.setPixelColor(145, color); 
    strip.setPixelColor(146, color); 
    strip.setPixelColor(147, color); 
    strip.setPixelColor(148, color); 
    strip.setPixelColor(152, color); 
    strip.setPixelColor(153, color); 
    strip.setPixelColor(154, color); 
    strip.setPixelColor(155, color); 
    strip.setPixelColor(156, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(192, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(218, color); 
    strip.setPixelColor(219, color); 
    strip.setPixelColor(220, color); 
    strip.setPixelColor(224, color); 
    strip.setPixelColor(225, color); 
    strip.setPixelColor(226, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(228, color); 
    strip.setPixelColor(232, color); 
    strip.setPixelColor(233, color); 
    strip.setPixelColor(234, color); 
    strip.setPixelColor(235, color); 
    strip.setPixelColor(236, color); 
    strip.setPixelColor(275, color); 
    strip.setPixelColor(276, color); 
    strip.setPixelColor(277, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(279, color); 
    strip.setPixelColor(283, color); 
    strip.setPixelColor(284, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(287, color); 
    strip.setPixelColor(291, color); 
    strip.setPixelColor(292, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(310, color); 
    strip.setPixelColor(311, color); 
    strip.setPixelColor(318, color); 
    strip.setPixelColor(319, color); 
    strip.setPixelColor(327, color); 
    strip.setPixelColor(355, color); 
    strip.setPixelColor(356, color); 
    strip.setPixelColor(357, color); 
    strip.setPixelColor(358, color); 
    strip.setPixelColor(359, color); 
    strip.setPixelColor(363, color); 
    strip.setPixelColor(364, color); 
    strip.setPixelColor(365, color); 
    strip.setPixelColor(366, color); 
    strip.setPixelColor(367, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(401, color); 
    strip.setPixelColor(402, color); 
    strip.setPixelColor(403, color); 
    strip.setPixelColor(404, color); 
    strip.setPixelColor(408, color); 
    strip.setPixelColor(409, color); 
    strip.setPixelColor(410, color); 
    strip.setPixelColor(411, color); 
    strip.setPixelColor(412, color); 
    strip.setPixelColor(440, color); 
    strip.setPixelColor(448, color); 
    strip.setPixelColor(449, color); 
    strip.setPixelColor(456, color); 
    strip.setPixelColor(457, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(465, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(474, color); 
    strip.setPixelColor(475, color); 
    strip.setPixelColor(476, color); 
    strip.setPixelColor(480, color); 
    strip.setPixelColor(481, color); 
    strip.setPixelColor(482, color); 
    strip.setPixelColor(483, color); 
    strip.setPixelColor(484, color); 
    strip.setPixelColor(488, color); 
    strip.setPixelColor(489, color); 
    strip.setPixelColor(490, color); 
    strip.setPixelColor(491, color); 
    strip.setPixelColor(492, color);
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face009BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face009 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face009BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face010 ---------------------------------------------------------------------------------------------
void Face010()
{
  if (Face010RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face010");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(4, color); 
    strip.setPixelColor(5, color); 
    strip.setPixelColor(12, color); 
    strip.setPixelColor(13, color); 
    strip.setPixelColor(20, color); 
    strip.setPixelColor(21, color); 
    strip.setPixelColor(29, color); 
    strip.setPixelColor(37, color); 
    strip.setPixelColor(38, color); 
    strip.setPixelColor(43, color); 
    strip.setPixelColor(44, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(53, color); 
    strip.setPixelColor(54, color); 
    strip.setPixelColor(61, color); 
    strip.setPixelColor(62, color); 
    strip.setPixelColor(69, color); 
    strip.setPixelColor(70, color); 
    strip.setPixelColor(75, color); 
    strip.setPixelColor(76, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(78, color); 
    strip.setPixelColor(79, color); 
    strip.setPixelColor(83, color); 
    strip.setPixelColor(84, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(86, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(94, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(103, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(111, color); 
    strip.setPixelColor(118, color); 
    strip.setPixelColor(119, color); 
    strip.setPixelColor(128, color); 
    strip.setPixelColor(129, color); 
    strip.setPixelColor(136, color); 
    strip.setPixelColor(137, color); 
    strip.setPixelColor(145, color); 
    strip.setPixelColor(153, color); 
    strip.setPixelColor(154, color); 
    strip.setPixelColor(161, color); 
    strip.setPixelColor(162, color); 
    strip.setPixelColor(168, color); 
    strip.setPixelColor(169, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(172, color); 
    strip.setPixelColor(177, color); 
    strip.setPixelColor(178, color); 
    strip.setPixelColor(185, color); 
    strip.setPixelColor(186, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(194, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(203, color); 
    strip.setPixelColor(204, color); 
    strip.setPixelColor(208, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(212, color); 
    strip.setPixelColor(218, color); 
    strip.setPixelColor(219, color); 
    strip.setPixelColor(226, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(234, color); 
    strip.setPixelColor(235, color); 
    strip.setPixelColor(242, color); 
    strip.setPixelColor(243, color); 
    strip.setPixelColor(268, color); 
    strip.setPixelColor(269, color); 
    strip.setPixelColor(276, color); 
    strip.setPixelColor(277, color); 
    strip.setPixelColor(284, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(292, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(299, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(303, color); 
    strip.setPixelColor(307, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(310, color); 
    strip.setPixelColor(311, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(318, color); 
    strip.setPixelColor(325, color); 
    strip.setPixelColor(326, color); 
    strip.setPixelColor(333, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(339, color); 
    strip.setPixelColor(340, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(349, color); 
    strip.setPixelColor(350, color); 
    strip.setPixelColor(357, color); 
    strip.setPixelColor(358, color); 
    strip.setPixelColor(366, color); 
    strip.setPixelColor(374, color); 
    strip.setPixelColor(375, color); 
    strip.setPixelColor(382, color); 
    strip.setPixelColor(383, color); 
    strip.setPixelColor(392, color); 
    strip.setPixelColor(393, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(401, color); 
    strip.setPixelColor(408, color); 
    strip.setPixelColor(409, color); 
    strip.setPixelColor(417, color); 
    strip.setPixelColor(424, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(427, color); 
    strip.setPixelColor(428, color); 
    strip.setPixelColor(432, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(435, color); 
    strip.setPixelColor(436, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(442, color); 
    strip.setPixelColor(449, color); 
    strip.setPixelColor(450, color); 
    strip.setPixelColor(457, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(464, color); 
    strip.setPixelColor(465, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(468, color); 
    strip.setPixelColor(473, color); 
    strip.setPixelColor(474, color); 
    strip.setPixelColor(482, color); 
    strip.setPixelColor(490, color); 
    strip.setPixelColor(491, color); 
    strip.setPixelColor(498, color); 
    strip.setPixelColor(499, color); 
    strip.setPixelColor(506, color); 
    strip.setPixelColor(507, color);
    strip.show(); // Turn on the LEDs
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = true; // Face010 - this face
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face010BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face010 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(4, color); 
    strip.setPixelColor(5, color); 
    strip.setPixelColor(12, color); 
    strip.setPixelColor(13, color); 
    strip.setPixelColor(20, color); 
    strip.setPixelColor(21, color); 
    strip.setPixelColor(29, color); 
    strip.setPixelColor(37, color); 
    strip.setPixelColor(38, color); 
    strip.setPixelColor(43, color); 
    strip.setPixelColor(44, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(53, color); 
    strip.setPixelColor(54, color); 
    strip.setPixelColor(61, color); 
    strip.setPixelColor(62, color); 
    strip.setPixelColor(69, color); 
    strip.setPixelColor(70, color); 
    strip.setPixelColor(75, color); 
    strip.setPixelColor(76, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(78, color); 
    strip.setPixelColor(79, color); 
    strip.setPixelColor(83, color); 
    strip.setPixelColor(84, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(86, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(94, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(103, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(111, color); 
    strip.setPixelColor(118, color); 
    strip.setPixelColor(119, color); 
    strip.setPixelColor(128, color); 
    strip.setPixelColor(129, color); 
    strip.setPixelColor(136, color); 
    strip.setPixelColor(137, color); 
    strip.setPixelColor(145, color); 
    strip.setPixelColor(153, color); 
    strip.setPixelColor(154, color); 
    strip.setPixelColor(161, color); 
    strip.setPixelColor(162, color); 
    strip.setPixelColor(168, color); 
    strip.setPixelColor(169, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(172, color); 
    strip.setPixelColor(177, color); 
    strip.setPixelColor(178, color); 
    strip.setPixelColor(185, color); 
    strip.setPixelColor(186, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(194, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(203, color); 
    strip.setPixelColor(204, color); 
    strip.setPixelColor(208, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(212, color); 
    strip.setPixelColor(218, color); 
    strip.setPixelColor(219, color); 
    strip.setPixelColor(226, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(234, color); 
    strip.setPixelColor(235, color); 
    strip.setPixelColor(242, color); 
    strip.setPixelColor(243, color); 
    strip.setPixelColor(268, color); 
    strip.setPixelColor(269, color); 
    strip.setPixelColor(276, color); 
    strip.setPixelColor(277, color); 
    strip.setPixelColor(284, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(292, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(299, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(303, color); 
    strip.setPixelColor(307, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(310, color); 
    strip.setPixelColor(311, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(318, color); 
    strip.setPixelColor(325, color); 
    strip.setPixelColor(326, color); 
    strip.setPixelColor(333, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(339, color); 
    strip.setPixelColor(340, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(349, color); 
    strip.setPixelColor(350, color); 
    strip.setPixelColor(357, color); 
    strip.setPixelColor(358, color); 
    strip.setPixelColor(366, color); 
    strip.setPixelColor(374, color); 
    strip.setPixelColor(375, color); 
    strip.setPixelColor(382, color); 
    strip.setPixelColor(383, color); 
    strip.setPixelColor(392, color); 
    strip.setPixelColor(393, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(401, color); 
    strip.setPixelColor(408, color); 
    strip.setPixelColor(409, color); 
    strip.setPixelColor(417, color); 
    strip.setPixelColor(424, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(427, color); 
    strip.setPixelColor(428, color); 
    strip.setPixelColor(432, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(435, color); 
    strip.setPixelColor(436, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(442, color); 
    strip.setPixelColor(449, color); 
    strip.setPixelColor(450, color); 
    strip.setPixelColor(457, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(464, color); 
    strip.setPixelColor(465, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(468, color); 
    strip.setPixelColor(473, color); 
    strip.setPixelColor(474, color); 
    strip.setPixelColor(482, color); 
    strip.setPixelColor(490, color); 
    strip.setPixelColor(491, color); 
    strip.setPixelColor(498, color); 
    strip.setPixelColor(499, color); 
    strip.setPixelColor(506, color); 
    strip.setPixelColor(507, color);
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face010BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face010 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face010BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face011 ---------------------------------------------------------------------------------------------
void Face011()
{
  if (Face011RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face011");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(43, color); 
    strip.setPixelColor(44, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(51, color); 
    strip.setPixelColor(53, color); 
    strip.setPixelColor(53, color); 
    strip.setPixelColor(54, color); 
    strip.setPixelColor(55, color); 
    strip.setPixelColor(75, color); 
    strip.setPixelColor(76, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(78, color); 
    strip.setPixelColor(79, color); 
    strip.setPixelColor(83, color); 
    strip.setPixelColor(84, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(86, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(168, color); 
    strip.setPixelColor(169, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(172, color); 
    strip.setPixelColor(173, color); 
    strip.setPixelColor(176, color); 
    strip.setPixelColor(177, color); 
    strip.setPixelColor(178, color); 
    strip.setPixelColor(179, color); 
    strip.setPixelColor(180, color); 
    strip.setPixelColor(181, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(203, color); 
    strip.setPixelColor(204, color); 
    strip.setPixelColor(205, color); 
    strip.setPixelColor(208, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(212, color); 
    strip.setPixelColor(213, color); 
    strip.setPixelColor(299, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(303, color); 
    strip.setPixelColor(307, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(310, color); 
    strip.setPixelColor(311, color); 
    strip.setPixelColor(331, color); 
    strip.setPixelColor(332, color); 
    strip.setPixelColor(333, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(335, color); 
    strip.setPixelColor(339, color); 
    strip.setPixelColor(340, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(424, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(427, color); 
    strip.setPixelColor(428, color); 
    strip.setPixelColor(429, color); 
    strip.setPixelColor(432, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(435, color); 
    strip.setPixelColor(436, color); 
    strip.setPixelColor(437, color); 
    strip.setPixelColor(456, color); 
    strip.setPixelColor(457, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(459, color); 
    strip.setPixelColor(460, color); 
    strip.setPixelColor(461, color); 
    strip.setPixelColor(464, color); 
    strip.setPixelColor(465, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(468, color); 
    strip.setPixelColor(469, color);
    strip.show(); // Turn on the LEDs
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = true; // Face011 - this face
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face011BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face011 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(43, color); 
    strip.setPixelColor(44, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(51, color); 
    strip.setPixelColor(53, color); 
    strip.setPixelColor(53, color); 
    strip.setPixelColor(54, color); 
    strip.setPixelColor(55, color); 
    strip.setPixelColor(75, color); 
    strip.setPixelColor(76, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(78, color); 
    strip.setPixelColor(79, color); 
    strip.setPixelColor(83, color); 
    strip.setPixelColor(84, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(86, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(168, color); 
    strip.setPixelColor(169, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(172, color); 
    strip.setPixelColor(173, color); 
    strip.setPixelColor(176, color); 
    strip.setPixelColor(177, color); 
    strip.setPixelColor(178, color); 
    strip.setPixelColor(179, color); 
    strip.setPixelColor(180, color); 
    strip.setPixelColor(181, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(203, color); 
    strip.setPixelColor(204, color); 
    strip.setPixelColor(205, color); 
    strip.setPixelColor(208, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(212, color); 
    strip.setPixelColor(213, color); 
    strip.setPixelColor(299, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(303, color); 
    strip.setPixelColor(307, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(310, color); 
    strip.setPixelColor(311, color); 
    strip.setPixelColor(331, color); 
    strip.setPixelColor(332, color); 
    strip.setPixelColor(333, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(335, color); 
    strip.setPixelColor(339, color); 
    strip.setPixelColor(340, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(424, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(427, color); 
    strip.setPixelColor(428, color); 
    strip.setPixelColor(429, color); 
    strip.setPixelColor(432, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(435, color); 
    strip.setPixelColor(436, color); 
    strip.setPixelColor(437, color); 
    strip.setPixelColor(456, color); 
    strip.setPixelColor(457, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(459, color); 
    strip.setPixelColor(460, color); 
    strip.setPixelColor(461, color); 
    strip.setPixelColor(464, color); 
    strip.setPixelColor(465, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(468, color); 
    strip.setPixelColor(469, color);
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face011BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face011 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face011BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face012 ---------------------------------------------------------------------------------------------
void Face012()
{
  if (Face012RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face012");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(14, color); 
    strip.setPixelColor(22, color); 
    strip.setPixelColor(23, color); 
    strip.setPixelColor(31, color); 
    strip.setPixelColor(39, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(54, color); 
    strip.setPixelColor(55, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(103, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(111, color); 
    strip.setPixelColor(152, color); 
    strip.setPixelColor(160, color); 
    strip.setPixelColor(168, color); 
    strip.setPixelColor(176, color); 
    strip.setPixelColor(224, color); 
    strip.setPixelColor(232, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(279, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(287, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(335, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(350, color); 
    strip.setPixelColor(351, color); 
    strip.setPixelColor(358, color); 
    strip.setPixelColor(359, color); 
    strip.setPixelColor(367, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(408, color); 
    strip.setPixelColor(456, color); 
    strip.setPixelColor(464, color); 
    strip.setPixelColor(488, color); 
    strip.setPixelColor(496, color); 
    strip.show(); // Turn on the LEDs
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = true; // Face012 - this face
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face012BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face012 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(14, color); 
    strip.setPixelColor(22, color); 
    strip.setPixelColor(23, color); 
    strip.setPixelColor(31, color); 
    strip.setPixelColor(39, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(54, color); 
    strip.setPixelColor(55, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(103, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(111, color); 
    strip.setPixelColor(152, color); 
    strip.setPixelColor(160, color); 
    strip.setPixelColor(168, color); 
    strip.setPixelColor(176, color); 
    strip.setPixelColor(224, color); 
    strip.setPixelColor(232, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(279, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(287, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(335, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(350, color); 
    strip.setPixelColor(351, color); 
    strip.setPixelColor(358, color); 
    strip.setPixelColor(359, color); 
    strip.setPixelColor(367, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(408, color); 
    strip.setPixelColor(456, color); 
    strip.setPixelColor(464, color); 
    strip.setPixelColor(488, color); 
    strip.setPixelColor(496, color); 
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face012BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face012 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face012BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face013 ---------------------------------------------------------------------------------------------
void Face013()
{
  if (Face013RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face013");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(75, color); 
    strip.setPixelColor(79, color); 
    strip.setPixelColor(83, color); 
    strip.setPixelColor(84, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(86, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(92, color); 
    strip.setPixelColor(93, color); 
    strip.setPixelColor(94, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(194, color); 
    strip.setPixelColor(195, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(203, color); 
    strip.setPixelColor(204, color); 
    strip.setPixelColor(208, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(212, color); 
    strip.setPixelColor(220, color); 
    strip.setPixelColor(270, color); 
    strip.setPixelColor(271, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(279, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(287, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(294, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(315, color); 
    strip.setPixelColor(316, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(323, color); 
    strip.setPixelColor(324, color); 
    strip.setPixelColor(392, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(408, color); 
    strip.setPixelColor(409, color); 
    strip.setPixelColor(416, color); 
    strip.setPixelColor(417, color); 
    strip.setPixelColor(424, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(442, color); 
    strip.setPixelColor(443, color); 
    strip.setPixelColor(450, color); 
    strip.setPixelColor(451, color); 
    strip.show(); // Turn on the LEDs
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = true; // Face013 - this face
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face013BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face013 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(75, color); 
    strip.setPixelColor(79, color); 
    strip.setPixelColor(83, color); 
    strip.setPixelColor(84, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(86, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(92, color); 
    strip.setPixelColor(93, color); 
    strip.setPixelColor(94, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(194, color); 
    strip.setPixelColor(195, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(203, color); 
    strip.setPixelColor(204, color); 
    strip.setPixelColor(208, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(212, color); 
    strip.setPixelColor(220, color); 
    strip.setPixelColor(270, color); 
    strip.setPixelColor(271, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(279, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(287, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(294, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(315, color); 
    strip.setPixelColor(316, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(323, color); 
    strip.setPixelColor(324, color); 
    strip.setPixelColor(392, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(408, color); 
    strip.setPixelColor(409, color); 
    strip.setPixelColor(416, color); 
    strip.setPixelColor(417, color); 
    strip.setPixelColor(424, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(442, color); 
    strip.setPixelColor(443, color); 
    strip.setPixelColor(450, color); 
    strip.setPixelColor(451, color); 
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face013BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face013 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face013BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face014 ---------------------------------------------------------------------------------------------
void Face014()
{
  if (Face014RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face014");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(30, color); 
    strip.setPixelColor(31, color); 
    strip.setPixelColor(37, color); 
    strip.setPixelColor(38, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(62, color); 
    strip.setPixelColor(63, color); 
    strip.setPixelColor(69, color); 
    strip.setPixelColor(70, color); 
    strip.setPixelColor(71, color); 
    strip.setPixelColor(76, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(78, color); 
    strip.setPixelColor(84, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(92, color); 
    strip.setPixelColor(93, color); 
    strip.setPixelColor(100, color); 
    strip.setPixelColor(101, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(109, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(111, color); 
    strip.setPixelColor(118, color); 
    strip.setPixelColor(119, color); 
    strip.setPixelColor(144, color); 
    strip.setPixelColor(152, color); 
    strip.setPixelColor(153, color); 
    strip.setPixelColor(154, color); 
    strip.setPixelColor(161, color); 
    strip.setPixelColor(162, color); 
    strip.setPixelColor(163, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(179, color); 
    strip.setPixelColor(180, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(185, color); 
    strip.setPixelColor(187, color); 
    strip.setPixelColor(188, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(194, color); 
    strip.setPixelColor(195, color); 
    strip.setPixelColor(196, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(203, color); 
    strip.setPixelColor(204, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(212, color); 
    strip.setPixelColor(219, color); 
    strip.setPixelColor(220, color); 
    strip.setPixelColor(226, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(232, color); 
    strip.setPixelColor(233, color); 
    strip.setPixelColor(234, color); 
    strip.setPixelColor(235, color); 
    strip.setPixelColor(240, color); 
    strip.setPixelColor(241, color); 
    strip.setPixelColor(242, color); 
    strip.setPixelColor(270, color); 
    strip.setPixelColor(271, color); 
    strip.setPixelColor(277, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(279, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(292, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(310, color); 
    strip.setPixelColor(316, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(318, color); 
    strip.setPixelColor(319, color); 
    strip.setPixelColor(324, color); 
    strip.setPixelColor(325, color); 
    strip.setPixelColor(327, color); 
    strip.setPixelColor(332, color); 
    strip.setPixelColor(333, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(349, color); 
    strip.setPixelColor(350, color); 
    strip.setPixelColor(351, color); 
    strip.setPixelColor(358, color); 
    strip.setPixelColor(359, color); 
    strip.setPixelColor(392, color); 
    strip.setPixelColor(393, color); 
    strip.setPixelColor(394, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(401, color); 
    strip.setPixelColor(402, color); 
    strip.setPixelColor(403, color); 
    strip.setPixelColor(410, color); 
    strip.setPixelColor(411, color); 
    strip.setPixelColor(412, color); 
    strip.setPixelColor(419, color); 
    strip.setPixelColor(420, color); 
    strip.setPixelColor(427, color); 
    strip.setPixelColor(428, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(435, color); 
    strip.setPixelColor(436, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(442, color); 
    strip.setPixelColor(443, color); 
    strip.setPixelColor(448, color); 
    strip.setPixelColor(449, color); 
    strip.setPixelColor(450, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(474, color); 
    strip.setPixelColor(475, color); 
    strip.setPixelColor(480, color); 
    strip.setPixelColor(481, color); 
    strip.setPixelColor(482, color); 
    strip.setPixelColor(488, color);
    strip.show(); // Turn on the LEDs
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = true; // Face014 - this face
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face014BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face014 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(30, color); 
    strip.setPixelColor(31, color); 
    strip.setPixelColor(37, color); 
    strip.setPixelColor(38, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(62, color); 
    strip.setPixelColor(63, color); 
    strip.setPixelColor(69, color); 
    strip.setPixelColor(70, color); 
    strip.setPixelColor(71, color); 
    strip.setPixelColor(76, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(78, color); 
    strip.setPixelColor(84, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(92, color); 
    strip.setPixelColor(93, color); 
    strip.setPixelColor(100, color); 
    strip.setPixelColor(101, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(109, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(111, color); 
    strip.setPixelColor(118, color); 
    strip.setPixelColor(119, color); 
    strip.setPixelColor(144, color); 
    strip.setPixelColor(152, color); 
    strip.setPixelColor(153, color); 
    strip.setPixelColor(154, color); 
    strip.setPixelColor(161, color); 
    strip.setPixelColor(162, color); 
    strip.setPixelColor(163, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(179, color); 
    strip.setPixelColor(180, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(185, color); 
    strip.setPixelColor(187, color); 
    strip.setPixelColor(188, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(194, color); 
    strip.setPixelColor(195, color); 
    strip.setPixelColor(196, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(203, color); 
    strip.setPixelColor(204, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(212, color); 
    strip.setPixelColor(219, color); 
    strip.setPixelColor(220, color); 
    strip.setPixelColor(226, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(232, color); 
    strip.setPixelColor(233, color); 
    strip.setPixelColor(234, color); 
    strip.setPixelColor(235, color); 
    strip.setPixelColor(240, color); 
    strip.setPixelColor(241, color); 
    strip.setPixelColor(242, color); 
    strip.setPixelColor(270, color); 
    strip.setPixelColor(271, color); 
    strip.setPixelColor(277, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(279, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(292, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(310, color); 
    strip.setPixelColor(316, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(318, color); 
    strip.setPixelColor(319, color); 
    strip.setPixelColor(324, color); 
    strip.setPixelColor(325, color); 
    strip.setPixelColor(327, color); 
    strip.setPixelColor(332, color); 
    strip.setPixelColor(333, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(342, color); 
    strip.setPixelColor(349, color); 
    strip.setPixelColor(350, color); 
    strip.setPixelColor(351, color); 
    strip.setPixelColor(358, color); 
    strip.setPixelColor(359, color); 
    strip.setPixelColor(392, color); 
    strip.setPixelColor(393, color); 
    strip.setPixelColor(394, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(401, color); 
    strip.setPixelColor(402, color); 
    strip.setPixelColor(403, color); 
    strip.setPixelColor(410, color); 
    strip.setPixelColor(411, color); 
    strip.setPixelColor(412, color); 
    strip.setPixelColor(419, color); 
    strip.setPixelColor(420, color); 
    strip.setPixelColor(427, color); 
    strip.setPixelColor(428, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(435, color); 
    strip.setPixelColor(436, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(442, color); 
    strip.setPixelColor(443, color); 
    strip.setPixelColor(448, color); 
    strip.setPixelColor(449, color); 
    strip.setPixelColor(450, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(474, color); 
    strip.setPixelColor(475, color); 
    strip.setPixelColor(480, color); 
    strip.setPixelColor(481, color); 
    strip.setPixelColor(482, color); 
    strip.setPixelColor(488, color);
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face014BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face014 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face014BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face015 ---------------------------------------------------------------------------------------------
void Face015()
{
  if (Face015RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face015");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(14, color); 
    strip.setPixelColor(15, color); 
    strip.setPixelColor(22, color); 
    strip.setPixelColor(23, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(55, color); 
    strip.setPixelColor(62, color); 
    strip.setPixelColor(63, color); 
    strip.setPixelColor(70, color); 
    strip.setPixelColor(71, color); 
    strip.setPixelColor(78, color); 
    strip.setPixelColor(79, color); 
    strip.setPixelColor(86, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(94, color); 
    strip.setPixelColor(95, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(103, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(111, color); 
    strip.setPixelColor(118, color); 
    strip.setPixelColor(119, color); 
    strip.setPixelColor(126, color); 
    strip.setPixelColor(127, color); 
    strip.setPixelColor(136, color); 
    strip.setPixelColor(144, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(192, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(208, color); 
    strip.setPixelColor(216, color); 
    strip.setPixelColor(224, color); 
    strip.setPixelColor(232, color); 
    strip.setPixelColor(240, color); 
    strip.setPixelColor(248, color); 
    strip.setPixelColor(262, color); 
    strip.setPixelColor(263, color); 
    strip.setPixelColor(270, color); 
    strip.setPixelColor(271, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(279, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(287, color); 
    strip.setPixelColor(294, color); 
    strip.setPixelColor(295, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(303, color); 
    strip.setPixelColor(310, color); 
    strip.setPixelColor(311, color); 
    strip.setPixelColor(318, color); 
    strip.setPixelColor(319, color); 
    strip.setPixelColor(326, color); 
    strip.setPixelColor(327, color); 
    strip.setPixelColor(335, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(366, color); 
    strip.setPixelColor(367, color); 
    strip.setPixelColor(374, color); 
    strip.setPixelColor(375, color); 
    strip.setPixelColor(384, color); 
    strip.setPixelColor(392, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(408, color); 
    strip.setPixelColor(416, color); 
    strip.setPixelColor(424, color); 
    strip.setPixelColor(432, color); 
    strip.setPixelColor(440, color); 
    strip.setPixelColor(448, color); 
    strip.setPixelColor(488, color); 
    strip.setPixelColor(496, color);
    strip.show(); // Turn on the LEDs
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = true; // Face015 - this face
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face015BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face015 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(14, color); 
    strip.setPixelColor(15, color); 
    strip.setPixelColor(22, color); 
    strip.setPixelColor(23, color); 
    strip.setPixelColor(47, color); 
    strip.setPixelColor(55, color); 
    strip.setPixelColor(62, color); 
    strip.setPixelColor(63, color); 
    strip.setPixelColor(70, color); 
    strip.setPixelColor(71, color); 
    strip.setPixelColor(78, color); 
    strip.setPixelColor(79, color); 
    strip.setPixelColor(86, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(94, color); 
    strip.setPixelColor(95, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(103, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(111, color); 
    strip.setPixelColor(118, color); 
    strip.setPixelColor(119, color); 
    strip.setPixelColor(126, color); 
    strip.setPixelColor(127, color); 
    strip.setPixelColor(136, color); 
    strip.setPixelColor(144, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(192, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(208, color); 
    strip.setPixelColor(216, color); 
    strip.setPixelColor(224, color); 
    strip.setPixelColor(232, color); 
    strip.setPixelColor(240, color); 
    strip.setPixelColor(248, color); 
    strip.setPixelColor(262, color); 
    strip.setPixelColor(263, color); 
    strip.setPixelColor(270, color); 
    strip.setPixelColor(271, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(279, color); 
    strip.setPixelColor(286, color); 
    strip.setPixelColor(287, color); 
    strip.setPixelColor(294, color); 
    strip.setPixelColor(295, color); 
    strip.setPixelColor(302, color); 
    strip.setPixelColor(303, color); 
    strip.setPixelColor(310, color); 
    strip.setPixelColor(311, color); 
    strip.setPixelColor(318, color); 
    strip.setPixelColor(319, color); 
    strip.setPixelColor(326, color); 
    strip.setPixelColor(327, color); 
    strip.setPixelColor(335, color); 
    strip.setPixelColor(343, color); 
    strip.setPixelColor(366, color); 
    strip.setPixelColor(367, color); 
    strip.setPixelColor(374, color); 
    strip.setPixelColor(375, color); 
    strip.setPixelColor(384, color); 
    strip.setPixelColor(392, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(408, color); 
    strip.setPixelColor(416, color); 
    strip.setPixelColor(424, color); 
    strip.setPixelColor(432, color); 
    strip.setPixelColor(440, color); 
    strip.setPixelColor(448, color); 
    strip.setPixelColor(488, color); 
    strip.setPixelColor(496, color);
   
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face015BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face015 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face015BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face016 ---------------------------------------------------------------------------------------------
void Face016()
{
  if (Face016RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face016");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(53, color); 
    strip.setPixelColor(54, color); 
    strip.setPixelColor(55, color); 
    strip.setPixelColor(62, color); 
    strip.setPixelColor(63, color); 
    strip.setPixelColor(68, color); 
    strip.setPixelColor(69, color); 
    strip.setPixelColor(70, color); 
    strip.setPixelColor(71, color); 
    strip.setPixelColor(76, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(79, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(95, color); 
    strip.setPixelColor(168, color); 
    strip.setPixelColor(169, color); 
    strip.setPixelColor(176, color); 
    strip.setPixelColor(177, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(192, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(194, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(295, color); 
    strip.setPixelColor(303, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(311, color); 
    strip.setPixelColor(316, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(318, color); 
    strip.setPixelColor(319, color); 
    strip.setPixelColor(326, color); 
    strip.setPixelColor(327, color); 
    strip.setPixelColor(333, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(335, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(440, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(442, color); 
    strip.setPixelColor(448, color); 
    strip.setPixelColor(456, color); 
    strip.setPixelColor(457, color); 
    strip.setPixelColor(464, color); 
    strip.setPixelColor(465, color);
    strip.show(); // Turn on the LEDs
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = true; // Face016 - this face
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face016BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face016 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(45, color); 
    strip.setPixelColor(53, color); 
    strip.setPixelColor(54, color); 
    strip.setPixelColor(55, color); 
    strip.setPixelColor(62, color); 
    strip.setPixelColor(63, color); 
    strip.setPixelColor(68, color); 
    strip.setPixelColor(69, color); 
    strip.setPixelColor(70, color); 
    strip.setPixelColor(71, color); 
    strip.setPixelColor(76, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(79, color); 
    strip.setPixelColor(87, color); 
    strip.setPixelColor(95, color); 
    strip.setPixelColor(168, color); 
    strip.setPixelColor(169, color); 
    strip.setPixelColor(176, color); 
    strip.setPixelColor(177, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(192, color); 
    strip.setPixelColor(193, color); 
    strip.setPixelColor(194, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(201, color); 
    strip.setPixelColor(295, color); 
    strip.setPixelColor(303, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(311, color); 
    strip.setPixelColor(316, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(318, color); 
    strip.setPixelColor(319, color); 
    strip.setPixelColor(326, color); 
    strip.setPixelColor(327, color); 
    strip.setPixelColor(333, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(335, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(433, color); 
    strip.setPixelColor(434, color); 
    strip.setPixelColor(440, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(442, color); 
    strip.setPixelColor(448, color); 
    strip.setPixelColor(456, color); 
    strip.setPixelColor(457, color); 
    strip.setPixelColor(464, color); 
    strip.setPixelColor(465, color);
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face016BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face016 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face016BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face017 ---------------------------------------------------------------------------------------------
void Face017()
{
  if (Face017RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face017");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(103, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(216, color); 
    strip.setPixelColor(217, color); 
    strip.setPixelColor(224, color); 
    strip.setPixelColor(287, color); 
    strip.setPixelColor(408, color); 
    strip.setPixelColor(416, color); 
    strip.setPixelColor(417, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(426, color); 
    strip.show(); // Turn on the LEDs
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = true; // Face017 - this face
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face017BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face017 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(103, color); 
    strip.setPixelColor(209, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(216, color); 
    strip.setPixelColor(217, color); 
    strip.setPixelColor(224, color); 
    strip.setPixelColor(287, color); 
    strip.setPixelColor(408, color); 
    strip.setPixelColor(416, color); 
    strip.setPixelColor(417, color); 
    strip.setPixelColor(425, color); 
    strip.setPixelColor(426, color); 
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face017BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face017 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face017BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face018 ---------------------------------------------------------------------------------------------
void Face018()
{
  if (Face018RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face018");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(21, color); 
    strip.setPixelColor(22, color); 
    strip.setPixelColor(29, color); 
    strip.setPixelColor(30, color); 
    strip.setPixelColor(128, color); 
    strip.setPixelColor(129, color); 
    strip.setPixelColor(137, color); 
    strip.setPixelColor(138, color); 
    strip.setPixelColor(145, color); 
    strip.setPixelColor(146, color); 
    strip.setPixelColor(154, color); 
    strip.setPixelColor(155, color);
    strip.setPixelColor(162, color); 
    strip.setPixelColor(163, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(178, color); 
    strip.setPixelColor(179, color); 
    strip.setPixelColor(186, color); 
    strip.setPixelColor(187, color);
    strip.setPixelColor(194, color); 
    strip.setPixelColor(195, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(203, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(218, color); 
    strip.setPixelColor(219, color);
    strip.setPixelColor(226, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(233, color); 
    strip.setPixelColor(234, color); 
    strip.setPixelColor(241, color); 
    strip.setPixelColor(242, color); 
    strip.setPixelColor(248, color); 
    strip.setPixelColor(249, color);
    strip.setPixelColor(262, color); 
    strip.setPixelColor(263, color); 
    strip.setPixelColor(269, color); 
    strip.setPixelColor(270, color); 
    strip.setPixelColor(277, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(284, color); 
    strip.setPixelColor(285, color);
    strip.setPixelColor(292, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(316, color); 
    strip.setPixelColor(317, color);
    strip.setPixelColor(324, color); 
    strip.setPixelColor(325, color); 
    strip.setPixelColor(332, color); 
    strip.setPixelColor(333, color); 
    strip.setPixelColor(340, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(348, color); 
    strip.setPixelColor(349, color);
    strip.setPixelColor(356, color); 
    strip.setPixelColor(357, color); 
    strip.setPixelColor(365, color); 
    strip.setPixelColor(366, color); 
    strip.setPixelColor(373, color); 
    strip.setPixelColor(374, color); 
    strip.setPixelColor(382, color); 
    strip.setPixelColor(383, color);
    strip.setPixelColor(488, color); 
    strip.setPixelColor(489, color); 
    strip.setPixelColor(496, color); 
    strip.setPixelColor(497, color);  
    strip.show(); // Turn on the LEDs
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = true; // Face018 - this face
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face018BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face018 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(21, color); 
    strip.setPixelColor(22, color); 
    strip.setPixelColor(29, color); 
    strip.setPixelColor(30, color); 
    strip.setPixelColor(128, color); 
    strip.setPixelColor(129, color); 
    strip.setPixelColor(137, color); 
    strip.setPixelColor(138, color); 
    strip.setPixelColor(145, color); 
    strip.setPixelColor(146, color); 
    strip.setPixelColor(154, color); 
    strip.setPixelColor(155, color);
    strip.setPixelColor(162, color); 
    strip.setPixelColor(163, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(178, color); 
    strip.setPixelColor(179, color); 
    strip.setPixelColor(186, color); 
    strip.setPixelColor(187, color);
    strip.setPixelColor(194, color); 
    strip.setPixelColor(195, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(203, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(218, color); 
    strip.setPixelColor(219, color);
    strip.setPixelColor(226, color); 
    strip.setPixelColor(227, color); 
    strip.setPixelColor(233, color); 
    strip.setPixelColor(234, color); 
    strip.setPixelColor(241, color); 
    strip.setPixelColor(242, color); 
    strip.setPixelColor(248, color); 
    strip.setPixelColor(249, color);
    strip.setPixelColor(262, color); 
    strip.setPixelColor(263, color); 
    strip.setPixelColor(269, color); 
    strip.setPixelColor(270, color); 
    strip.setPixelColor(277, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(284, color); 
    strip.setPixelColor(285, color);
    strip.setPixelColor(292, color); 
    strip.setPixelColor(293, color); 
    strip.setPixelColor(300, color); 
    strip.setPixelColor(301, color); 
    strip.setPixelColor(308, color); 
    strip.setPixelColor(309, color); 
    strip.setPixelColor(316, color); 
    strip.setPixelColor(317, color);
    strip.setPixelColor(324, color); 
    strip.setPixelColor(325, color); 
    strip.setPixelColor(332, color); 
    strip.setPixelColor(333, color); 
    strip.setPixelColor(340, color); 
    strip.setPixelColor(341, color); 
    strip.setPixelColor(348, color); 
    strip.setPixelColor(349, color);
    strip.setPixelColor(356, color); 
    strip.setPixelColor(357, color); 
    strip.setPixelColor(365, color); 
    strip.setPixelColor(366, color); 
    strip.setPixelColor(373, color); 
    strip.setPixelColor(374, color); 
    strip.setPixelColor(382, color); 
    strip.setPixelColor(383, color);
    strip.setPixelColor(488, color); 
    strip.setPixelColor(489, color); 
    strip.setPixelColor(496, color); 
    strip.setPixelColor(497, color); 
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face018BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face018 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face018BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
void Face019()
{
  if (Face019RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face019");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(6, color); 
    strip.setPixelColor(7, color); 
    strip.setPixelColor(13, color); 
    strip.setPixelColor(14, color); 
    strip.setPixelColor(21, color); 
    strip.setPixelColor(22, color); 
    strip.setPixelColor(28, color); 
    strip.setPixelColor(29, color);
    strip.setPixelColor(36, color); 
    strip.setPixelColor(37, color); 
    strip.setPixelColor(44, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(52, color);
    strip.setPixelColor(53, color); 
    strip.setPixelColor(60, color); 
    strip.setPixelColor(61, color); 
    strip.setPixelColor(68, color); 
    strip.setPixelColor(69, color); 
    strip.setPixelColor(76, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(84, color);
    strip.setPixelColor(85, color); 
    strip.setPixelColor(92, color); 
    strip.setPixelColor(93, color); 
    strip.setPixelColor(100, color); 
    strip.setPixelColor(101, color); 
    strip.setPixelColor(109, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(117, color);
    strip.setPixelColor(118, color); 
    strip.setPixelColor(126, color); 
    strip.setPixelColor(127, color); 
    strip.setPixelColor(145, color); 
    strip.setPixelColor(146, color); 
    strip.setPixelColor(153, color); 
    strip.setPixelColor(154, color); 
    strip.setPixelColor(357, color);
    strip.setPixelColor(358, color); 
    strip.setPixelColor(365, color); 
    strip.setPixelColor(366, color); 
    strip.setPixelColor(384, color); 
    strip.setPixelColor(385, color); 
    strip.setPixelColor(393, color); 
    strip.setPixelColor(394, color); 
    strip.setPixelColor(401, color);
    strip.setPixelColor(402, color); 
    strip.setPixelColor(410, color); 
    strip.setPixelColor(411, color); 
    strip.setPixelColor(418, color); 
    strip.setPixelColor(419, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(427, color);
    strip.setPixelColor(434, color); 
    strip.setPixelColor(435, color);
    strip.setPixelColor(442, color); 
    strip.setPixelColor(443, color); 
    strip.setPixelColor(450, color); 
    strip.setPixelColor(451, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(459, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color);
    strip.setPixelColor(474, color); 
    strip.setPixelColor(475, color); 
    strip.setPixelColor(482, color); 
    strip.setPixelColor(483, color); 
    strip.setPixelColor(489, color); 
    strip.setPixelColor(490, color); 
    strip.setPixelColor(497, color); 
    strip.setPixelColor(498, color);
    strip.setPixelColor(504, color); 
    strip.setPixelColor(505, color);  
    strip.show(); // Turn on the LEDs
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = true; // Face019 - this face
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face019BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face019 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(6, color); 
    strip.setPixelColor(7, color); 
    strip.setPixelColor(13, color); 
    strip.setPixelColor(14, color); 
    strip.setPixelColor(21, color); 
    strip.setPixelColor(22, color); 
    strip.setPixelColor(28, color); 
    strip.setPixelColor(29, color);
    strip.setPixelColor(36, color); 
    strip.setPixelColor(37, color); 
    strip.setPixelColor(44, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(52, color);
    strip.setPixelColor(53, color); 
    strip.setPixelColor(60, color); 
    strip.setPixelColor(61, color); 
    strip.setPixelColor(68, color); 
    strip.setPixelColor(69, color); 
    strip.setPixelColor(76, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(84, color);
    strip.setPixelColor(85, color); 
    strip.setPixelColor(92, color); 
    strip.setPixelColor(93, color); 
    strip.setPixelColor(100, color); 
    strip.setPixelColor(101, color); 
    strip.setPixelColor(109, color); 
    strip.setPixelColor(110, color); 
    strip.setPixelColor(117, color);
    strip.setPixelColor(118, color); 
    strip.setPixelColor(126, color); 
    strip.setPixelColor(127, color); 
    strip.setPixelColor(145, color); 
    strip.setPixelColor(146, color); 
    strip.setPixelColor(153, color); 
    strip.setPixelColor(154, color); 
    strip.setPixelColor(357, color);
    strip.setPixelColor(358, color); 
    strip.setPixelColor(365, color); 
    strip.setPixelColor(366, color); 
    strip.setPixelColor(384, color); 
    strip.setPixelColor(385, color); 
    strip.setPixelColor(393, color); 
    strip.setPixelColor(394, color); 
    strip.setPixelColor(401, color);
    strip.setPixelColor(402, color); 
    strip.setPixelColor(410, color); 
    strip.setPixelColor(411, color); 
    strip.setPixelColor(418, color); 
    strip.setPixelColor(419, color); 
    strip.setPixelColor(426, color); 
    strip.setPixelColor(427, color);
    strip.setPixelColor(434, color); 
    strip.setPixelColor(435, color);
    strip.setPixelColor(442, color); 
    strip.setPixelColor(443, color); 
    strip.setPixelColor(450, color); 
    strip.setPixelColor(451, color); 
    strip.setPixelColor(458, color); 
    strip.setPixelColor(459, color); 
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color);
    strip.setPixelColor(474, color); 
    strip.setPixelColor(475, color); 
    strip.setPixelColor(482, color); 
    strip.setPixelColor(483, color); 
    strip.setPixelColor(489, color); 
    strip.setPixelColor(490, color); 
    strip.setPixelColor(497, color); 
    strip.setPixelColor(498, color);
    strip.setPixelColor(504, color); 
    strip.setPixelColor(505, color);
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face019BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face019 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face019BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face020 ---------------------------------------------------------------------------------------------
void Face020()
{
  if (Face020RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face020");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(31, color); 
    strip.setPixelColor(37, color); 
    strip.setPixelColor(38, color); 
    strip.setPixelColor(39, color); 
    strip.setPixelColor(44, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(52, color); 
    strip.setPixelColor(53, color); 
    strip.setPixelColor(60, color); 
    strip.setPixelColor(61, color); 
    strip.setPixelColor(68, color); 
    strip.setPixelColor(69, color); 
    strip.setPixelColor(76, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(84, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(93, color); 
    strip.setPixelColor(94, color); 
    strip.setPixelColor(95, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(103, color); 
    strip.setPixelColor(152, color); 
    strip.setPixelColor(160, color); 
    strip.setPixelColor(161, color); 
    strip.setPixelColor(162, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(178, color); 
    strip.setPixelColor(179, color); 
    strip.setPixelColor(187, color); 
    strip.setPixelColor(188, color); 
    strip.setPixelColor(195, color); 
    strip.setPixelColor(196, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(203, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(216, color); 
    strip.setPixelColor(217, color); 
    strip.setPixelColor(218, color); 
    strip.setPixelColor(224, color); 
    strip.setPixelColor(225, color); 
    strip.setPixelColor(270, color); 
    strip.setPixelColor(271, color); 
    strip.setPixelColor(276, color); 
    strip.setPixelColor(277, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(279, color); 
    strip.setPixelColor(283, color); 
    strip.setPixelColor(284, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(291, color); 
    strip.setPixelColor(292, color); 
    strip.setPixelColor(298, color); 
    strip.setPixelColor(299, color); 
    strip.setPixelColor(306, color); 
    strip.setPixelColor(307, color); 
    strip.setPixelColor(314, color); 
    strip.setPixelColor(315, color); 
    strip.setPixelColor(322, color); 
    strip.setPixelColor(323, color); 
    strip.setPixelColor(330, color); 
    strip.setPixelColor(331, color); 
    strip.setPixelColor(338, color); 
    strip.setPixelColor(339, color); 
    strip.setPixelColor(347, color); 
    strip.setPixelColor(348, color); 
    strip.setPixelColor(356, color); 
    strip.setPixelColor(357, color); 
    strip.setPixelColor(358, color); 
    strip.setPixelColor(359, color); 
    strip.setPixelColor(365, color); 
    strip.setPixelColor(366, color); 
    strip.setPixelColor(367, color); 
    strip.setPixelColor(392, color); 
    strip.setPixelColor(393, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(401, color); 
    strip.setPixelColor(402, color); 
    strip.setPixelColor(403, color); 
    strip.setPixelColor(410, color); 
    strip.setPixelColor(411, color); 
    strip.setPixelColor(412, color); 
    strip.setPixelColor(419, color); 
    strip.setPixelColor(420, color); 
    strip.setPixelColor(427, color); 
    strip.setPixelColor(428, color); 
    strip.setPixelColor(429, color); 
    strip.setPixelColor(436, color); 
    strip.setPixelColor(437, color); 
    strip.setPixelColor(444, color); 
    strip.setPixelColor(445, color); 
    strip.setPixelColor(452, color); 
    strip.setPixelColor(453, color); 
    strip.setPixelColor(460, color); 
    strip.setPixelColor(461, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(468, color); 
    strip.setPixelColor(474, color); 
    strip.setPixelColor(475, color); 
    strip.setPixelColor(476, color); 
    strip.setPixelColor(480, color); 
    strip.setPixelColor(481, color); 
    strip.setPixelColor(482, color); 
    strip.setPixelColor(483, color); 
    strip.setPixelColor(488, color); 
    strip.setPixelColor(489, color);
    strip.show(); // Turn on the LEDs 
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = true; // Face020 - this face
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face020BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face020 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(31, color); 
    strip.setPixelColor(37, color); 
    strip.setPixelColor(38, color); 
    strip.setPixelColor(39, color); 
    strip.setPixelColor(44, color); 
    strip.setPixelColor(45, color); 
    strip.setPixelColor(46, color); 
    strip.setPixelColor(52, color); 
    strip.setPixelColor(53, color); 
    strip.setPixelColor(60, color); 
    strip.setPixelColor(61, color); 
    strip.setPixelColor(68, color); 
    strip.setPixelColor(69, color); 
    strip.setPixelColor(76, color); 
    strip.setPixelColor(77, color); 
    strip.setPixelColor(84, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(93, color); 
    strip.setPixelColor(94, color); 
    strip.setPixelColor(95, color); 
    strip.setPixelColor(102, color); 
    strip.setPixelColor(103, color); 
    strip.setPixelColor(152, color); 
    strip.setPixelColor(160, color); 
    strip.setPixelColor(161, color); 
    strip.setPixelColor(162, color); 
    strip.setPixelColor(170, color); 
    strip.setPixelColor(171, color); 
    strip.setPixelColor(178, color); 
    strip.setPixelColor(179, color); 
    strip.setPixelColor(187, color); 
    strip.setPixelColor(188, color); 
    strip.setPixelColor(195, color); 
    strip.setPixelColor(196, color); 
    strip.setPixelColor(202, color); 
    strip.setPixelColor(203, color); 
    strip.setPixelColor(210, color); 
    strip.setPixelColor(211, color); 
    strip.setPixelColor(216, color); 
    strip.setPixelColor(217, color); 
    strip.setPixelColor(218, color); 
    strip.setPixelColor(224, color); 
    strip.setPixelColor(225, color); 
    strip.setPixelColor(270, color); 
    strip.setPixelColor(271, color); 
    strip.setPixelColor(276, color); 
    strip.setPixelColor(277, color); 
    strip.setPixelColor(278, color); 
    strip.setPixelColor(279, color); 
    strip.setPixelColor(283, color); 
    strip.setPixelColor(284, color); 
    strip.setPixelColor(285, color); 
    strip.setPixelColor(291, color); 
    strip.setPixelColor(292, color); 
    strip.setPixelColor(298, color); 
    strip.setPixelColor(299, color); 
    strip.setPixelColor(306, color); 
    strip.setPixelColor(307, color); 
    strip.setPixelColor(314, color); 
    strip.setPixelColor(315, color); 
    strip.setPixelColor(322, color); 
    strip.setPixelColor(323, color); 
    strip.setPixelColor(330, color); 
    strip.setPixelColor(331, color); 
    strip.setPixelColor(338, color); 
    strip.setPixelColor(339, color); 
    strip.setPixelColor(347, color); 
    strip.setPixelColor(348, color); 
    strip.setPixelColor(356, color); 
    strip.setPixelColor(357, color); 
    strip.setPixelColor(358, color); 
    strip.setPixelColor(359, color); 
    strip.setPixelColor(365, color); 
    strip.setPixelColor(366, color); 
    strip.setPixelColor(367, color); 
    strip.setPixelColor(392, color); 
    strip.setPixelColor(393, color); 
    strip.setPixelColor(400, color); 
    strip.setPixelColor(401, color); 
    strip.setPixelColor(402, color); 
    strip.setPixelColor(403, color); 
    strip.setPixelColor(410, color); 
    strip.setPixelColor(411, color); 
    strip.setPixelColor(412, color); 
    strip.setPixelColor(419, color); 
    strip.setPixelColor(420, color); 
    strip.setPixelColor(427, color); 
    strip.setPixelColor(428, color); 
    strip.setPixelColor(429, color); 
    strip.setPixelColor(436, color); 
    strip.setPixelColor(437, color); 
    strip.setPixelColor(444, color); 
    strip.setPixelColor(445, color); 
    strip.setPixelColor(452, color); 
    strip.setPixelColor(453, color); 
    strip.setPixelColor(460, color); 
    strip.setPixelColor(461, color); 
    strip.setPixelColor(467, color); 
    strip.setPixelColor(468, color); 
    strip.setPixelColor(474, color); 
    strip.setPixelColor(475, color); 
    strip.setPixelColor(476, color); 
    strip.setPixelColor(480, color); 
    strip.setPixelColor(481, color); 
    strip.setPixelColor(482, color); 
    strip.setPixelColor(483, color); 
    strip.setPixelColor(488, color); 
    strip.setPixelColor(489, color); 
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face020BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face020 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face020BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face021 ---------------------------------------------------------------------------------------------
void Face021()
{
  if (Face021RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face021");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(54, color);
    strip.setPixelColor(55, color); 
    strip.setPixelColor(62, color); 
    strip.setPixelColor(63, color);
    strip.setPixelColor(70, color);
    strip.setPixelColor(71, color); 
    strip.setPixelColor(78, color); 
    strip.setPixelColor(79, color); 
    strip.setPixelColor(176, color);
    strip.setPixelColor(177, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(185, color); 
    strip.setPixelColor(192, color);
    strip.setPixelColor(193, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(201, color);
    strip.setPixelColor(310, color);
    strip.setPixelColor(311, color); 
    strip.setPixelColor(318, color); 
    strip.setPixelColor(319, color);
    strip.setPixelColor(326, color);
    strip.setPixelColor(327, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(335, color); 
    strip.setPixelColor(432, color);
    strip.setPixelColor(433, color); 
    strip.setPixelColor(440, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(448, color);
    strip.setPixelColor(449, color); 
    strip.setPixelColor(456, color); 
    strip.setPixelColor(457, color);
    strip.show(); // Turn on the LEDs 
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = true; // Face021 - thise face
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face021BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face021 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(54, color);
    strip.setPixelColor(55, color); 
    strip.setPixelColor(62, color); 
    strip.setPixelColor(63, color);
    strip.setPixelColor(70, color);
    strip.setPixelColor(71, color); 
    strip.setPixelColor(78, color); 
    strip.setPixelColor(79, color); 
    strip.setPixelColor(176, color);
    strip.setPixelColor(177, color); 
    strip.setPixelColor(184, color); 
    strip.setPixelColor(185, color); 
    strip.setPixelColor(192, color);
    strip.setPixelColor(193, color); 
    strip.setPixelColor(200, color); 
    strip.setPixelColor(201, color);
    strip.setPixelColor(310, color);
    strip.setPixelColor(311, color); 
    strip.setPixelColor(318, color); 
    strip.setPixelColor(319, color);
    strip.setPixelColor(326, color);
    strip.setPixelColor(327, color); 
    strip.setPixelColor(334, color); 
    strip.setPixelColor(335, color); 
    strip.setPixelColor(432, color);
    strip.setPixelColor(433, color); 
    strip.setPixelColor(440, color); 
    strip.setPixelColor(441, color); 
    strip.setPixelColor(448, color);
    strip.setPixelColor(449, color); 
    strip.setPixelColor(456, color); 
    strip.setPixelColor(457, color);   
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face021BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face021 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face021BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face022 ---------------------------------------------------------------------------------------------
void Face022()
{
  if (Face022RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face022");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(44, color);
    strip.setPixelColor(45, color); 
    strip.setPixelColor(52, color); 
    strip.setPixelColor(53, color);
    strip.setPixelColor(60, color);
    strip.setPixelColor(61, color); 
    strip.setPixelColor(62, color); 
    strip.setPixelColor(63, color); 
    strip.setPixelColor(68, color);
    strip.setPixelColor(69, color); 
    strip.setPixelColor(70, color); 
    strip.setPixelColor(71, color); 
    strip.setPixelColor(192, color);
    strip.setPixelColor(193, color); 
    strip.setPixelColor(194, color); 
    strip.setPixelColor(195, color);
    strip.setPixelColor(184, color);
    strip.setPixelColor(185, color); 
    strip.setPixelColor(186, color); 
    strip.setPixelColor(187, color);
    strip.setPixelColor(315, color);
    strip.setPixelColor(316, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(318, color); 
    strip.setPixelColor(319, color);
    strip.setPixelColor(323, color); 
    strip.setPixelColor(324, color); 
    strip.setPixelColor(325, color); 
    strip.setPixelColor(326, color);
    strip.setPixelColor(327, color); 
    strip.setPixelColor(440, color); 
    strip.setPixelColor(441, color);
    strip.setPixelColor(442, color); 
    strip.setPixelColor(443, color);
    strip.setPixelColor(448, color); 
    strip.setPixelColor(449, color);
    strip.setPixelColor(450, color); 
    strip.setPixelColor(451, color);
    strip.setPixelColor(458, color); 
    strip.setPixelColor(459, color);
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color); 
    strip.show(); // Turn on the LEDs 
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = true; // Face022 - this face
    Face023RunningCheck = false; // Face023
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face022BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face022 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(44, color);
    strip.setPixelColor(45, color); 
    strip.setPixelColor(52, color); 
    strip.setPixelColor(53, color);
    strip.setPixelColor(60, color);
    strip.setPixelColor(61, color); 
    strip.setPixelColor(62, color); 
    strip.setPixelColor(63, color); 
    strip.setPixelColor(68, color);
    strip.setPixelColor(69, color); 
    strip.setPixelColor(70, color); 
    strip.setPixelColor(71, color); 
    strip.setPixelColor(192, color);
    strip.setPixelColor(193, color); 
    strip.setPixelColor(194, color); 
    strip.setPixelColor(195, color);
    strip.setPixelColor(184, color);
    strip.setPixelColor(185, color); 
    strip.setPixelColor(186, color); 
    strip.setPixelColor(187, color);
    strip.setPixelColor(315, color);
    strip.setPixelColor(316, color); 
    strip.setPixelColor(317, color); 
    strip.setPixelColor(318, color); 
    strip.setPixelColor(319, color);
    strip.setPixelColor(323, color); 
    strip.setPixelColor(324, color); 
    strip.setPixelColor(325, color); 
    strip.setPixelColor(326, color);
    strip.setPixelColor(327, color); 
    strip.setPixelColor(440, color); 
    strip.setPixelColor(441, color);
    strip.setPixelColor(442, color); 
    strip.setPixelColor(443, color);
    strip.setPixelColor(448, color); 
    strip.setPixelColor(449, color);
    strip.setPixelColor(450, color); 
    strip.setPixelColor(451, color);
    strip.setPixelColor(458, color); 
    strip.setPixelColor(459, color);
    strip.setPixelColor(466, color); 
    strip.setPixelColor(467, color);   
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face022BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face022 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face022BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face023 ---------------------------------------------------------------------------------------------
void Face023()
{
  if (Face022RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
     // Debug output
    Serial.println("Switching to Face023");
    Serial.println("Blink Paused");
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face 
    strip.setPixelColor(38, color);
    strip.setPixelColor(39, color); 
    strip.setPixelColor(44, color); 
    strip.setPixelColor(45, color);
    strip.setPixelColor(46, color);
    strip.setPixelColor(50, color); 
    strip.setPixelColor(51, color); 
    strip.setPixelColor(52, color); 
    strip.setPixelColor(56, color);
    strip.setPixelColor(57, color); 
    strip.setPixelColor(58, color); 
    strip.setPixelColor(64, color); 
    strip.setPixelColor(65, color);
    strip.setPixelColor(66, color); 
    strip.setPixelColor(74, color); 
    strip.setPixelColor(75, color);
    strip.setPixelColor(76, color);
    strip.setPixelColor(84, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(86, color);
    strip.setPixelColor(94, color);
    strip.setPixelColor(95, color); 
    strip.setPixelColor(155, color); 
    strip.setPixelColor(156, color); 
    strip.setPixelColor(157, color);
    strip.setPixelColor(160, color); 
    strip.setPixelColor(162, color); 
    strip.setPixelColor(166, color); 
    strip.setPixelColor(174, color);
    strip.setPixelColor(182, color); 
    strip.setPixelColor(189, color); 
    strip.setPixelColor(197, color);
    strip.setPixelColor(206, color); 
    strip.setPixelColor(214, color);
    strip.setPixelColor(216, color); 
    strip.setPixelColor(218, color);
    strip.setPixelColor(222, color); 
    strip.setPixelColor(227, color);
    strip.setPixelColor(228, color); 
    strip.setPixelColor(229, color);
    strip.setPixelColor(282, color); 
    strip.setPixelColor(283, color);
    strip.setPixelColor(284, color);
    strip.setPixelColor(289, color);
    strip.setPixelColor(293, color);
    strip.setPixelColor(295, color);
    strip.setPixelColor(297, color);
    strip.setPixelColor(305, color);
    strip.setPixelColor(314, color);
    strip.setPixelColor(322, color);
    strip.setPixelColor(329, color);
    strip.setPixelColor(337, color);
    strip.setPixelColor(345, color);
    strip.setPixelColor(349, color);
    strip.setPixelColor(351, color);
    strip.setPixelColor(354, color);
    strip.setPixelColor(355, color);
    strip.setPixelColor(356, color);
    strip.setPixelColor(416, color);
    strip.setPixelColor(417, color);
    strip.setPixelColor(425, color);
    strip.setPixelColor(426, color);
    strip.setPixelColor(427, color);
    strip.setPixelColor(435, color);
    strip.setPixelColor(436, color);
    strip.setPixelColor(437, color);
    strip.setPixelColor(445, color);
    strip.setPixelColor(446, color);
    strip.setPixelColor(447, color);
    strip.setPixelColor(453, color);
    strip.setPixelColor(454, color);
    strip.setPixelColor(455, color);
    strip.setPixelColor(459, color);
    strip.setPixelColor(460, color);
    strip.setPixelColor(461, color);
    strip.setPixelColor(465, color);
    strip.setPixelColor(466, color);
    strip.setPixelColor(467, color);
    strip.setPixelColor(472, color);
    strip.setPixelColor(473, color);  
    strip.show(); // Turn on the LEDs 
    // Setting FaceRunningCheck variables for smoother changes
    Face001RunningCheck = false; // Face001
    Face002RunningCheck = false; // Face002
    Face003RunningCheck = false; // Face003
    Face004RunningCheck = false; // Face004
    Face005RunningCheck = false; // Face005
    Face006RunningCheck = false; // Face006 
    Face007RunningCheck = false; // Face007
    Face008RunningCheck = false; // Face008
    Face009RunningCheck = false; // Face009
    Face010RunningCheck = false; // Face010
    Face011RunningCheck = false; // Face011
    Face012RunningCheck = false; // Face012
    Face013RunningCheck = false; // Face013
    Face014RunningCheck = false; // Face014
    Face015RunningCheck = false; // Face015
    Face016RunningCheck = false; // Face016
    Face017RunningCheck = false; // Face017
    Face018RunningCheck = false; // Face018
    Face019RunningCheck = false; // Face019
    Face020RunningCheck = false; // Face020
    Face021RunningCheck = false; // Face021
    Face022RunningCheck = false; // Face022
    Face023RunningCheck = true; // Face023 - this face
    // Clearing BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    Face023BlinkCheck = false;
  }
  currentMillis = millis(); // Set variable to see if it's time to change the state of the LEDs
  if ((ledState == 0) && (currentMillis - previousMillis >= OffTime)) // Turn on the LEDs 
  {
    ledState = 1;  // Set the LEDs status to on
    previousMillis = currentMillis; // Remember the time
    Serial.println("Face023 LEDs ON"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    // Set LEDs for this face  
    strip.setPixelColor(38, color);
    strip.setPixelColor(39, color); 
    strip.setPixelColor(44, color); 
    strip.setPixelColor(45, color);
    strip.setPixelColor(46, color);
    strip.setPixelColor(50, color); 
    strip.setPixelColor(51, color); 
    strip.setPixelColor(52, color); 
    strip.setPixelColor(56, color);
    strip.setPixelColor(57, color); 
    strip.setPixelColor(58, color); 
    strip.setPixelColor(64, color); 
    strip.setPixelColor(65, color);
    strip.setPixelColor(66, color); 
    strip.setPixelColor(74, color); 
    strip.setPixelColor(75, color);
    strip.setPixelColor(76, color);
    strip.setPixelColor(84, color); 
    strip.setPixelColor(85, color); 
    strip.setPixelColor(86, color);
    strip.setPixelColor(94, color);
    strip.setPixelColor(95, color); 
    strip.setPixelColor(155, color); 
    strip.setPixelColor(156, color); 
    strip.setPixelColor(157, color);
    strip.setPixelColor(160, color); 
    strip.setPixelColor(162, color); 
    strip.setPixelColor(166, color); 
    strip.setPixelColor(174, color);
    strip.setPixelColor(182, color); 
    strip.setPixelColor(189, color); 
    strip.setPixelColor(197, color);
    strip.setPixelColor(206, color); 
    strip.setPixelColor(214, color);
    strip.setPixelColor(216, color); 
    strip.setPixelColor(218, color);
    strip.setPixelColor(222, color); 
    strip.setPixelColor(227, color);
    strip.setPixelColor(228, color); 
    strip.setPixelColor(229, color);
    strip.setPixelColor(282, color); 
    strip.setPixelColor(283, color);
    strip.setPixelColor(284, color);
    strip.setPixelColor(289, color);
    strip.setPixelColor(293, color);
    strip.setPixelColor(295, color);
    strip.setPixelColor(297, color);
    strip.setPixelColor(305, color);
    strip.setPixelColor(314, color);
    strip.setPixelColor(322, color);
    strip.setPixelColor(329, color);
    strip.setPixelColor(337, color);
    strip.setPixelColor(345, color);
    strip.setPixelColor(349, color);
    strip.setPixelColor(351, color);
    strip.setPixelColor(354, color);
    strip.setPixelColor(355, color);
    strip.setPixelColor(356, color);
    strip.setPixelColor(416, color);
    strip.setPixelColor(417, color);
    strip.setPixelColor(425, color);
    strip.setPixelColor(426, color);
    strip.setPixelColor(427, color);
    strip.setPixelColor(435, color);
    strip.setPixelColor(436, color);
    strip.setPixelColor(437, color);
    strip.setPixelColor(445, color);
    strip.setPixelColor(446, color);
    strip.setPixelColor(447, color);
    strip.setPixelColor(453, color);
    strip.setPixelColor(454, color);
    strip.setPixelColor(455, color);
    strip.setPixelColor(459, color);
    strip.setPixelColor(460, color);
    strip.setPixelColor(461, color);
    strip.setPixelColor(465, color);
    strip.setPixelColor(466, color);
    strip.setPixelColor(467, color);
    strip.setPixelColor(472, color);
    strip.setPixelColor(473, color);   
    strip.show(); // Turn on the LEDs
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face023BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face022 LEDs OFF - Blink"); // Debug output
    strip.clear(); // Clear previously configured LEDs
    strip.show(); // Turn off the LEDs 
    }
    else
    {
    Face023BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
