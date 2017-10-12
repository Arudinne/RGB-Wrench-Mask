/*  Wrench Mask - BLE Version (NRF52832)
 *  
 *  Hardware Used:
 *  Adafruit Feather NRF52
 *  Adafruit DotStar High Density 8x8 Grid - 64 RGB LED Pixel Matrix
 *  TXB0104 Bi-Directional Level Shifter
 *  
 *  by Brandon C. Allen 
 *  
 *  Based on the bleuart example from AdaFruit:
 *  https://github.com/adafruit/Adafruit_nRF52_Arduino/blob/master/libraries/Bluefruit52Lib/examples/Peripheral/bleuart/bleuart.ino
 *  
*/

/* Facial Expressions List
 *  X X - Face001
 *  > < - Face002
 *  ^ ^ - Face003
 *  @ @ - Face004
 *  / \ - Face005
 *  ? ? - Face006
 *  O o - Face007
 *  \ / - Face008
 *  Z Z - Face009
 *  # # - Face010
 *  = = - Face011
 *  ~ ^ - Face012
 *  9 9 - Face013
 *  ! ! - Face014
 *  * * - Face015
 *  ` ` - Face016
 *  ; ; - Face017
 *  .) .) - Face018
 *  (. (. - Face019
 *  o O - Face020
 *  . . - Face021
 *  ⌐ ⌐ - Face022
 *  <3 <3 - Face023
*/
#include <bluefruit.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>

// BLE Service
BLEDis  bledis;
BLEUart bleuart;
BLEBas  blebas;

#define NUMPIXELS 256

#define DATAPINL    13
#define CLOCKPINL   12

#define DATAPINR    30
#define CLOCKPINR   27

// Build the strip
Adafruit_DotStar lefteye = Adafruit_DotStar(NUMPIXELS, DATAPINL, CLOCKPINL, DOTSTAR_BGR);
Adafruit_DotStar righteye = Adafruit_DotStar(NUMPIXELS, DATAPINR, CLOCKPINR, DOTSTAR_BGR);

// Define initial facial expression
char FaceMode = 1;  

// Define color parameters
uint32_t colorRed = 0x010000;
uint32_t colorGreen = 0x000100;
uint32_t colorBlue = 0x000001;
uint32_t colorWhite = 0x010101;

// Define initial color
uint32_t color = colorWhite;

// Define Variables for blinking the LEDs
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
int ledState = 0; //
long OnTime = 6000; // milliseconds of on-time
long OffTime = 50; // milliseconds of off-time

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

void setup()
{
  lefteye.begin();
  righteye.begin();
  lefteye.show(); 
  righteye.show();
  
  Serial.begin(115200); // Enable Serial for debugging purposes
  Serial.println("=======================================");
  Serial.println("|| Opening Serial port for Debugging ||");
  Serial.println("||     Begin Wrnech Mask Program     ||");
  Serial.println("=======================================");

  //Configure BLE
  Bluefruit.begin();
  // Set max power. Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(4);
  Bluefruit.setName("Wrench Mask");
  Bluefruit.setConnectCallback(connect_callback);
  Bluefruit.setDisconnectCallback(disconnect_callback);

  // Configure and Start Device Information Service
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();

  // Configure and Start BLE Uart Service
  bleuart.begin();

  // Start BLE Battery Service
  blebas.begin();
  blebas.write(100);

  // Set up and start advertising
  startAdv();
}

void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
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
    if(ch == 0x57){
      color=colorRed;
    }
    else if(ch == 0x58){
      color=colorGreen;
    } 
    else if(ch == 0x59){
      color=colorBlue;
    } 
    else if(ch == 0x5A){
      color=colorWhite;
    } 
    else if(ch == 0x31){
      FaceMode=1;
    }
    else if(ch == 0x32){
      FaceMode=2;
    }
    else if(ch == 0x33){
      FaceMode=3;
    }
    else if(ch == 0x34){
      FaceMode=4;
    }
    else if(ch == 0x35){
      FaceMode=5;
    }
    else if(ch == 0x36){
      FaceMode=6;
    }
    else if(ch == 0x37){
      FaceMode=7;
    }
    else if(ch == 0x38){
      FaceMode=8;
    }
    else if(ch == 0x39){
      FaceMode=9;
    }
    else if(ch == 0x30){
      FaceMode=0;
    }
    else if(ch == 0x41){
      FaceMode='A';
    }
    else if(ch == 0x42){
      FaceMode='B';
    }
    else if(ch == 0x43){
      FaceMode='C';
    }
    else if(ch == 0x44){
      FaceMode='D';
    }
    else if(ch == 0x45){
      FaceMode='E';
    }
    else if(ch == 0x46){
      FaceMode='F';
    }
    else if(ch == 0x47){
      FaceMode='G';
    }
    else if(ch == 0x48){
      FaceMode='H';
    }
    else if(ch == 0x49){
      FaceMode='I';
    }
    else if(ch == 0x4A){
      FaceMode='J';
    }
    else if(ch == 0x4B){
      FaceMode='K';
    }
    else if(ch == 0x4C){
      FaceMode='L';
    }
    else if(ch == 0x4D){
      FaceMode='M';
    }
    else if(ch == 0x4E){
      FaceMode='N';
    } 
    Serial.write(ch);
  }

  switch (FaceMode) 
   {
    case 1: Face001(); break; // Draw Face001
    case 2: Face002(); break; // Draw Face002
    case 3: Face003(); break; // Draw Face003
    case 4: Face004(); break; // Draw Face004
    case 5: Face005(); break; // Draw Face005
    case 6: Face006(); break; // Draw Face006
    case 7: Face007(); break; // Draw Face007
    case 8: Face008(); break; // Draw Face008
    case 9: Face009(); break; // Draw Face009
    case 0: Face010(); break; // Draw Face010
    case 'A': Face011(); break; // Draw Face011
    case 'B': Face012(); break; // Draw Face012
    case 'C': Face013(); break; // Draw Face013
    case 'D': Face014(); break; // Draw Face014
    case 'E': Face015(); break; // Draw Face015
    case 'F': Face016(); break; // Draw Face016
    case 'G': Face017(); break; // Draw Face017
    case 'H': Face018(); break; // Draw Face018
    case 'I': Face019(); break; // Draw Face019
    case 'J': Face020(); break; // Draw Face020
    case 'K': Face021(); break; // Draw Face021
    case 'L': Face022(); break; // Draw Face022
    case 'M': Face023(); break; // Draw Face023
    case 'N': Umbrella(); break; // Draw Umbrella Corp logo
   }
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

/**
 * RTOS Idle callback is automatically invoked by FreeRTOS
 * when there are no active threads. E.g when loop() calls delay() and
 * there is no bluetooth or hw event. This is the ideal place to handle
 * background data.
 * 
 * NOTE: It is recommended to call waitForEvent() to put MCU into low-power mode
 * at the end of this callback. You could also turn off other Peripherals such as
 * Serial/PWM and turn them back on if wanted
 * 
 * e.g
 * 
 * void rtos_idle_callback(void)
 * {
 *    Serial.stop(); // will lose data when sleeping
 *    waitForEvent();
 *    Serial.begin(115200); 
 * }
 * 
 * NOTE2: If rtos_idle_callback() is not defined at all. Bluefruit will force
 * waitForEvent() to save power. If you don't want MCU to sleep at all, define
 * an rtos_idle_callback() with empty body !
 * 
 * WARNING: This function MUST NOT call any blocking FreeRTOS API 
 * such as delay(), xSemaphoreTake() etc ... for more information
 * http://www.freertos.org/a00016.html
 */
void rtos_idle_callback(void)
{
  // Don't call any other FreeRTOS blocking API()
  // Perform background task(s) here

  // Request CPU to enter low-power mode until an event/interrupt occurs
  waitForEvent();
}

void Face001()
{
  if (Face001RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(6, color);
    lefteye.setPixelColor(7, color);
    lefteye.setPixelColor(13, color);
    lefteye.setPixelColor(14, color);
    lefteye.setPixelColor(15, color);
    lefteye.setPixelColor(20, color);
    lefteye.setPixelColor(21, color);
    lefteye.setPixelColor(22, color);
    lefteye.setPixelColor(27, color);
    lefteye.setPixelColor(28, color);
    lefteye.setPixelColor(29, color);
    lefteye.setPixelColor(34, color);
    lefteye.setPixelColor(35, color);
    lefteye.setPixelColor(36, color);
    lefteye.setPixelColor(41, color);
    lefteye.setPixelColor(42, color);
    lefteye.setPixelColor(43, color);
    lefteye.setPixelColor(48, color);
    lefteye.setPixelColor(49, color);
    lefteye.setPixelColor(50, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(57, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(65, color);
    lefteye.setPixelColor(72, color);
    lefteye.setPixelColor(73, color);
    lefteye.setPixelColor(74, color);
    lefteye.setPixelColor(81, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(90, color);
    lefteye.setPixelColor(91, color);
    lefteye.setPixelColor(92, color);
    lefteye.setPixelColor(99, color);
    lefteye.setPixelColor(100, color);
    lefteye.setPixelColor(101, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(109, color);
    lefteye.setPixelColor(110, color);
    lefteye.setPixelColor(117, color);
    lefteye.setPixelColor(118, color);
    lefteye.setPixelColor(119, color);
    lefteye.setPixelColor(126, color);
    lefteye.setPixelColor(127, color);
    lefteye.setPixelColor(134, color);
    lefteye.setPixelColor(135, color);
    lefteye.setPixelColor(141, color);
    lefteye.setPixelColor(142, color);
    lefteye.setPixelColor(143, color);
    lefteye.setPixelColor(148, color);
    lefteye.setPixelColor(149, color);
    lefteye.setPixelColor(150, color);
    lefteye.setPixelColor(155, color);
    lefteye.setPixelColor(156, color);
    lefteye.setPixelColor(157, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(163, color);
    lefteye.setPixelColor(164, color);
    lefteye.setPixelColor(169, color);
    lefteye.setPixelColor(170, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(176, color);
    lefteye.setPixelColor(177, color);
    lefteye.setPixelColor(178, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(185, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(193, color);
    lefteye.setPixelColor(200, color);
    lefteye.setPixelColor(201, color);
    lefteye.setPixelColor(202, color);
    lefteye.setPixelColor(209, color);
    lefteye.setPixelColor(210, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(219, color);
    lefteye.setPixelColor(220, color);
    lefteye.setPixelColor(227, color);
    lefteye.setPixelColor(228, color);
    lefteye.setPixelColor(229, color);
    lefteye.setPixelColor(236, color);
    lefteye.setPixelColor(237, color);
    lefteye.setPixelColor(238, color);
    lefteye.setPixelColor(245, color);
    lefteye.setPixelColor(246, color);
    lefteye.setPixelColor(247, color);
    lefteye.setPixelColor(254, color);
    lefteye.setPixelColor(255, color);
    righteye.setPixelColor(0, color);
    righteye.setPixelColor(1, color);
    righteye.setPixelColor(8, color);
    righteye.setPixelColor(9, color);
    righteye.setPixelColor(10, color);
    righteye.setPixelColor(17, color);
    righteye.setPixelColor(18, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(26, color);
    righteye.setPixelColor(27, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(35, color);
    righteye.setPixelColor(36, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(44, color);
    righteye.setPixelColor(45, color);
    righteye.setPixelColor(46, color);
    righteye.setPixelColor(53, color);
    righteye.setPixelColor(54, color);
    righteye.setPixelColor(55, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(70, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(77, color);
    righteye.setPixelColor(78, color);
    righteye.setPixelColor(79, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(85, color);
    righteye.setPixelColor(86, color);
    righteye.setPixelColor(91, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(98, color);
    righteye.setPixelColor(99, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(105, color);
    righteye.setPixelColor(106, color);
    righteye.setPixelColor(107, color);
    righteye.setPixelColor(112, color);
    righteye.setPixelColor(113, color);
    righteye.setPixelColor(114, color);
    righteye.setPixelColor(120, color);
    righteye.setPixelColor(121, color);
    righteye.setPixelColor(128, color);
    righteye.setPixelColor(129, color);
    righteye.setPixelColor(136, color);
    righteye.setPixelColor(137, color);
    righteye.setPixelColor(138, color);
    righteye.setPixelColor(145, color);
    righteye.setPixelColor(146, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(154, color);
    righteye.setPixelColor(155, color);
    righteye.setPixelColor(156, color);
    righteye.setPixelColor(163, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(174, color);
    righteye.setPixelColor(181, color);
    righteye.setPixelColor(182, color);
    righteye.setPixelColor(183, color);
    righteye.setPixelColor(190, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(198, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(205, color);
    righteye.setPixelColor(206, color);
    righteye.setPixelColor(207, color);
    righteye.setPixelColor(212, color);
    righteye.setPixelColor(213, color);
    righteye.setPixelColor(214, color);
    righteye.setPixelColor(219, color);
    righteye.setPixelColor(220, color);
    righteye.setPixelColor(221, color);
    righteye.setPixelColor(226, color);
    righteye.setPixelColor(227, color);
    righteye.setPixelColor(228, color);
    righteye.setPixelColor(233, color);
    righteye.setPixelColor(234, color);
    righteye.setPixelColor(235, color);
    righteye.setPixelColor(240, color);
    righteye.setPixelColor(241, color);
    righteye.setPixelColor(242, color);
    righteye.setPixelColor(248, color);
    righteye.setPixelColor(249, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(6, color);
    lefteye.setPixelColor(7, color);
    lefteye.setPixelColor(13, color);
    lefteye.setPixelColor(14, color);
    lefteye.setPixelColor(15, color);
    lefteye.setPixelColor(20, color);
    lefteye.setPixelColor(21, color);
    lefteye.setPixelColor(22, color);
    lefteye.setPixelColor(27, color);
    lefteye.setPixelColor(28, color);
    lefteye.setPixelColor(29, color);
    lefteye.setPixelColor(34, color);
    lefteye.setPixelColor(35, color);
    lefteye.setPixelColor(36, color);
    lefteye.setPixelColor(41, color);
    lefteye.setPixelColor(42, color);
    lefteye.setPixelColor(43, color);
    lefteye.setPixelColor(48, color);
    lefteye.setPixelColor(49, color);
    lefteye.setPixelColor(50, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(57, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(65, color);
    lefteye.setPixelColor(72, color);
    lefteye.setPixelColor(73, color);
    lefteye.setPixelColor(74, color);
    lefteye.setPixelColor(81, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(90, color);
    lefteye.setPixelColor(91, color);
    lefteye.setPixelColor(92, color);
    lefteye.setPixelColor(99, color);
    lefteye.setPixelColor(100, color);
    lefteye.setPixelColor(101, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(109, color);
    lefteye.setPixelColor(110, color);
    lefteye.setPixelColor(117, color);
    lefteye.setPixelColor(118, color);
    lefteye.setPixelColor(119, color);
    lefteye.setPixelColor(126, color);
    lefteye.setPixelColor(127, color);
    lefteye.setPixelColor(134, color);
    lefteye.setPixelColor(135, color);
    lefteye.setPixelColor(141, color);
    lefteye.setPixelColor(142, color);
    lefteye.setPixelColor(143, color);
    lefteye.setPixelColor(148, color);
    lefteye.setPixelColor(149, color);
    lefteye.setPixelColor(150, color);
    lefteye.setPixelColor(155, color);
    lefteye.setPixelColor(156, color);
    lefteye.setPixelColor(157, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(163, color);
    lefteye.setPixelColor(164, color);
    lefteye.setPixelColor(169, color);
    lefteye.setPixelColor(170, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(176, color);
    lefteye.setPixelColor(177, color);
    lefteye.setPixelColor(178, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(185, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(193, color);
    lefteye.setPixelColor(200, color);
    lefteye.setPixelColor(201, color);
    lefteye.setPixelColor(202, color);
    lefteye.setPixelColor(209, color);
    lefteye.setPixelColor(210, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(219, color);
    lefteye.setPixelColor(220, color);
    lefteye.setPixelColor(227, color);
    lefteye.setPixelColor(228, color);
    lefteye.setPixelColor(229, color);
    lefteye.setPixelColor(236, color);
    lefteye.setPixelColor(237, color);
    lefteye.setPixelColor(238, color);
    lefteye.setPixelColor(245, color);
    lefteye.setPixelColor(246, color);
    lefteye.setPixelColor(247, color);
    lefteye.setPixelColor(254, color);
    lefteye.setPixelColor(255, color);
    righteye.setPixelColor(0, color);
    righteye.setPixelColor(1, color);
    righteye.setPixelColor(8, color);
    righteye.setPixelColor(9, color);
    righteye.setPixelColor(10, color);
    righteye.setPixelColor(17, color);
    righteye.setPixelColor(18, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(26, color);
    righteye.setPixelColor(27, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(35, color);
    righteye.setPixelColor(36, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(44, color);
    righteye.setPixelColor(45, color);
    righteye.setPixelColor(46, color);
    righteye.setPixelColor(53, color);
    righteye.setPixelColor(54, color);
    righteye.setPixelColor(55, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(70, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(77, color);
    righteye.setPixelColor(78, color);
    righteye.setPixelColor(79, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(85, color);
    righteye.setPixelColor(86, color);
    righteye.setPixelColor(91, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(98, color);
    righteye.setPixelColor(99, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(105, color);
    righteye.setPixelColor(106, color);
    righteye.setPixelColor(107, color);
    righteye.setPixelColor(112, color);
    righteye.setPixelColor(113, color);
    righteye.setPixelColor(114, color);
    righteye.setPixelColor(120, color);
    righteye.setPixelColor(121, color);
    righteye.setPixelColor(128, color);
    righteye.setPixelColor(129, color);
    righteye.setPixelColor(136, color);
    righteye.setPixelColor(137, color);
    righteye.setPixelColor(138, color);
    righteye.setPixelColor(145, color);
    righteye.setPixelColor(146, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(154, color);
    righteye.setPixelColor(155, color);
    righteye.setPixelColor(156, color);
    righteye.setPixelColor(163, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(174, color);
    righteye.setPixelColor(181, color);
    righteye.setPixelColor(182, color);
    righteye.setPixelColor(183, color);
    righteye.setPixelColor(190, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(198, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(205, color);
    righteye.setPixelColor(206, color);
    righteye.setPixelColor(207, color);
    righteye.setPixelColor(212, color);
    righteye.setPixelColor(213, color);
    righteye.setPixelColor(214, color);
    righteye.setPixelColor(219, color);
    righteye.setPixelColor(220, color);
    righteye.setPixelColor(221, color);
    righteye.setPixelColor(226, color);
    righteye.setPixelColor(227, color);
    righteye.setPixelColor(228, color);
    righteye.setPixelColor(233, color);
    righteye.setPixelColor(234, color);
    righteye.setPixelColor(235, color);
    righteye.setPixelColor(240, color);
    righteye.setPixelColor(241, color);
    righteye.setPixelColor(242, color);
    righteye.setPixelColor(248, color);
    righteye.setPixelColor(249, color);
    righteye.show();
    lefteye.show();
  }
  else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
  {
    ledState = 0; // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
    if(Face001BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face001 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face001BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
    }
  }  
}
// Draw Face002
void Face002()
{
  if (Face002RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(6, color);
    lefteye.setPixelColor(7, color);
    lefteye.setPixelColor(12, color);
    lefteye.setPixelColor(13, color);
    lefteye.setPixelColor(14, color);
    lefteye.setPixelColor(15, color);
    lefteye.setPixelColor(18, color);
    lefteye.setPixelColor(19, color);
    lefteye.setPixelColor(20, color);
    lefteye.setPixelColor(21, color);
    lefteye.setPixelColor(24, color);
    lefteye.setPixelColor(25, color);
    lefteye.setPixelColor(26, color);
    lefteye.setPixelColor(27, color);
    lefteye.setPixelColor(32, color);
    lefteye.setPixelColor(33, color);
    lefteye.setPixelColor(88, color);
    lefteye.setPixelColor(89, color);
    lefteye.setPixelColor(96, color);
    lefteye.setPixelColor(97, color);
    lefteye.setPixelColor(98, color);
    lefteye.setPixelColor(99, color);
    lefteye.setPixelColor(106, color);
    lefteye.setPixelColor(107, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(109, color);
    lefteye.setPixelColor(116, color);
    lefteye.setPixelColor(117, color);
    lefteye.setPixelColor(118, color);
    lefteye.setPixelColor(119, color);
    lefteye.setPixelColor(126, color);
    lefteye.setPixelColor(127, color);
    lefteye.setPixelColor(160, color);
    lefteye.setPixelColor(161, color);
    lefteye.setPixelColor(168, color);
    lefteye.setPixelColor(169, color);
    lefteye.setPixelColor(170, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(178, color);
    lefteye.setPixelColor(179, color);
    lefteye.setPixelColor(180, color);
    lefteye.setPixelColor(181, color);
    lefteye.setPixelColor(188, color);
    lefteye.setPixelColor(189, color);
    lefteye.setPixelColor(190, color);
    lefteye.setPixelColor(191, color);
    lefteye.setPixelColor(196, color);
    lefteye.setPixelColor(197, color);
    lefteye.setPixelColor(198, color);
    lefteye.setPixelColor(199, color);
    lefteye.setPixelColor(202, color);
    lefteye.setPixelColor(203, color);
    lefteye.setPixelColor(204, color);
    lefteye.setPixelColor(205, color);
    lefteye.setPixelColor(208, color);
    lefteye.setPixelColor(209, color);
    lefteye.setPixelColor(210, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(216, color);
    lefteye.setPixelColor(217, color);
    righteye.setPixelColor(0, color);
    righteye.setPixelColor(1, color);
    righteye.setPixelColor(8, color);
    righteye.setPixelColor(9, color);
    righteye.setPixelColor(10, color);
    righteye.setPixelColor(11, color);
    righteye.setPixelColor(18, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(20, color);
    righteye.setPixelColor(21, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(29, color);
    righteye.setPixelColor(30, color);
    righteye.setPixelColor(31, color);
    righteye.setPixelColor(38, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(94, color);
    righteye.setPixelColor(95, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(101, color);
    righteye.setPixelColor(102, color);
    righteye.setPixelColor(103, color);
    righteye.setPixelColor(106, color);
    righteye.setPixelColor(107, color);
    righteye.setPixelColor(108, color);
    righteye.setPixelColor(109, color);
    righteye.setPixelColor(112, color);
    righteye.setPixelColor(113, color);
    righteye.setPixelColor(114, color);
    righteye.setPixelColor(115, color);
    righteye.setPixelColor(120, color);
    righteye.setPixelColor(121, color);
    righteye.setPixelColor(166, color);
    righteye.setPixelColor(167, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(174, color);
    righteye.setPixelColor(175, color);
    righteye.setPixelColor(178, color);
    righteye.setPixelColor(179, color);
    righteye.setPixelColor(180, color);
    righteye.setPixelColor(181, color);
    righteye.setPixelColor(184, color);
    righteye.setPixelColor(185, color);
    righteye.setPixelColor(186, color);
    righteye.setPixelColor(187, color);
    righteye.setPixelColor(192, color);
    righteye.setPixelColor(193, color);
    righteye.setPixelColor(194, color);
    righteye.setPixelColor(195, color);
    righteye.setPixelColor(202, color);
    righteye.setPixelColor(203, color);
    righteye.setPixelColor(204, color);
    righteye.setPixelColor(205, color);
    righteye.setPixelColor(212, color);
    righteye.setPixelColor(213, color);
    righteye.setPixelColor(214, color);
    righteye.setPixelColor(215, color);
    righteye.setPixelColor(222, color);
    righteye.setPixelColor(223, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(6, color);
    lefteye.setPixelColor(7, color);
    lefteye.setPixelColor(12, color);
    lefteye.setPixelColor(13, color);
    lefteye.setPixelColor(14, color);
    lefteye.setPixelColor(15, color);
    lefteye.setPixelColor(18, color);
    lefteye.setPixelColor(19, color);
    lefteye.setPixelColor(20, color);
    lefteye.setPixelColor(21, color);
    lefteye.setPixelColor(24, color);
    lefteye.setPixelColor(25, color);
    lefteye.setPixelColor(26, color);
    lefteye.setPixelColor(27, color);
    lefteye.setPixelColor(32, color);
    lefteye.setPixelColor(33, color);
    lefteye.setPixelColor(88, color);
    lefteye.setPixelColor(89, color);
    lefteye.setPixelColor(96, color);
    lefteye.setPixelColor(97, color);
    lefteye.setPixelColor(98, color);
    lefteye.setPixelColor(99, color);
    lefteye.setPixelColor(106, color);
    lefteye.setPixelColor(107, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(109, color);
    lefteye.setPixelColor(116, color);
    lefteye.setPixelColor(117, color);
    lefteye.setPixelColor(118, color);
    lefteye.setPixelColor(119, color);
    lefteye.setPixelColor(126, color);
    lefteye.setPixelColor(127, color);
    lefteye.setPixelColor(160, color);
    lefteye.setPixelColor(161, color);
    lefteye.setPixelColor(168, color);
    lefteye.setPixelColor(169, color);
    lefteye.setPixelColor(170, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(178, color);
    lefteye.setPixelColor(179, color);
    lefteye.setPixelColor(180, color);
    lefteye.setPixelColor(181, color);
    lefteye.setPixelColor(188, color);
    lefteye.setPixelColor(189, color);
    lefteye.setPixelColor(190, color);
    lefteye.setPixelColor(191, color);
    lefteye.setPixelColor(196, color);
    lefteye.setPixelColor(197, color);
    lefteye.setPixelColor(198, color);
    lefteye.setPixelColor(199, color);
    lefteye.setPixelColor(202, color);
    lefteye.setPixelColor(203, color);
    lefteye.setPixelColor(204, color);
    lefteye.setPixelColor(205, color);
    lefteye.setPixelColor(208, color);
    lefteye.setPixelColor(209, color);
    lefteye.setPixelColor(210, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(216, color);
    lefteye.setPixelColor(217, color);
    righteye.setPixelColor(0, color);
    righteye.setPixelColor(1, color);
    righteye.setPixelColor(8, color);
    righteye.setPixelColor(9, color);
    righteye.setPixelColor(10, color);
    righteye.setPixelColor(11, color);
    righteye.setPixelColor(18, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(20, color);
    righteye.setPixelColor(21, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(29, color);
    righteye.setPixelColor(30, color);
    righteye.setPixelColor(31, color);
    righteye.setPixelColor(38, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(94, color);
    righteye.setPixelColor(95, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(101, color);
    righteye.setPixelColor(102, color);
    righteye.setPixelColor(103, color);
    righteye.setPixelColor(106, color);
    righteye.setPixelColor(107, color);
    righteye.setPixelColor(108, color);
    righteye.setPixelColor(109, color);
    righteye.setPixelColor(112, color);
    righteye.setPixelColor(113, color);
    righteye.setPixelColor(114, color);
    righteye.setPixelColor(115, color);
    righteye.setPixelColor(120, color);
    righteye.setPixelColor(121, color);
    righteye.setPixelColor(166, color);
    righteye.setPixelColor(167, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(174, color);
    righteye.setPixelColor(175, color);
    righteye.setPixelColor(178, color);
    righteye.setPixelColor(179, color);
    righteye.setPixelColor(180, color);
    righteye.setPixelColor(181, color);
    righteye.setPixelColor(184, color);
    righteye.setPixelColor(185, color);
    righteye.setPixelColor(186, color);
    righteye.setPixelColor(187, color);
    righteye.setPixelColor(192, color);
    righteye.setPixelColor(193, color);
    righteye.setPixelColor(194, color);
    righteye.setPixelColor(195, color);
    righteye.setPixelColor(202, color);
    righteye.setPixelColor(203, color);
    righteye.setPixelColor(204, color);
    righteye.setPixelColor(205, color);
    righteye.setPixelColor(212, color);
    righteye.setPixelColor(213, color);
    righteye.setPixelColor(214, color);
    righteye.setPixelColor(215, color);
    righteye.setPixelColor(222, color);
    righteye.setPixelColor(223, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face002BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face002 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face002BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face003
void Face003()
{
  if (Face003RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(32, color);
    lefteye.setPixelColor(40, color);
    lefteye.setPixelColor(48, color);
    lefteye.setPixelColor(49, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(57, color);
    lefteye.setPixelColor(65, color);
    lefteye.setPixelColor(66, color);
    lefteye.setPixelColor(73, color);
    lefteye.setPixelColor(74, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(90, color);
    lefteye.setPixelColor(91, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(163, color);
    lefteye.setPixelColor(170, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(177, color);
    lefteye.setPixelColor(178, color);
    lefteye.setPixelColor(185, color);
    lefteye.setPixelColor(186, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(193, color);
    lefteye.setPixelColor(200, color);
    lefteye.setPixelColor(201, color);
    lefteye.setPixelColor(208, color);
    lefteye.setPixelColor(216, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(47, color);
    righteye.setPixelColor(54, color);
    righteye.setPixelColor(55, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(69, color);
    righteye.setPixelColor(70, color);
    righteye.setPixelColor(77, color);
    righteye.setPixelColor(78, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(85, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(181, color);
    righteye.setPixelColor(182, color);
    righteye.setPixelColor(189, color);
    righteye.setPixelColor(190, color);
    righteye.setPixelColor(198, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(206, color);
    righteye.setPixelColor(207, color);
    righteye.setPixelColor(215, color);
    righteye.setPixelColor(223, color);
    righteye.show();
    lefteye.show(); 
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(32, color);
    lefteye.setPixelColor(40, color);
    lefteye.setPixelColor(48, color);
    lefteye.setPixelColor(49, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(57, color);
    lefteye.setPixelColor(65, color);
    lefteye.setPixelColor(66, color);
    lefteye.setPixelColor(73, color);
    lefteye.setPixelColor(74, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(90, color);
    lefteye.setPixelColor(91, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(163, color);
    lefteye.setPixelColor(170, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(177, color);
    lefteye.setPixelColor(178, color);
    lefteye.setPixelColor(185, color);
    lefteye.setPixelColor(186, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(193, color);
    lefteye.setPixelColor(200, color);
    lefteye.setPixelColor(201, color);
    lefteye.setPixelColor(208, color);
    lefteye.setPixelColor(216, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(47, color);
    righteye.setPixelColor(54, color);
    righteye.setPixelColor(55, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(69, color);
    righteye.setPixelColor(70, color);
    righteye.setPixelColor(77, color);
    righteye.setPixelColor(78, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(85, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(181, color);
    righteye.setPixelColor(182, color);
    righteye.setPixelColor(189, color);
    righteye.setPixelColor(190, color);
    righteye.setPixelColor(198, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(206, color);
    righteye.setPixelColor(207, color);
    righteye.setPixelColor(215, color);
    righteye.setPixelColor(223, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face003BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face003 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face003BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}   
// Draw Face004
void Face004()
{
  if (Face004RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(0, color);
    lefteye.setPixelColor(1, color);
    lefteye.setPixelColor(2, color);
    lefteye.setPixelColor(3, color);
    lefteye.setPixelColor(4, color);
    lefteye.setPixelColor(8, color);
    lefteye.setPixelColor(9, color);
    lefteye.setPixelColor(10, color);
    lefteye.setPixelColor(11, color);
    lefteye.setPixelColor(12, color);
    lefteye.setPixelColor(13, color);
    lefteye.setPixelColor(20, color);
    lefteye.setPixelColor(21, color);
    lefteye.setPixelColor(22, color);
    lefteye.setPixelColor(29, color);
    lefteye.setPixelColor(30, color);
    lefteye.setPixelColor(31, color);
    lefteye.setPixelColor(32, color);
    lefteye.setPixelColor(33, color);
    lefteye.setPixelColor(38, color);
    lefteye.setPixelColor(39, color);
    lefteye.setPixelColor(40, color);
    lefteye.setPixelColor(41, color);
    lefteye.setPixelColor(42, color);
    lefteye.setPixelColor(46, color);
    lefteye.setPixelColor(47, color);
    lefteye.setPixelColor(50, color);
    lefteye.setPixelColor(51, color);
    lefteye.setPixelColor(54, color);
    lefteye.setPixelColor(55, color);
    lefteye.setPixelColor(58, color);
    lefteye.setPixelColor(59, color);
    lefteye.setPixelColor(62, color);
    lefteye.setPixelColor(63, color);
    lefteye.setPixelColor(66, color);
    lefteye.setPixelColor(67, color);
    lefteye.setPixelColor(70, color);
    lefteye.setPixelColor(71, color);
    lefteye.setPixelColor(74, color);
    lefteye.setPixelColor(75, color);
    lefteye.setPixelColor(78, color);
    lefteye.setPixelColor(79, color);
    lefteye.setPixelColor(80, color);
    lefteye.setPixelColor(81, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(86, color);
    lefteye.setPixelColor(87, color);
    lefteye.setPixelColor(88, color);
    lefteye.setPixelColor(89, color);
    lefteye.setPixelColor(94, color);
    lefteye.setPixelColor(95, color);
    lefteye.setPixelColor(101, color);
    lefteye.setPixelColor(102, color);
    lefteye.setPixelColor(103, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(109, color);
    lefteye.setPixelColor(110, color);
    lefteye.setPixelColor(112, color);
    lefteye.setPixelColor(113, color);
    lefteye.setPixelColor(114, color);
    lefteye.setPixelColor(115, color);
    lefteye.setPixelColor(116, color);
    lefteye.setPixelColor(117, color);
    lefteye.setPixelColor(120, color);
    lefteye.setPixelColor(121, color);
    lefteye.setPixelColor(122, color);
    lefteye.setPixelColor(123, color);
    lefteye.setPixelColor(124, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(129, color);
    lefteye.setPixelColor(130, color);
    lefteye.setPixelColor(131, color);
    lefteye.setPixelColor(132, color);
    lefteye.setPixelColor(136, color);
    lefteye.setPixelColor(137, color);
    lefteye.setPixelColor(138, color);
    lefteye.setPixelColor(139, color);
    lefteye.setPixelColor(140, color);
    lefteye.setPixelColor(141, color);
    lefteye.setPixelColor(154, color);
    lefteye.setPixelColor(155, color);
    lefteye.setPixelColor(156, color);
    lefteye.setPixelColor(157, color);
    lefteye.setPixelColor(158, color);
    lefteye.setPixelColor(160, color);
    lefteye.setPixelColor(161, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(163, color);
    lefteye.setPixelColor(164, color);
    lefteye.setPixelColor(165, color);
    lefteye.setPixelColor(166, color);
    lefteye.setPixelColor(167, color);
    lefteye.setPixelColor(168, color);
    lefteye.setPixelColor(169, color);
    lefteye.setPixelColor(170, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(174, color);
    lefteye.setPixelColor(175, color);
    lefteye.setPixelColor(178, color);
    lefteye.setPixelColor(179, color);
    lefteye.setPixelColor(182, color);
    lefteye.setPixelColor(183, color);
    lefteye.setPixelColor(186, color);
    lefteye.setPixelColor(187, color);
    lefteye.setPixelColor(190, color);
    lefteye.setPixelColor(191, color);
    lefteye.setPixelColor(194, color);
    lefteye.setPixelColor(195, color);
    lefteye.setPixelColor(198, color);
    lefteye.setPixelColor(199, color);
    lefteye.setPixelColor(202, color);
    lefteye.setPixelColor(203, color);
    lefteye.setPixelColor(206, color);
    lefteye.setPixelColor(207, color);
    lefteye.setPixelColor(208, color);
    lefteye.setPixelColor(209, color);
    lefteye.setPixelColor(210, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(214, color);
    lefteye.setPixelColor(215, color);
    lefteye.setPixelColor(216, color);
    lefteye.setPixelColor(217, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(219, color);
    lefteye.setPixelColor(222, color);
    lefteye.setPixelColor(223, color);
    lefteye.setPixelColor(229, color);
    lefteye.setPixelColor(230, color);
    lefteye.setPixelColor(231, color);
    lefteye.setPixelColor(236, color);
    lefteye.setPixelColor(237, color);
    lefteye.setPixelColor(238, color);
    lefteye.setPixelColor(240, color);
    lefteye.setPixelColor(241, color);
    lefteye.setPixelColor(242, color);
    lefteye.setPixelColor(243, color);
    lefteye.setPixelColor(244, color);
    lefteye.setPixelColor(245, color);
    lefteye.setPixelColor(248, color);
    lefteye.setPixelColor(249, color);
    lefteye.setPixelColor(250, color);
    lefteye.setPixelColor(251, color);
    lefteye.setPixelColor(252, color);
    righteye.setPixelColor(3, color);
    righteye.setPixelColor(4, color);
    righteye.setPixelColor(5, color);
    righteye.setPixelColor(6, color);
    righteye.setPixelColor(7, color);
    righteye.setPixelColor(10, color);
    righteye.setPixelColor(11, color);
    righteye.setPixelColor(12, color);
    righteye.setPixelColor(13, color);
    righteye.setPixelColor(14, color);
    righteye.setPixelColor(15, color);
    righteye.setPixelColor(17, color);
    righteye.setPixelColor(18, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(24, color);
    righteye.setPixelColor(25, color);
    righteye.setPixelColor(26, color);
    righteye.setPixelColor(32, color);
    righteye.setPixelColor(33, color);
    righteye.setPixelColor(36, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(38, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(40, color);
    righteye.setPixelColor(41, color);
    righteye.setPixelColor(44, color);
    righteye.setPixelColor(45, color);
    righteye.setPixelColor(46, color);
    righteye.setPixelColor(47, color);
    righteye.setPixelColor(48, color);
    righteye.setPixelColor(49, color);
    righteye.setPixelColor(52, color);
    righteye.setPixelColor(53, color);
    righteye.setPixelColor(56, color);
    righteye.setPixelColor(57, color);
    righteye.setPixelColor(60, color);
    righteye.setPixelColor(61, color);
    righteye.setPixelColor(64, color);
    righteye.setPixelColor(65, color);
    righteye.setPixelColor(68, color);
    righteye.setPixelColor(69, color);
    righteye.setPixelColor(72, color);
    righteye.setPixelColor(73, color);
    righteye.setPixelColor(76, color);
    righteye.setPixelColor(77, color);
    righteye.setPixelColor(80, color);
    righteye.setPixelColor(81, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(85, color);
    righteye.setPixelColor(86, color);
    righteye.setPixelColor(87, color);
    righteye.setPixelColor(88, color);
    righteye.setPixelColor(89, color);
    righteye.setPixelColor(90, color);
    righteye.setPixelColor(91, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(94, color);
    righteye.setPixelColor(95, color);
    righteye.setPixelColor(97, color);
    righteye.setPixelColor(98, color);
    righteye.setPixelColor(99, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(101, color);
    righteye.setPixelColor(114, color);
    righteye.setPixelColor(115, color);
    righteye.setPixelColor(116, color);
    righteye.setPixelColor(117, color);
    righteye.setPixelColor(118, color);
    righteye.setPixelColor(119, color);
    righteye.setPixelColor(123, color);
    righteye.setPixelColor(124, color);
    righteye.setPixelColor(125, color);
    righteye.setPixelColor(126, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(131, color);
    righteye.setPixelColor(132, color);
    righteye.setPixelColor(133, color);
    righteye.setPixelColor(134, color);
    righteye.setPixelColor(135, color);
    righteye.setPixelColor(138, color);
    righteye.setPixelColor(139, color);
    righteye.setPixelColor(140, color);
    righteye.setPixelColor(141, color);
    righteye.setPixelColor(142, color);
    righteye.setPixelColor(143, color);
    righteye.setPixelColor(145, color);
    righteye.setPixelColor(146, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(152, color);
    righteye.setPixelColor(153, color);
    righteye.setPixelColor(154, color);
    righteye.setPixelColor(160, color);
    righteye.setPixelColor(161, color);
    righteye.setPixelColor(166, color);
    righteye.setPixelColor(167, color);
    righteye.setPixelColor(168, color);
    righteye.setPixelColor(169, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(174, color);
    righteye.setPixelColor(175, color);
    righteye.setPixelColor(176, color);
    righteye.setPixelColor(177, color);
    righteye.setPixelColor(180, color);
    righteye.setPixelColor(181, color);
    righteye.setPixelColor(184, color);
    righteye.setPixelColor(185, color);
    righteye.setPixelColor(188, color);
    righteye.setPixelColor(189, color);
    righteye.setPixelColor(192, color);
    righteye.setPixelColor(193, color);
    righteye.setPixelColor(196, color);
    righteye.setPixelColor(197, color);
    righteye.setPixelColor(200, color);
    righteye.setPixelColor(201, color);
    righteye.setPixelColor(204, color);
    righteye.setPixelColor(205, color);
    righteye.setPixelColor(208, color);
    righteye.setPixelColor(209, color);
    righteye.setPixelColor(213, color);
    righteye.setPixelColor(214, color);
    righteye.setPixelColor(215, color);
    righteye.setPixelColor(216, color);
    righteye.setPixelColor(217, color);
    righteye.setPixelColor(222, color);
    righteye.setPixelColor(223, color);
    righteye.setPixelColor(224, color);
    righteye.setPixelColor(225, color);
    righteye.setPixelColor(226, color);
    righteye.setPixelColor(233, color);
    righteye.setPixelColor(234, color);
    righteye.setPixelColor(235, color);
    righteye.setPixelColor(242, color);
    righteye.setPixelColor(243, color);
    righteye.setPixelColor(244, color);
    righteye.setPixelColor(245, color);
    righteye.setPixelColor(246, color);
    righteye.setPixelColor(247, color);
    righteye.setPixelColor(251, color);
    righteye.setPixelColor(252, color);
    righteye.setPixelColor(253, color);
    righteye.setPixelColor(254, color);
    righteye.setPixelColor(255, color);
    righteye.show();
    lefteye.show(); 
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(0, color);
    lefteye.setPixelColor(1, color);
    lefteye.setPixelColor(2, color);
    lefteye.setPixelColor(3, color);
    lefteye.setPixelColor(4, color);
    lefteye.setPixelColor(8, color);
    lefteye.setPixelColor(9, color);
    lefteye.setPixelColor(10, color);
    lefteye.setPixelColor(11, color);
    lefteye.setPixelColor(12, color);
    lefteye.setPixelColor(13, color);
    lefteye.setPixelColor(20, color);
    lefteye.setPixelColor(21, color);
    lefteye.setPixelColor(22, color);
    lefteye.setPixelColor(29, color);
    lefteye.setPixelColor(30, color);
    lefteye.setPixelColor(31, color);
    lefteye.setPixelColor(32, color);
    lefteye.setPixelColor(33, color);
    lefteye.setPixelColor(38, color);
    lefteye.setPixelColor(39, color);
    lefteye.setPixelColor(40, color);
    lefteye.setPixelColor(41, color);
    lefteye.setPixelColor(42, color);
    lefteye.setPixelColor(46, color);
    lefteye.setPixelColor(47, color);
    lefteye.setPixelColor(50, color);
    lefteye.setPixelColor(51, color);
    lefteye.setPixelColor(54, color);
    lefteye.setPixelColor(55, color);
    lefteye.setPixelColor(58, color);
    lefteye.setPixelColor(59, color);
    lefteye.setPixelColor(62, color);
    lefteye.setPixelColor(63, color);
    lefteye.setPixelColor(66, color);
    lefteye.setPixelColor(67, color);
    lefteye.setPixelColor(70, color);
    lefteye.setPixelColor(71, color);
    lefteye.setPixelColor(74, color);
    lefteye.setPixelColor(75, color);
    lefteye.setPixelColor(78, color);
    lefteye.setPixelColor(79, color);
    lefteye.setPixelColor(80, color);
    lefteye.setPixelColor(81, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(86, color);
    lefteye.setPixelColor(87, color);
    lefteye.setPixelColor(88, color);
    lefteye.setPixelColor(89, color);
    lefteye.setPixelColor(94, color);
    lefteye.setPixelColor(95, color);
    lefteye.setPixelColor(101, color);
    lefteye.setPixelColor(102, color);
    lefteye.setPixelColor(103, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(109, color);
    lefteye.setPixelColor(110, color);
    lefteye.setPixelColor(112, color);
    lefteye.setPixelColor(113, color);
    lefteye.setPixelColor(114, color);
    lefteye.setPixelColor(115, color);
    lefteye.setPixelColor(116, color);
    lefteye.setPixelColor(117, color);
    lefteye.setPixelColor(120, color);
    lefteye.setPixelColor(121, color);
    lefteye.setPixelColor(122, color);
    lefteye.setPixelColor(123, color);
    lefteye.setPixelColor(124, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(129, color);
    lefteye.setPixelColor(130, color);
    lefteye.setPixelColor(131, color);
    lefteye.setPixelColor(132, color);
    lefteye.setPixelColor(136, color);
    lefteye.setPixelColor(137, color);
    lefteye.setPixelColor(138, color);
    lefteye.setPixelColor(139, color);
    lefteye.setPixelColor(140, color);
    lefteye.setPixelColor(141, color);
    lefteye.setPixelColor(154, color);
    lefteye.setPixelColor(155, color);
    lefteye.setPixelColor(156, color);
    lefteye.setPixelColor(157, color);
    lefteye.setPixelColor(158, color);
    lefteye.setPixelColor(160, color);
    lefteye.setPixelColor(161, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(163, color);
    lefteye.setPixelColor(164, color);
    lefteye.setPixelColor(165, color);
    lefteye.setPixelColor(166, color);
    lefteye.setPixelColor(167, color);
    lefteye.setPixelColor(168, color);
    lefteye.setPixelColor(169, color);
    lefteye.setPixelColor(170, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(174, color);
    lefteye.setPixelColor(175, color);
    lefteye.setPixelColor(178, color);
    lefteye.setPixelColor(179, color);
    lefteye.setPixelColor(182, color);
    lefteye.setPixelColor(183, color);
    lefteye.setPixelColor(186, color);
    lefteye.setPixelColor(187, color);
    lefteye.setPixelColor(190, color);
    lefteye.setPixelColor(191, color);
    lefteye.setPixelColor(194, color);
    lefteye.setPixelColor(195, color);
    lefteye.setPixelColor(198, color);
    lefteye.setPixelColor(199, color);
    lefteye.setPixelColor(202, color);
    lefteye.setPixelColor(203, color);
    lefteye.setPixelColor(206, color);
    lefteye.setPixelColor(207, color);
    lefteye.setPixelColor(208, color);
    lefteye.setPixelColor(209, color);
    lefteye.setPixelColor(210, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(214, color);
    lefteye.setPixelColor(215, color);
    lefteye.setPixelColor(216, color);
    lefteye.setPixelColor(217, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(219, color);
    lefteye.setPixelColor(222, color);
    lefteye.setPixelColor(223, color);
    lefteye.setPixelColor(229, color);
    lefteye.setPixelColor(230, color);
    lefteye.setPixelColor(231, color);
    lefteye.setPixelColor(236, color);
    lefteye.setPixelColor(237, color);
    lefteye.setPixelColor(238, color);
    lefteye.setPixelColor(240, color);
    lefteye.setPixelColor(241, color);
    lefteye.setPixelColor(242, color);
    lefteye.setPixelColor(243, color);
    lefteye.setPixelColor(244, color);
    lefteye.setPixelColor(245, color);
    lefteye.setPixelColor(248, color);
    lefteye.setPixelColor(249, color);
    lefteye.setPixelColor(250, color);
    lefteye.setPixelColor(251, color);
    lefteye.setPixelColor(252, color);
    righteye.setPixelColor(3, color);
    righteye.setPixelColor(4, color);
    righteye.setPixelColor(5, color);
    righteye.setPixelColor(6, color);
    righteye.setPixelColor(7, color);
    righteye.setPixelColor(10, color);
    righteye.setPixelColor(11, color);
    righteye.setPixelColor(12, color);
    righteye.setPixelColor(13, color);
    righteye.setPixelColor(14, color);
    righteye.setPixelColor(15, color);
    righteye.setPixelColor(17, color);
    righteye.setPixelColor(18, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(24, color);
    righteye.setPixelColor(25, color);
    righteye.setPixelColor(26, color);
    righteye.setPixelColor(32, color);
    righteye.setPixelColor(33, color);
    righteye.setPixelColor(36, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(38, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(40, color);
    righteye.setPixelColor(41, color);
    righteye.setPixelColor(44, color);
    righteye.setPixelColor(45, color);
    righteye.setPixelColor(46, color);
    righteye.setPixelColor(47, color);
    righteye.setPixelColor(48, color);
    righteye.setPixelColor(49, color);
    righteye.setPixelColor(52, color);
    righteye.setPixelColor(53, color);
    righteye.setPixelColor(56, color);
    righteye.setPixelColor(57, color);
    righteye.setPixelColor(60, color);
    righteye.setPixelColor(61, color);
    righteye.setPixelColor(64, color);
    righteye.setPixelColor(65, color);
    righteye.setPixelColor(68, color);
    righteye.setPixelColor(69, color);
    righteye.setPixelColor(72, color);
    righteye.setPixelColor(73, color);
    righteye.setPixelColor(76, color);
    righteye.setPixelColor(77, color);
    righteye.setPixelColor(80, color);
    righteye.setPixelColor(81, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(85, color);
    righteye.setPixelColor(86, color);
    righteye.setPixelColor(87, color);
    righteye.setPixelColor(88, color);
    righteye.setPixelColor(89, color);
    righteye.setPixelColor(90, color);
    righteye.setPixelColor(91, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(94, color);
    righteye.setPixelColor(95, color);
    righteye.setPixelColor(97, color);
    righteye.setPixelColor(98, color);
    righteye.setPixelColor(99, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(101, color);
    righteye.setPixelColor(114, color);
    righteye.setPixelColor(115, color);
    righteye.setPixelColor(116, color);
    righteye.setPixelColor(117, color);
    righteye.setPixelColor(118, color);
    righteye.setPixelColor(119, color);
    righteye.setPixelColor(123, color);
    righteye.setPixelColor(124, color);
    righteye.setPixelColor(125, color);
    righteye.setPixelColor(126, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(131, color);
    righteye.setPixelColor(132, color);
    righteye.setPixelColor(133, color);
    righteye.setPixelColor(134, color);
    righteye.setPixelColor(135, color);
    righteye.setPixelColor(138, color);
    righteye.setPixelColor(139, color);
    righteye.setPixelColor(140, color);
    righteye.setPixelColor(141, color);
    righteye.setPixelColor(142, color);
    righteye.setPixelColor(143, color);
    righteye.setPixelColor(145, color);
    righteye.setPixelColor(146, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(152, color);
    righteye.setPixelColor(153, color);
    righteye.setPixelColor(154, color);
    righteye.setPixelColor(160, color);
    righteye.setPixelColor(161, color);
    righteye.setPixelColor(166, color);
    righteye.setPixelColor(167, color);
    righteye.setPixelColor(168, color);
    righteye.setPixelColor(169, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(174, color);
    righteye.setPixelColor(175, color);
    righteye.setPixelColor(176, color);
    righteye.setPixelColor(177, color);
    righteye.setPixelColor(180, color);
    righteye.setPixelColor(181, color);
    righteye.setPixelColor(184, color);
    righteye.setPixelColor(185, color);
    righteye.setPixelColor(188, color);
    righteye.setPixelColor(189, color);
    righteye.setPixelColor(192, color);
    righteye.setPixelColor(193, color);
    righteye.setPixelColor(196, color);
    righteye.setPixelColor(197, color);
    righteye.setPixelColor(200, color);
    righteye.setPixelColor(201, color);
    righteye.setPixelColor(204, color);
    righteye.setPixelColor(205, color);
    righteye.setPixelColor(208, color);
    righteye.setPixelColor(209, color);
    righteye.setPixelColor(213, color);
    righteye.setPixelColor(214, color);
    righteye.setPixelColor(215, color);
    righteye.setPixelColor(216, color);
    righteye.setPixelColor(217, color);
    righteye.setPixelColor(222, color);
    righteye.setPixelColor(223, color);
    righteye.setPixelColor(224, color);
    righteye.setPixelColor(225, color);
    righteye.setPixelColor(226, color);
    righteye.setPixelColor(233, color);
    righteye.setPixelColor(234, color);
    righteye.setPixelColor(235, color);
    righteye.setPixelColor(242, color);
    righteye.setPixelColor(243, color);
    righteye.setPixelColor(244, color);
    righteye.setPixelColor(245, color);
    righteye.setPixelColor(246, color);
    righteye.setPixelColor(247, color);
    righteye.setPixelColor(251, color);
    righteye.setPixelColor(252, color);
    righteye.setPixelColor(253, color);
    righteye.setPixelColor(254, color);
    righteye.setPixelColor(255, color);
    righteye.show();
    lefteye.show(); 
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face004BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face004 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face004BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face005
void Face005()
{
  if (Face005RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(72, color);
    lefteye.setPixelColor(73, color);
    lefteye.setPixelColor(81, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(89, color);
    lefteye.setPixelColor(90, color);
    lefteye.setPixelColor(98, color);
    lefteye.setPixelColor(99, color);
    lefteye.setPixelColor(107, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(115, color);
    lefteye.setPixelColor(116, color);
    lefteye.setPixelColor(124, color);
    lefteye.setPixelColor(125, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(200, color);
    lefteye.setPixelColor(201, color);
    lefteye.setPixelColor(209, color);
    lefteye.setPixelColor(210, color);
    lefteye.setPixelColor(217, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(226, color);
    lefteye.setPixelColor(227, color);
    lefteye.setPixelColor(235, color);
    lefteye.setPixelColor(236, color);
    lefteye.setPixelColor(243, color);
    lefteye.setPixelColor(244, color);
    lefteye.setPixelColor(252, color);
    lefteye.setPixelColor(253, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(78, color);
    righteye.setPixelColor(79, color);
    righteye.setPixelColor(85, color);
    righteye.setPixelColor(86, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(94, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(101, color);
    righteye.setPixelColor(107, color);
    righteye.setPixelColor(108, color);
    righteye.setPixelColor(115, color);
    righteye.setPixelColor(116, color);
    righteye.setPixelColor(122, color);
    righteye.setPixelColor(123, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(206, color);
    righteye.setPixelColor(207, color);
    righteye.setPixelColor(213, color);
    righteye.setPixelColor(214, color);
    righteye.setPixelColor(221, color);
    righteye.setPixelColor(222, color);
    righteye.setPixelColor(228, color);
    righteye.setPixelColor(229, color);
    righteye.setPixelColor(235, color);
    righteye.setPixelColor(236, color);
    righteye.setPixelColor(243, color);
    righteye.setPixelColor(244, color);
    righteye.setPixelColor(250, color);
    righteye.setPixelColor(251, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(72, color);
    lefteye.setPixelColor(73, color);
    lefteye.setPixelColor(81, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(89, color);
    lefteye.setPixelColor(90, color);
    lefteye.setPixelColor(98, color);
    lefteye.setPixelColor(99, color);
    lefteye.setPixelColor(107, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(115, color);
    lefteye.setPixelColor(116, color);
    lefteye.setPixelColor(124, color);
    lefteye.setPixelColor(125, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(200, color);
    lefteye.setPixelColor(201, color);
    lefteye.setPixelColor(209, color);
    lefteye.setPixelColor(210, color);
    lefteye.setPixelColor(217, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(226, color);
    lefteye.setPixelColor(227, color);
    lefteye.setPixelColor(235, color);
    lefteye.setPixelColor(236, color);
    lefteye.setPixelColor(243, color);
    lefteye.setPixelColor(244, color);
    lefteye.setPixelColor(252, color);
    lefteye.setPixelColor(253, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(78, color);
    righteye.setPixelColor(79, color);
    righteye.setPixelColor(85, color);
    righteye.setPixelColor(86, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(94, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(101, color);
    righteye.setPixelColor(107, color);
    righteye.setPixelColor(108, color);
    righteye.setPixelColor(115, color);
    righteye.setPixelColor(116, color);
    righteye.setPixelColor(122, color);
    righteye.setPixelColor(123, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(206, color);
    righteye.setPixelColor(207, color);
    righteye.setPixelColor(213, color);
    righteye.setPixelColor(214, color);
    righteye.setPixelColor(221, color);
    righteye.setPixelColor(222, color);
    righteye.setPixelColor(228, color);
    righteye.setPixelColor(229, color);
    righteye.setPixelColor(235, color);
    righteye.setPixelColor(236, color);
    righteye.setPixelColor(243, color);
    righteye.setPixelColor(244, color);
    righteye.setPixelColor(250, color);
    righteye.setPixelColor(251, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face005BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face005 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face005BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face006
void Face006()
{
  if (Face006RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(0, color);
    lefteye.setPixelColor(1, color);
    lefteye.setPixelColor(2, color);
    lefteye.setPixelColor(3, color);
    lefteye.setPixelColor(4, color);
    lefteye.setPixelColor(8, color);
    lefteye.setPixelColor(9, color);
    lefteye.setPixelColor(10, color);
    lefteye.setPixelColor(11, color);
    lefteye.setPixelColor(12, color);
    lefteye.setPixelColor(13, color);
    lefteye.setPixelColor(14, color);
    lefteye.setPixelColor(21, color);
    lefteye.setPixelColor(22, color);
    lefteye.setPixelColor(23, color);
    lefteye.setPixelColor(30, color);
    lefteye.setPixelColor(31, color);
    lefteye.setPixelColor(38, color);
    lefteye.setPixelColor(39, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(72, color);
    lefteye.setPixelColor(80, color);
    lefteye.setPixelColor(88, color);
    lefteye.setPixelColor(112, color);
    lefteye.setPixelColor(120, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(136, color);
    lefteye.setPixelColor(160, color);
    lefteye.setPixelColor(168, color);
    lefteye.setPixelColor(176, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(185, color);
    lefteye.setPixelColor(186, color);
    lefteye.setPixelColor(187, color);
    lefteye.setPixelColor(188, color);
    lefteye.setPixelColor(189, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(193, color);
    lefteye.setPixelColor(194, color);
    lefteye.setPixelColor(195, color);
    lefteye.setPixelColor(196, color);
    lefteye.setPixelColor(197, color);
    lefteye.setPixelColor(198, color);
    lefteye.setPixelColor(205, color);
    lefteye.setPixelColor(206, color);
    lefteye.setPixelColor(207, color);
    lefteye.setPixelColor(214, color);
    lefteye.setPixelColor(215, color);
    lefteye.setPixelColor(222, color);
    lefteye.setPixelColor(223, color);
    lefteye.setPixelColor(230, color);
    lefteye.setPixelColor(231, color);
    lefteye.setPixelColor(237, color);
    lefteye.setPixelColor(238, color);
    lefteye.setPixelColor(239, color);
    lefteye.setPixelColor(240, color);
    lefteye.setPixelColor(241, color);
    lefteye.setPixelColor(242, color);
    lefteye.setPixelColor(243, color);
    lefteye.setPixelColor(244, color);
    lefteye.setPixelColor(245, color);
    lefteye.setPixelColor(246, color);
    lefteye.setPixelColor(248, color);
    lefteye.setPixelColor(249, color);
    lefteye.setPixelColor(250, color);
    lefteye.setPixelColor(251, color);
    lefteye.setPixelColor(252, color);
    lefteye.setPixelColor(253, color);
    righteye.setPixelColor(2, color);
    righteye.setPixelColor(3, color);
    righteye.setPixelColor(4, color);
    righteye.setPixelColor(5, color);
    righteye.setPixelColor(6, color);
    righteye.setPixelColor(7, color);
    righteye.setPixelColor(9, color);
    righteye.setPixelColor(10, color);
    righteye.setPixelColor(11, color);
    righteye.setPixelColor(12, color);
    righteye.setPixelColor(13, color);
    righteye.setPixelColor(14, color);
    righteye.setPixelColor(15, color);
    righteye.setPixelColor(16, color);
    righteye.setPixelColor(17, color);
    righteye.setPixelColor(18, color);
    righteye.setPixelColor(24, color);
    righteye.setPixelColor(25, color);
    righteye.setPixelColor(32, color);
    righteye.setPixelColor(33, color);
    righteye.setPixelColor(40, color);
    righteye.setPixelColor(41, color);
    righteye.setPixelColor(48, color);
    righteye.setPixelColor(49, color);
    righteye.setPixelColor(50, color);
    righteye.setPixelColor(57, color);
    righteye.setPixelColor(58, color);
    righteye.setPixelColor(59, color);
    righteye.setPixelColor(60, color);
    righteye.setPixelColor(61, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(66, color);
    righteye.setPixelColor(67, color);
    righteye.setPixelColor(68, color);
    righteye.setPixelColor(69, color);
    righteye.setPixelColor(70, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(79, color);
    righteye.setPixelColor(87, color);
    righteye.setPixelColor(95, color);
    righteye.setPixelColor(119, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(135, color);
    righteye.setPixelColor(143, color);
    righteye.setPixelColor(167, color);
    righteye.setPixelColor(175, color);
    righteye.setPixelColor(183, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(216, color);
    righteye.setPixelColor(217, color);
    righteye.setPixelColor(224, color);
    righteye.setPixelColor(225, color);
    righteye.setPixelColor(232, color);
    righteye.setPixelColor(233, color);
    righteye.setPixelColor(234, color);
    righteye.setPixelColor(241, color);
    righteye.setPixelColor(242, color);
    righteye.setPixelColor(243, color);
    righteye.setPixelColor(244, color);
    righteye.setPixelColor(245, color);
    righteye.setPixelColor(246, color);
    righteye.setPixelColor(247, color);
    righteye.setPixelColor(250, color);
    righteye.setPixelColor(251, color);
    righteye.setPixelColor(252, color);
    righteye.setPixelColor(253, color);
    righteye.setPixelColor(254, color);
    righteye.setPixelColor(255, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(0, color);
    lefteye.setPixelColor(1, color);
    lefteye.setPixelColor(2, color);
    lefteye.setPixelColor(3, color);
    lefteye.setPixelColor(4, color);
    lefteye.setPixelColor(8, color);
    lefteye.setPixelColor(9, color);
    lefteye.setPixelColor(10, color);
    lefteye.setPixelColor(11, color);
    lefteye.setPixelColor(12, color);
    lefteye.setPixelColor(13, color);
    lefteye.setPixelColor(14, color);
    lefteye.setPixelColor(21, color);
    lefteye.setPixelColor(22, color);
    lefteye.setPixelColor(23, color);
    lefteye.setPixelColor(30, color);
    lefteye.setPixelColor(31, color);
    lefteye.setPixelColor(38, color);
    lefteye.setPixelColor(39, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(72, color);
    lefteye.setPixelColor(80, color);
    lefteye.setPixelColor(88, color);
    lefteye.setPixelColor(112, color);
    lefteye.setPixelColor(120, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(136, color);
    lefteye.setPixelColor(160, color);
    lefteye.setPixelColor(168, color);
    lefteye.setPixelColor(176, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(185, color);
    lefteye.setPixelColor(186, color);
    lefteye.setPixelColor(187, color);
    lefteye.setPixelColor(188, color);
    lefteye.setPixelColor(189, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(193, color);
    lefteye.setPixelColor(194, color);
    lefteye.setPixelColor(195, color);
    lefteye.setPixelColor(196, color);
    lefteye.setPixelColor(197, color);
    lefteye.setPixelColor(198, color);
    lefteye.setPixelColor(205, color);
    lefteye.setPixelColor(206, color);
    lefteye.setPixelColor(207, color);
    lefteye.setPixelColor(214, color);
    lefteye.setPixelColor(215, color);
    lefteye.setPixelColor(222, color);
    lefteye.setPixelColor(223, color);
    lefteye.setPixelColor(230, color);
    lefteye.setPixelColor(231, color);
    lefteye.setPixelColor(237, color);
    lefteye.setPixelColor(238, color);
    lefteye.setPixelColor(239, color);
    lefteye.setPixelColor(240, color);
    lefteye.setPixelColor(241, color);
    lefteye.setPixelColor(242, color);
    lefteye.setPixelColor(243, color);
    lefteye.setPixelColor(244, color);
    lefteye.setPixelColor(245, color);
    lefteye.setPixelColor(246, color);
    lefteye.setPixelColor(248, color);
    lefteye.setPixelColor(249, color);
    lefteye.setPixelColor(250, color);
    lefteye.setPixelColor(251, color);
    lefteye.setPixelColor(252, color);
    lefteye.setPixelColor(253, color);
    righteye.setPixelColor(2, color);
    righteye.setPixelColor(3, color);
    righteye.setPixelColor(4, color);
    righteye.setPixelColor(5, color);
    righteye.setPixelColor(6, color);
    righteye.setPixelColor(7, color);
    righteye.setPixelColor(9, color);
    righteye.setPixelColor(10, color);
    righteye.setPixelColor(11, color);
    righteye.setPixelColor(12, color);
    righteye.setPixelColor(13, color);
    righteye.setPixelColor(14, color);
    righteye.setPixelColor(15, color);
    righteye.setPixelColor(16, color);
    righteye.setPixelColor(17, color);
    righteye.setPixelColor(18, color);
    righteye.setPixelColor(24, color);
    righteye.setPixelColor(25, color);
    righteye.setPixelColor(32, color);
    righteye.setPixelColor(33, color);
    righteye.setPixelColor(40, color);
    righteye.setPixelColor(41, color);
    righteye.setPixelColor(48, color);
    righteye.setPixelColor(49, color);
    righteye.setPixelColor(50, color);
    righteye.setPixelColor(57, color);
    righteye.setPixelColor(58, color);
    righteye.setPixelColor(59, color);
    righteye.setPixelColor(60, color);
    righteye.setPixelColor(61, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(66, color);
    righteye.setPixelColor(67, color);
    righteye.setPixelColor(68, color);
    righteye.setPixelColor(69, color);
    righteye.setPixelColor(70, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(79, color);
    righteye.setPixelColor(87, color);
    righteye.setPixelColor(95, color);
    righteye.setPixelColor(119, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(135, color);
    righteye.setPixelColor(143, color);
    righteye.setPixelColor(167, color);
    righteye.setPixelColor(175, color);
    righteye.setPixelColor(183, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(216, color);
    righteye.setPixelColor(217, color);
    righteye.setPixelColor(224, color);
    righteye.setPixelColor(225, color);
    righteye.setPixelColor(232, color);
    righteye.setPixelColor(233, color);
    righteye.setPixelColor(234, color);
    righteye.setPixelColor(241, color);
    righteye.setPixelColor(242, color);
    righteye.setPixelColor(243, color);
    righteye.setPixelColor(244, color);
    righteye.setPixelColor(245, color);
    righteye.setPixelColor(246, color);
    righteye.setPixelColor(247, color);
    righteye.setPixelColor(250, color);
    righteye.setPixelColor(251, color);
    righteye.setPixelColor(252, color);
    righteye.setPixelColor(253, color);
    righteye.setPixelColor(254, color);
    righteye.setPixelColor(255, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face006BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face006 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face006BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face007
void Face007()
{
  if (Face007RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(0, color);
    lefteye.setPixelColor(1, color);
    lefteye.setPixelColor(2, color);
    lefteye.setPixelColor(3, color);
    lefteye.setPixelColor(4, color);
    lefteye.setPixelColor(8, color);
    lefteye.setPixelColor(9, color);
    lefteye.setPixelColor(10, color);
    lefteye.setPixelColor(11, color);
    lefteye.setPixelColor(12, color);
    lefteye.setPixelColor(13, color);
    lefteye.setPixelColor(20, color);
    lefteye.setPixelColor(21, color);
    lefteye.setPixelColor(22, color);
    lefteye.setPixelColor(29, color);
    lefteye.setPixelColor(30, color);
    lefteye.setPixelColor(31, color);
    lefteye.setPixelColor(38, color);
    lefteye.setPixelColor(39, color);
    lefteye.setPixelColor(46, color);
    lefteye.setPixelColor(47, color);
    lefteye.setPixelColor(54, color);
    lefteye.setPixelColor(55, color);
    lefteye.setPixelColor(62, color);
    lefteye.setPixelColor(63, color);
    lefteye.setPixelColor(70, color);
    lefteye.setPixelColor(71, color);
    lefteye.setPixelColor(78, color);
    lefteye.setPixelColor(79, color);
    lefteye.setPixelColor(86, color);
    lefteye.setPixelColor(87, color);
    lefteye.setPixelColor(94, color);
    lefteye.setPixelColor(95, color);
    lefteye.setPixelColor(101, color);
    lefteye.setPixelColor(102, color);
    lefteye.setPixelColor(103, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(109, color);
    lefteye.setPixelColor(110, color);
    lefteye.setPixelColor(112, color);
    lefteye.setPixelColor(113, color);
    lefteye.setPixelColor(114, color);
    lefteye.setPixelColor(115, color);
    lefteye.setPixelColor(116, color);
    lefteye.setPixelColor(117, color);
    lefteye.setPixelColor(120, color);
    lefteye.setPixelColor(121, color);
    lefteye.setPixelColor(122, color);
    lefteye.setPixelColor(123, color);
    lefteye.setPixelColor(124, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(129, color);
    lefteye.setPixelColor(130, color);
    lefteye.setPixelColor(131, color);
    lefteye.setPixelColor(132, color);
    lefteye.setPixelColor(136, color);
    lefteye.setPixelColor(137, color);
    lefteye.setPixelColor(138, color);
    lefteye.setPixelColor(139, color);
    lefteye.setPixelColor(140, color);
    lefteye.setPixelColor(141, color);
    lefteye.setPixelColor(148, color);
    lefteye.setPixelColor(149, color);
    lefteye.setPixelColor(150, color);
    lefteye.setPixelColor(157, color);
    lefteye.setPixelColor(158, color);
    lefteye.setPixelColor(159, color);
    lefteye.setPixelColor(166, color);
    lefteye.setPixelColor(167, color);
    lefteye.setPixelColor(174, color);
    lefteye.setPixelColor(175, color);
    lefteye.setPixelColor(182, color);
    lefteye.setPixelColor(183, color);
    lefteye.setPixelColor(190, color);
    lefteye.setPixelColor(191, color);
    lefteye.setPixelColor(198, color);
    lefteye.setPixelColor(199, color);
    lefteye.setPixelColor(206, color);
    lefteye.setPixelColor(207, color);
    lefteye.setPixelColor(214, color);
    lefteye.setPixelColor(215, color);
    lefteye.setPixelColor(222, color);
    lefteye.setPixelColor(223, color);
    lefteye.setPixelColor(229, color);
    lefteye.setPixelColor(230, color);
    lefteye.setPixelColor(231, color);
    lefteye.setPixelColor(236, color);
    lefteye.setPixelColor(237, color);
    lefteye.setPixelColor(238, color);
    lefteye.setPixelColor(240, color);
    lefteye.setPixelColor(241, color);
    lefteye.setPixelColor(242, color);
    lefteye.setPixelColor(243, color);
    lefteye.setPixelColor(244, color);
    lefteye.setPixelColor(245, color);
    lefteye.setPixelColor(248, color);
    lefteye.setPixelColor(249, color);
    lefteye.setPixelColor(250, color);
    lefteye.setPixelColor(251, color);
    lefteye.setPixelColor(252, color);
    righteye.setPixelColor(21, color);
    righteye.setPixelColor(22, color);
    righteye.setPixelColor(23, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(29, color);
    righteye.setPixelColor(30, color);
    righteye.setPixelColor(31, color);
    righteye.setPixelColor(35, color);
    righteye.setPixelColor(36, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(42, color);
    righteye.setPixelColor(43, color);
    righteye.setPixelColor(44, color);
    righteye.setPixelColor(50, color);
    righteye.setPixelColor(51, color);
    righteye.setPixelColor(58, color);
    righteye.setPixelColor(59, color);
    righteye.setPixelColor(66, color);
    righteye.setPixelColor(67, color);
    righteye.setPixelColor(74, color);
    righteye.setPixelColor(75, color);
    righteye.setPixelColor(82, color);
    righteye.setPixelColor(83, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(91, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(101, color);
    righteye.setPixelColor(102, color);
    righteye.setPixelColor(103, color);
    righteye.setPixelColor(109, color);
    righteye.setPixelColor(110, color);
    righteye.setPixelColor(111, color);
    righteye.setPixelColor(149, color);
    righteye.setPixelColor(150, color);
    righteye.setPixelColor(151, color);
    righteye.setPixelColor(156, color);
    righteye.setPixelColor(157, color);
    righteye.setPixelColor(158, color);
    righteye.setPixelColor(159, color);
    righteye.setPixelColor(163, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(170, color);
    righteye.setPixelColor(171, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(178, color);
    righteye.setPixelColor(179, color);
    righteye.setPixelColor(186, color);
    righteye.setPixelColor(187, color);
    righteye.setPixelColor(194, color);
    righteye.setPixelColor(195, color);
    righteye.setPixelColor(202, color);
    righteye.setPixelColor(203, color);
    righteye.setPixelColor(210, color);
    righteye.setPixelColor(211, color);
    righteye.setPixelColor(212, color);
    righteye.setPixelColor(219, color);
    righteye.setPixelColor(220, color);
    righteye.setPixelColor(221, color);
    righteye.setPixelColor(228, color);
    righteye.setPixelColor(229, color);
    righteye.setPixelColor(230, color);
    righteye.setPixelColor(231, color);
    righteye.setPixelColor(237, color);
    righteye.setPixelColor(238, color);
    righteye.setPixelColor(239, color);
    righteye.show();
    lefteye.show(); 
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(0, color);
    lefteye.setPixelColor(1, color);
    lefteye.setPixelColor(2, color);
    lefteye.setPixelColor(3, color);
    lefteye.setPixelColor(4, color);
    lefteye.setPixelColor(8, color);
    lefteye.setPixelColor(9, color);
    lefteye.setPixelColor(10, color);
    lefteye.setPixelColor(11, color);
    lefteye.setPixelColor(12, color);
    lefteye.setPixelColor(13, color);
    lefteye.setPixelColor(20, color);
    lefteye.setPixelColor(21, color);
    lefteye.setPixelColor(22, color);
    lefteye.setPixelColor(29, color);
    lefteye.setPixelColor(30, color);
    lefteye.setPixelColor(31, color);
    lefteye.setPixelColor(38, color);
    lefteye.setPixelColor(39, color);
    lefteye.setPixelColor(46, color);
    lefteye.setPixelColor(47, color);
    lefteye.setPixelColor(54, color);
    lefteye.setPixelColor(55, color);
    lefteye.setPixelColor(62, color);
    lefteye.setPixelColor(63, color);
    lefteye.setPixelColor(70, color);
    lefteye.setPixelColor(71, color);
    lefteye.setPixelColor(78, color);
    lefteye.setPixelColor(79, color);
    lefteye.setPixelColor(86, color);
    lefteye.setPixelColor(87, color);
    lefteye.setPixelColor(94, color);
    lefteye.setPixelColor(95, color);
    lefteye.setPixelColor(101, color);
    lefteye.setPixelColor(102, color);
    lefteye.setPixelColor(103, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(109, color);
    lefteye.setPixelColor(110, color);
    lefteye.setPixelColor(112, color);
    lefteye.setPixelColor(113, color);
    lefteye.setPixelColor(114, color);
    lefteye.setPixelColor(115, color);
    lefteye.setPixelColor(116, color);
    lefteye.setPixelColor(117, color);
    lefteye.setPixelColor(120, color);
    lefteye.setPixelColor(121, color);
    lefteye.setPixelColor(122, color);
    lefteye.setPixelColor(123, color);
    lefteye.setPixelColor(124, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(129, color);
    lefteye.setPixelColor(130, color);
    lefteye.setPixelColor(131, color);
    lefteye.setPixelColor(132, color);
    lefteye.setPixelColor(136, color);
    lefteye.setPixelColor(137, color);
    lefteye.setPixelColor(138, color);
    lefteye.setPixelColor(139, color);
    lefteye.setPixelColor(140, color);
    lefteye.setPixelColor(141, color);
    lefteye.setPixelColor(148, color);
    lefteye.setPixelColor(149, color);
    lefteye.setPixelColor(150, color);
    lefteye.setPixelColor(157, color);
    lefteye.setPixelColor(158, color);
    lefteye.setPixelColor(159, color);
    lefteye.setPixelColor(166, color);
    lefteye.setPixelColor(167, color);
    lefteye.setPixelColor(174, color);
    lefteye.setPixelColor(175, color);
    lefteye.setPixelColor(182, color);
    lefteye.setPixelColor(183, color);
    lefteye.setPixelColor(190, color);
    lefteye.setPixelColor(191, color);
    lefteye.setPixelColor(198, color);
    lefteye.setPixelColor(199, color);
    lefteye.setPixelColor(206, color);
    lefteye.setPixelColor(207, color);
    lefteye.setPixelColor(214, color);
    lefteye.setPixelColor(215, color);
    lefteye.setPixelColor(222, color);
    lefteye.setPixelColor(223, color);
    lefteye.setPixelColor(229, color);
    lefteye.setPixelColor(230, color);
    lefteye.setPixelColor(231, color);
    lefteye.setPixelColor(236, color);
    lefteye.setPixelColor(237, color);
    lefteye.setPixelColor(238, color);
    lefteye.setPixelColor(240, color);
    lefteye.setPixelColor(241, color);
    lefteye.setPixelColor(242, color);
    lefteye.setPixelColor(243, color);
    lefteye.setPixelColor(244, color);
    lefteye.setPixelColor(245, color);
    lefteye.setPixelColor(248, color);
    lefteye.setPixelColor(249, color);
    lefteye.setPixelColor(250, color);
    lefteye.setPixelColor(251, color);
    lefteye.setPixelColor(252, color);
    righteye.setPixelColor(21, color);
    righteye.setPixelColor(22, color);
    righteye.setPixelColor(23, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(29, color);
    righteye.setPixelColor(30, color);
    righteye.setPixelColor(31, color);
    righteye.setPixelColor(35, color);
    righteye.setPixelColor(36, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(42, color);
    righteye.setPixelColor(43, color);
    righteye.setPixelColor(44, color);
    righteye.setPixelColor(50, color);
    righteye.setPixelColor(51, color);
    righteye.setPixelColor(58, color);
    righteye.setPixelColor(59, color);
    righteye.setPixelColor(66, color);
    righteye.setPixelColor(67, color);
    righteye.setPixelColor(74, color);
    righteye.setPixelColor(75, color);
    righteye.setPixelColor(82, color);
    righteye.setPixelColor(83, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(91, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(101, color);
    righteye.setPixelColor(102, color);
    righteye.setPixelColor(103, color);
    righteye.setPixelColor(109, color);
    righteye.setPixelColor(110, color);
    righteye.setPixelColor(111, color);
    righteye.setPixelColor(149, color);
    righteye.setPixelColor(150, color);
    righteye.setPixelColor(151, color);
    righteye.setPixelColor(156, color);
    righteye.setPixelColor(157, color);
    righteye.setPixelColor(158, color);
    righteye.setPixelColor(159, color);
    righteye.setPixelColor(163, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(170, color);
    righteye.setPixelColor(171, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(178, color);
    righteye.setPixelColor(179, color);
    righteye.setPixelColor(186, color);
    righteye.setPixelColor(187, color);
    righteye.setPixelColor(194, color);
    righteye.setPixelColor(195, color);
    righteye.setPixelColor(202, color);
    righteye.setPixelColor(203, color);
    righteye.setPixelColor(210, color);
    righteye.setPixelColor(211, color);
    righteye.setPixelColor(212, color);
    righteye.setPixelColor(219, color);
    righteye.setPixelColor(220, color);
    righteye.setPixelColor(221, color);
    righteye.setPixelColor(228, color);
    righteye.setPixelColor(229, color);
    righteye.setPixelColor(230, color);
    righteye.setPixelColor(231, color);
    righteye.setPixelColor(237, color);
    righteye.setPixelColor(238, color);
    righteye.setPixelColor(239, color);
    righteye.show();
    lefteye.show(); 
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face007BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face007 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face007BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face008
void Face008()
{
  if (Face008RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(4, color);
    lefteye.setPixelColor(5, color);
    lefteye.setPixelColor(11, color);
    lefteye.setPixelColor(12, color);
    lefteye.setPixelColor(19, color);
    lefteye.setPixelColor(20, color);
    lefteye.setPixelColor(26, color);
    lefteye.setPixelColor(27, color);
    lefteye.setPixelColor(33, color);
    lefteye.setPixelColor(34, color);
    lefteye.setPixelColor(41, color);
    lefteye.setPixelColor(42, color);
    lefteye.setPixelColor(48, color);
    lefteye.setPixelColor(49, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(132, color);
    lefteye.setPixelColor(133, color);
    lefteye.setPixelColor(139, color);
    lefteye.setPixelColor(140, color);
    lefteye.setPixelColor(147, color);
    lefteye.setPixelColor(148, color);
    lefteye.setPixelColor(154, color);
    lefteye.setPixelColor(155, color);
    lefteye.setPixelColor(161, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(169, color);
    lefteye.setPixelColor(170, color);
    lefteye.setPixelColor(176, color);
    lefteye.setPixelColor(177, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(192, color);
    righteye.setPixelColor(2, color);
    righteye.setPixelColor(3, color);
    righteye.setPixelColor(11, color);
    righteye.setPixelColor(12, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(20, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(29, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(38, color);
    righteye.setPixelColor(45, color);
    righteye.setPixelColor(46, color);
    righteye.setPixelColor(54, color);
    righteye.setPixelColor(55, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(130, color);
    righteye.setPixelColor(131, color);
    righteye.setPixelColor(139, color);
    righteye.setPixelColor(140, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(148, color);
    righteye.setPixelColor(156, color);
    righteye.setPixelColor(157, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(166, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(174, color);
    righteye.setPixelColor(182, color);
    righteye.setPixelColor(183, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(199, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(4, color);
    lefteye.setPixelColor(5, color);
    lefteye.setPixelColor(11, color);
    lefteye.setPixelColor(12, color);
    lefteye.setPixelColor(19, color);
    lefteye.setPixelColor(20, color);
    lefteye.setPixelColor(26, color);
    lefteye.setPixelColor(27, color);
    lefteye.setPixelColor(33, color);
    lefteye.setPixelColor(34, color);
    lefteye.setPixelColor(41, color);
    lefteye.setPixelColor(42, color);
    lefteye.setPixelColor(48, color);
    lefteye.setPixelColor(49, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(132, color);
    lefteye.setPixelColor(133, color);
    lefteye.setPixelColor(139, color);
    lefteye.setPixelColor(140, color);
    lefteye.setPixelColor(147, color);
    lefteye.setPixelColor(148, color);
    lefteye.setPixelColor(154, color);
    lefteye.setPixelColor(155, color);
    lefteye.setPixelColor(161, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(169, color);
    lefteye.setPixelColor(170, color);
    lefteye.setPixelColor(176, color);
    lefteye.setPixelColor(177, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(192, color);
    righteye.setPixelColor(2, color);
    righteye.setPixelColor(3, color);
    righteye.setPixelColor(11, color);
    righteye.setPixelColor(12, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(20, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(29, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(38, color);
    righteye.setPixelColor(45, color);
    righteye.setPixelColor(46, color);
    righteye.setPixelColor(54, color);
    righteye.setPixelColor(55, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(130, color);
    righteye.setPixelColor(131, color);
    righteye.setPixelColor(139, color);
    righteye.setPixelColor(140, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(148, color);
    righteye.setPixelColor(156, color);
    righteye.setPixelColor(157, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(166, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(174, color);
    righteye.setPixelColor(182, color);
    righteye.setPixelColor(183, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(199, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face008BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face008 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face008BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face009
void Face009()
{
  if (Face009RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(0, color);
    lefteye.setPixelColor(1, color);
    lefteye.setPixelColor(2, color);
    lefteye.setPixelColor(3, color);
    lefteye.setPixelColor(4, color);
    lefteye.setPixelColor(5, color);
    lefteye.setPixelColor(6, color);
    lefteye.setPixelColor(7, color);
    lefteye.setPixelColor(8, color);
    lefteye.setPixelColor(9, color);
    lefteye.setPixelColor(10, color);
    lefteye.setPixelColor(11, color);
    lefteye.setPixelColor(12, color);
    lefteye.setPixelColor(13, color);
    lefteye.setPixelColor(14, color);
    lefteye.setPixelColor(15, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(65, color);
    lefteye.setPixelColor(72, color);
    lefteye.setPixelColor(73, color);
    lefteye.setPixelColor(74, color);
    lefteye.setPixelColor(81, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(90, color);
    lefteye.setPixelColor(91, color);
    lefteye.setPixelColor(92, color);
    lefteye.setPixelColor(99, color);
    lefteye.setPixelColor(100, color);
    lefteye.setPixelColor(101, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(109, color);
    lefteye.setPixelColor(110, color);
    lefteye.setPixelColor(112, color);
    lefteye.setPixelColor(113, color);
    lefteye.setPixelColor(114, color);
    lefteye.setPixelColor(115, color);
    lefteye.setPixelColor(116, color);
    lefteye.setPixelColor(117, color);
    lefteye.setPixelColor(118, color);
    lefteye.setPixelColor(119, color);
    lefteye.setPixelColor(120, color);
    lefteye.setPixelColor(121, color);
    lefteye.setPixelColor(122, color);
    lefteye.setPixelColor(123, color);
    lefteye.setPixelColor(124, color);
    lefteye.setPixelColor(125, color);
    lefteye.setPixelColor(126, color);
    lefteye.setPixelColor(127, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(129, color);
    lefteye.setPixelColor(130, color);
    lefteye.setPixelColor(131, color);
    lefteye.setPixelColor(132, color);
    lefteye.setPixelColor(133, color);
    lefteye.setPixelColor(134, color);
    lefteye.setPixelColor(135, color);
    lefteye.setPixelColor(136, color);
    lefteye.setPixelColor(137, color);
    lefteye.setPixelColor(138, color);
    lefteye.setPixelColor(139, color);
    lefteye.setPixelColor(140, color);
    lefteye.setPixelColor(141, color);
    lefteye.setPixelColor(142, color);
    lefteye.setPixelColor(143, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(193, color);
    lefteye.setPixelColor(200, color);
    lefteye.setPixelColor(201, color);
    lefteye.setPixelColor(202, color);
    lefteye.setPixelColor(209, color);
    lefteye.setPixelColor(210, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(219, color);
    lefteye.setPixelColor(220, color);
    lefteye.setPixelColor(227, color);
    lefteye.setPixelColor(228, color);
    lefteye.setPixelColor(229, color);
    lefteye.setPixelColor(236, color);
    lefteye.setPixelColor(237, color);
    lefteye.setPixelColor(238, color);
    lefteye.setPixelColor(240, color);
    lefteye.setPixelColor(241, color);
    lefteye.setPixelColor(242, color);
    lefteye.setPixelColor(243, color);
    lefteye.setPixelColor(244, color);
    lefteye.setPixelColor(245, color);
    lefteye.setPixelColor(246, color);
    lefteye.setPixelColor(247, color);
    lefteye.setPixelColor(248, color);
    lefteye.setPixelColor(249, color);
    lefteye.setPixelColor(250, color);
    lefteye.setPixelColor(251, color);
    lefteye.setPixelColor(252, color);
    lefteye.setPixelColor(253, color);
    lefteye.setPixelColor(254, color);
    lefteye.setPixelColor(255, color);
    righteye.setPixelColor(0, color);
    righteye.setPixelColor(1, color);
    righteye.setPixelColor(2, color);
    righteye.setPixelColor(3, color);
    righteye.setPixelColor(4, color);
    righteye.setPixelColor(5, color);
    righteye.setPixelColor(6, color);
    righteye.setPixelColor(7, color);
    righteye.setPixelColor(8, color);
    righteye.setPixelColor(9, color);
    righteye.setPixelColor(10, color);
    righteye.setPixelColor(11, color);
    righteye.setPixelColor(12, color);
    righteye.setPixelColor(13, color);
    righteye.setPixelColor(14, color);
    righteye.setPixelColor(15, color);
    righteye.setPixelColor(17, color);
    righteye.setPixelColor(18, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(26, color);
    righteye.setPixelColor(27, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(35, color);
    righteye.setPixelColor(36, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(44, color);
    righteye.setPixelColor(45, color);
    righteye.setPixelColor(46, color);
    righteye.setPixelColor(53, color);
    righteye.setPixelColor(54, color);
    righteye.setPixelColor(55, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(112, color);
    righteye.setPixelColor(113, color);
    righteye.setPixelColor(114, color);
    righteye.setPixelColor(115, color);
    righteye.setPixelColor(116, color);
    righteye.setPixelColor(117, color);
    righteye.setPixelColor(118, color);
    righteye.setPixelColor(119, color);
    righteye.setPixelColor(120, color);
    righteye.setPixelColor(121, color);
    righteye.setPixelColor(122, color);
    righteye.setPixelColor(123, color);
    righteye.setPixelColor(124, color);
    righteye.setPixelColor(125, color);
    righteye.setPixelColor(126, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(128, color);
    righteye.setPixelColor(129, color);
    righteye.setPixelColor(130, color);
    righteye.setPixelColor(131, color);
    righteye.setPixelColor(132, color);
    righteye.setPixelColor(133, color);
    righteye.setPixelColor(134, color);
    righteye.setPixelColor(135, color);
    righteye.setPixelColor(136, color);
    righteye.setPixelColor(137, color);
    righteye.setPixelColor(138, color);
    righteye.setPixelColor(139, color);
    righteye.setPixelColor(140, color);
    righteye.setPixelColor(141, color);
    righteye.setPixelColor(142, color);
    righteye.setPixelColor(143, color);
    righteye.setPixelColor(145, color);
    righteye.setPixelColor(146, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(154, color);
    righteye.setPixelColor(155, color);
    righteye.setPixelColor(156, color);
    righteye.setPixelColor(163, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(174, color);
    righteye.setPixelColor(181, color);
    righteye.setPixelColor(182, color);
    righteye.setPixelColor(183, color);
    righteye.setPixelColor(190, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(240, color);
    righteye.setPixelColor(241, color);
    righteye.setPixelColor(242, color);
    righteye.setPixelColor(243, color);
    righteye.setPixelColor(244, color);
    righteye.setPixelColor(245, color);
    righteye.setPixelColor(246, color);
    righteye.setPixelColor(247, color);
    righteye.setPixelColor(248, color);
    righteye.setPixelColor(249, color);
    righteye.setPixelColor(250, color);
    righteye.setPixelColor(251, color);
    righteye.setPixelColor(252, color);
    righteye.setPixelColor(253, color);
    righteye.setPixelColor(254, color);
    righteye.setPixelColor(255, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(0, color);
    lefteye.setPixelColor(1, color);
    lefteye.setPixelColor(2, color);
    lefteye.setPixelColor(3, color);
    lefteye.setPixelColor(4, color);
    lefteye.setPixelColor(5, color);
    lefteye.setPixelColor(6, color);
    lefteye.setPixelColor(7, color);
    lefteye.setPixelColor(8, color);
    lefteye.setPixelColor(9, color);
    lefteye.setPixelColor(10, color);
    lefteye.setPixelColor(11, color);
    lefteye.setPixelColor(12, color);
    lefteye.setPixelColor(13, color);
    lefteye.setPixelColor(14, color);
    lefteye.setPixelColor(15, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(65, color);
    lefteye.setPixelColor(72, color);
    lefteye.setPixelColor(73, color);
    lefteye.setPixelColor(74, color);
    lefteye.setPixelColor(81, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(90, color);
    lefteye.setPixelColor(91, color);
    lefteye.setPixelColor(92, color);
    lefteye.setPixelColor(99, color);
    lefteye.setPixelColor(100, color);
    lefteye.setPixelColor(101, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(109, color);
    lefteye.setPixelColor(110, color);
    lefteye.setPixelColor(112, color);
    lefteye.setPixelColor(113, color);
    lefteye.setPixelColor(114, color);
    lefteye.setPixelColor(115, color);
    lefteye.setPixelColor(116, color);
    lefteye.setPixelColor(117, color);
    lefteye.setPixelColor(118, color);
    lefteye.setPixelColor(119, color);
    lefteye.setPixelColor(120, color);
    lefteye.setPixelColor(121, color);
    lefteye.setPixelColor(122, color);
    lefteye.setPixelColor(123, color);
    lefteye.setPixelColor(124, color);
    lefteye.setPixelColor(125, color);
    lefteye.setPixelColor(126, color);
    lefteye.setPixelColor(127, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(129, color);
    lefteye.setPixelColor(130, color);
    lefteye.setPixelColor(131, color);
    lefteye.setPixelColor(132, color);
    lefteye.setPixelColor(133, color);
    lefteye.setPixelColor(134, color);
    lefteye.setPixelColor(135, color);
    lefteye.setPixelColor(136, color);
    lefteye.setPixelColor(137, color);
    lefteye.setPixelColor(138, color);
    lefteye.setPixelColor(139, color);
    lefteye.setPixelColor(140, color);
    lefteye.setPixelColor(141, color);
    lefteye.setPixelColor(142, color);
    lefteye.setPixelColor(143, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(193, color);
    lefteye.setPixelColor(200, color);
    lefteye.setPixelColor(201, color);
    lefteye.setPixelColor(202, color);
    lefteye.setPixelColor(209, color);
    lefteye.setPixelColor(210, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(219, color);
    lefteye.setPixelColor(220, color);
    lefteye.setPixelColor(227, color);
    lefteye.setPixelColor(228, color);
    lefteye.setPixelColor(229, color);
    lefteye.setPixelColor(236, color);
    lefteye.setPixelColor(237, color);
    lefteye.setPixelColor(238, color);
    lefteye.setPixelColor(240, color);
    lefteye.setPixelColor(241, color);
    lefteye.setPixelColor(242, color);
    lefteye.setPixelColor(243, color);
    lefteye.setPixelColor(244, color);
    lefteye.setPixelColor(245, color);
    lefteye.setPixelColor(246, color);
    lefteye.setPixelColor(247, color);
    lefteye.setPixelColor(248, color);
    lefteye.setPixelColor(249, color);
    lefteye.setPixelColor(250, color);
    lefteye.setPixelColor(251, color);
    lefteye.setPixelColor(252, color);
    lefteye.setPixelColor(253, color);
    lefteye.setPixelColor(254, color);
    lefteye.setPixelColor(255, color);
    righteye.setPixelColor(0, color);
    righteye.setPixelColor(1, color);
    righteye.setPixelColor(2, color);
    righteye.setPixelColor(3, color);
    righteye.setPixelColor(4, color);
    righteye.setPixelColor(5, color);
    righteye.setPixelColor(6, color);
    righteye.setPixelColor(7, color);
    righteye.setPixelColor(8, color);
    righteye.setPixelColor(9, color);
    righteye.setPixelColor(10, color);
    righteye.setPixelColor(11, color);
    righteye.setPixelColor(12, color);
    righteye.setPixelColor(13, color);
    righteye.setPixelColor(14, color);
    righteye.setPixelColor(15, color);
    righteye.setPixelColor(17, color);
    righteye.setPixelColor(18, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(26, color);
    righteye.setPixelColor(27, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(35, color);
    righteye.setPixelColor(36, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(44, color);
    righteye.setPixelColor(45, color);
    righteye.setPixelColor(46, color);
    righteye.setPixelColor(53, color);
    righteye.setPixelColor(54, color);
    righteye.setPixelColor(55, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(112, color);
    righteye.setPixelColor(113, color);
    righteye.setPixelColor(114, color);
    righteye.setPixelColor(115, color);
    righteye.setPixelColor(116, color);
    righteye.setPixelColor(117, color);
    righteye.setPixelColor(118, color);
    righteye.setPixelColor(119, color);
    righteye.setPixelColor(120, color);
    righteye.setPixelColor(121, color);
    righteye.setPixelColor(122, color);
    righteye.setPixelColor(123, color);
    righteye.setPixelColor(124, color);
    righteye.setPixelColor(125, color);
    righteye.setPixelColor(126, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(128, color);
    righteye.setPixelColor(129, color);
    righteye.setPixelColor(130, color);
    righteye.setPixelColor(131, color);
    righteye.setPixelColor(132, color);
    righteye.setPixelColor(133, color);
    righteye.setPixelColor(134, color);
    righteye.setPixelColor(135, color);
    righteye.setPixelColor(136, color);
    righteye.setPixelColor(137, color);
    righteye.setPixelColor(138, color);
    righteye.setPixelColor(139, color);
    righteye.setPixelColor(140, color);
    righteye.setPixelColor(141, color);
    righteye.setPixelColor(142, color);
    righteye.setPixelColor(143, color);
    righteye.setPixelColor(145, color);
    righteye.setPixelColor(146, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(154, color);
    righteye.setPixelColor(155, color);
    righteye.setPixelColor(156, color);
    righteye.setPixelColor(163, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(174, color);
    righteye.setPixelColor(181, color);
    righteye.setPixelColor(182, color);
    righteye.setPixelColor(183, color);
    righteye.setPixelColor(190, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(240, color);
    righteye.setPixelColor(241, color);
    righteye.setPixelColor(242, color);
    righteye.setPixelColor(243, color);
    righteye.setPixelColor(244, color);
    righteye.setPixelColor(245, color);
    righteye.setPixelColor(246, color);
    righteye.setPixelColor(247, color);
    righteye.setPixelColor(248, color);
    righteye.setPixelColor(249, color);
    righteye.setPixelColor(250, color);
    righteye.setPixelColor(251, color);
    righteye.setPixelColor(252, color);
    righteye.setPixelColor(253, color);
    righteye.setPixelColor(254, color);
    righteye.setPixelColor(255, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face009BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face009 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face009BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face010
void Face010()
{
  if (Face010RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(3, color);
    lefteye.setPixelColor(4, color);
    lefteye.setPixelColor(11, color);
    lefteye.setPixelColor(12, color);
    lefteye.setPixelColor(19, color);
    lefteye.setPixelColor(20, color);
    lefteye.setPixelColor(24, color);
    lefteye.setPixelColor(25, color);
    lefteye.setPixelColor(26, color);
    lefteye.setPixelColor(27, color);
    lefteye.setPixelColor(28, color);
    lefteye.setPixelColor(29, color);
    lefteye.setPixelColor(30, color);
    lefteye.setPixelColor(31, color);
    lefteye.setPixelColor(32, color);
    lefteye.setPixelColor(33, color);
    lefteye.setPixelColor(34, color);
    lefteye.setPixelColor(35, color);
    lefteye.setPixelColor(36, color);
    lefteye.setPixelColor(37, color);
    lefteye.setPixelColor(38, color);
    lefteye.setPixelColor(39, color);
    lefteye.setPixelColor(43, color);
    lefteye.setPixelColor(44, color);
    lefteye.setPixelColor(51, color);
    lefteye.setPixelColor(52, color);
    lefteye.setPixelColor(59, color);
    lefteye.setPixelColor(60, color);
    lefteye.setPixelColor(67, color);
    lefteye.setPixelColor(68, color);
    lefteye.setPixelColor(75, color);
    lefteye.setPixelColor(76, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(84, color);
    lefteye.setPixelColor(88, color);
    lefteye.setPixelColor(89, color);
    lefteye.setPixelColor(90, color);
    lefteye.setPixelColor(91, color);
    lefteye.setPixelColor(92, color);
    lefteye.setPixelColor(93, color);
    lefteye.setPixelColor(94, color);
    lefteye.setPixelColor(95, color);
    lefteye.setPixelColor(96, color);
    lefteye.setPixelColor(97, color);
    lefteye.setPixelColor(98, color);
    lefteye.setPixelColor(99, color);
    lefteye.setPixelColor(100, color);
    lefteye.setPixelColor(101, color);
    lefteye.setPixelColor(102, color);
    lefteye.setPixelColor(103, color);
    lefteye.setPixelColor(107, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(115, color);
    lefteye.setPixelColor(116, color);
    lefteye.setPixelColor(123, color);
    lefteye.setPixelColor(124, color);
    lefteye.setPixelColor(131, color);
    lefteye.setPixelColor(132, color);
    lefteye.setPixelColor(139, color);
    lefteye.setPixelColor(140, color);
    lefteye.setPixelColor(147, color);
    lefteye.setPixelColor(148, color);
    lefteye.setPixelColor(152, color);
    lefteye.setPixelColor(153, color);
    lefteye.setPixelColor(154, color);
    lefteye.setPixelColor(155, color);
    lefteye.setPixelColor(156, color);
    lefteye.setPixelColor(157, color);
    lefteye.setPixelColor(158, color);
    lefteye.setPixelColor(159, color);
    lefteye.setPixelColor(160, color);
    lefteye.setPixelColor(161, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(163, color);
    lefteye.setPixelColor(164, color);
    lefteye.setPixelColor(165, color);
    lefteye.setPixelColor(166, color);
    lefteye.setPixelColor(167, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(172, color);
    lefteye.setPixelColor(179, color);
    lefteye.setPixelColor(180, color);
    lefteye.setPixelColor(187, color);
    lefteye.setPixelColor(188, color);
    lefteye.setPixelColor(195, color);
    lefteye.setPixelColor(196, color);
    lefteye.setPixelColor(203, color);
    lefteye.setPixelColor(204, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(212, color);
    lefteye.setPixelColor(216, color);
    lefteye.setPixelColor(217, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(219, color);
    lefteye.setPixelColor(220, color);
    lefteye.setPixelColor(221, color);
    lefteye.setPixelColor(222, color);
    lefteye.setPixelColor(223, color);
    lefteye.setPixelColor(224, color);
    lefteye.setPixelColor(225, color);
    lefteye.setPixelColor(226, color);
    lefteye.setPixelColor(227, color);
    lefteye.setPixelColor(228, color);
    lefteye.setPixelColor(229, color);
    lefteye.setPixelColor(230, color);
    lefteye.setPixelColor(231, color);
    lefteye.setPixelColor(235, color);
    lefteye.setPixelColor(236, color);
    lefteye.setPixelColor(243, color);
    lefteye.setPixelColor(244, color);
    lefteye.setPixelColor(251, color);
    lefteye.setPixelColor(252, color);
    righteye.setPixelColor(3, color);
    righteye.setPixelColor(4, color);
    righteye.setPixelColor(11, color);
    righteye.setPixelColor(12, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(20, color);
    righteye.setPixelColor(24, color);
    righteye.setPixelColor(25, color);
    righteye.setPixelColor(26, color);
    righteye.setPixelColor(27, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(29, color);
    righteye.setPixelColor(30, color);
    righteye.setPixelColor(31, color);
    righteye.setPixelColor(32, color);
    righteye.setPixelColor(33, color);
    righteye.setPixelColor(34, color);
    righteye.setPixelColor(35, color);
    righteye.setPixelColor(36, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(38, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(43, color);
    righteye.setPixelColor(44, color);
    righteye.setPixelColor(51, color);
    righteye.setPixelColor(52, color);
    righteye.setPixelColor(59, color);
    righteye.setPixelColor(60, color);
    righteye.setPixelColor(67, color);
    righteye.setPixelColor(68, color);
    righteye.setPixelColor(75, color);
    righteye.setPixelColor(76, color);
    righteye.setPixelColor(83, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(88, color);
    righteye.setPixelColor(89, color);
    righteye.setPixelColor(90, color);
    righteye.setPixelColor(91, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(94, color);
    righteye.setPixelColor(95, color);
    righteye.setPixelColor(96, color);
    righteye.setPixelColor(97, color);
    righteye.setPixelColor(98, color);
    righteye.setPixelColor(99, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(101, color);
    righteye.setPixelColor(102, color);
    righteye.setPixelColor(103, color);
    righteye.setPixelColor(107, color);
    righteye.setPixelColor(108, color);
    righteye.setPixelColor(115, color);
    righteye.setPixelColor(116, color);
    righteye.setPixelColor(123, color);
    righteye.setPixelColor(124, color);
    righteye.setPixelColor(131, color);
    righteye.setPixelColor(132, color);
    righteye.setPixelColor(139, color);
    righteye.setPixelColor(140, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(148, color);
    righteye.setPixelColor(152, color);
    righteye.setPixelColor(153, color);
    righteye.setPixelColor(154, color);
    righteye.setPixelColor(155, color);
    righteye.setPixelColor(156, color);
    righteye.setPixelColor(157, color);
    righteye.setPixelColor(158, color);
    righteye.setPixelColor(159, color);
    righteye.setPixelColor(160, color);
    righteye.setPixelColor(161, color);
    righteye.setPixelColor(162, color);
    righteye.setPixelColor(163, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(166, color);
    righteye.setPixelColor(167, color);
    righteye.setPixelColor(171, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(179, color);
    righteye.setPixelColor(180, color);
    righteye.setPixelColor(187, color);
    righteye.setPixelColor(188, color);
    righteye.setPixelColor(195, color);
    righteye.setPixelColor(196, color);
    righteye.setPixelColor(203, color);
    righteye.setPixelColor(204, color);
    righteye.setPixelColor(211, color);
    righteye.setPixelColor(212, color);
    righteye.setPixelColor(216, color);
    righteye.setPixelColor(217, color);
    righteye.setPixelColor(218, color);
    righteye.setPixelColor(219, color);
    righteye.setPixelColor(220, color);
    righteye.setPixelColor(221, color);
    righteye.setPixelColor(222, color);
    righteye.setPixelColor(223, color);
    righteye.setPixelColor(224, color);
    righteye.setPixelColor(225, color);
    righteye.setPixelColor(226, color);
    righteye.setPixelColor(227, color);
    righteye.setPixelColor(228, color);
    righteye.setPixelColor(229, color);
    righteye.setPixelColor(230, color);
    righteye.setPixelColor(231, color);
    righteye.setPixelColor(235, color);
    righteye.setPixelColor(236, color);
    righteye.setPixelColor(243, color);
    righteye.setPixelColor(244, color);
    righteye.setPixelColor(251, color);
    righteye.setPixelColor(252, color);
    righteye.show();
    lefteye.show();
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
        lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(3, color);
    lefteye.setPixelColor(4, color);
    lefteye.setPixelColor(11, color);
    lefteye.setPixelColor(12, color);
    lefteye.setPixelColor(19, color);
    lefteye.setPixelColor(20, color);
    lefteye.setPixelColor(24, color);
    lefteye.setPixelColor(25, color);
    lefteye.setPixelColor(26, color);
    lefteye.setPixelColor(27, color);
    lefteye.setPixelColor(28, color);
    lefteye.setPixelColor(29, color);
    lefteye.setPixelColor(30, color);
    lefteye.setPixelColor(31, color);
    lefteye.setPixelColor(32, color);
    lefteye.setPixelColor(33, color);
    lefteye.setPixelColor(34, color);
    lefteye.setPixelColor(35, color);
    lefteye.setPixelColor(36, color);
    lefteye.setPixelColor(37, color);
    lefteye.setPixelColor(38, color);
    lefteye.setPixelColor(39, color);
    lefteye.setPixelColor(43, color);
    lefteye.setPixelColor(44, color);
    lefteye.setPixelColor(51, color);
    lefteye.setPixelColor(52, color);
    lefteye.setPixelColor(59, color);
    lefteye.setPixelColor(60, color);
    lefteye.setPixelColor(67, color);
    lefteye.setPixelColor(68, color);
    lefteye.setPixelColor(75, color);
    lefteye.setPixelColor(76, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(84, color);
    lefteye.setPixelColor(88, color);
    lefteye.setPixelColor(89, color);
    lefteye.setPixelColor(90, color);
    lefteye.setPixelColor(91, color);
    lefteye.setPixelColor(92, color);
    lefteye.setPixelColor(93, color);
    lefteye.setPixelColor(94, color);
    lefteye.setPixelColor(95, color);
    lefteye.setPixelColor(96, color);
    lefteye.setPixelColor(97, color);
    lefteye.setPixelColor(98, color);
    lefteye.setPixelColor(99, color);
    lefteye.setPixelColor(100, color);
    lefteye.setPixelColor(101, color);
    lefteye.setPixelColor(102, color);
    lefteye.setPixelColor(103, color);
    lefteye.setPixelColor(107, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(115, color);
    lefteye.setPixelColor(116, color);
    lefteye.setPixelColor(123, color);
    lefteye.setPixelColor(124, color);
    lefteye.setPixelColor(131, color);
    lefteye.setPixelColor(132, color);
    lefteye.setPixelColor(139, color);
    lefteye.setPixelColor(140, color);
    lefteye.setPixelColor(147, color);
    lefteye.setPixelColor(148, color);
    lefteye.setPixelColor(152, color);
    lefteye.setPixelColor(153, color);
    lefteye.setPixelColor(154, color);
    lefteye.setPixelColor(155, color);
    lefteye.setPixelColor(156, color);
    lefteye.setPixelColor(157, color);
    lefteye.setPixelColor(158, color);
    lefteye.setPixelColor(159, color);
    lefteye.setPixelColor(160, color);
    lefteye.setPixelColor(161, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(163, color);
    lefteye.setPixelColor(164, color);
    lefteye.setPixelColor(165, color);
    lefteye.setPixelColor(166, color);
    lefteye.setPixelColor(167, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(172, color);
    lefteye.setPixelColor(179, color);
    lefteye.setPixelColor(180, color);
    lefteye.setPixelColor(187, color);
    lefteye.setPixelColor(188, color);
    lefteye.setPixelColor(195, color);
    lefteye.setPixelColor(196, color);
    lefteye.setPixelColor(203, color);
    lefteye.setPixelColor(204, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(212, color);
    lefteye.setPixelColor(216, color);
    lefteye.setPixelColor(217, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(219, color);
    lefteye.setPixelColor(220, color);
    lefteye.setPixelColor(221, color);
    lefteye.setPixelColor(222, color);
    lefteye.setPixelColor(223, color);
    lefteye.setPixelColor(224, color);
    lefteye.setPixelColor(225, color);
    lefteye.setPixelColor(226, color);
    lefteye.setPixelColor(227, color);
    lefteye.setPixelColor(228, color);
    lefteye.setPixelColor(229, color);
    lefteye.setPixelColor(230, color);
    lefteye.setPixelColor(231, color);
    lefteye.setPixelColor(235, color);
    lefteye.setPixelColor(236, color);
    lefteye.setPixelColor(243, color);
    lefteye.setPixelColor(244, color);
    lefteye.setPixelColor(251, color);
    lefteye.setPixelColor(252, color);
    righteye.setPixelColor(3, color);
    righteye.setPixelColor(4, color);
    righteye.setPixelColor(11, color);
    righteye.setPixelColor(12, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(20, color);
    righteye.setPixelColor(24, color);
    righteye.setPixelColor(25, color);
    righteye.setPixelColor(26, color);
    righteye.setPixelColor(27, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(29, color);
    righteye.setPixelColor(30, color);
    righteye.setPixelColor(31, color);
    righteye.setPixelColor(32, color);
    righteye.setPixelColor(33, color);
    righteye.setPixelColor(34, color);
    righteye.setPixelColor(35, color);
    righteye.setPixelColor(36, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(38, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(43, color);
    righteye.setPixelColor(44, color);
    righteye.setPixelColor(51, color);
    righteye.setPixelColor(52, color);
    righteye.setPixelColor(59, color);
    righteye.setPixelColor(60, color);
    righteye.setPixelColor(67, color);
    righteye.setPixelColor(68, color);
    righteye.setPixelColor(75, color);
    righteye.setPixelColor(76, color);
    righteye.setPixelColor(83, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(88, color);
    righteye.setPixelColor(89, color);
    righteye.setPixelColor(90, color);
    righteye.setPixelColor(91, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(94, color);
    righteye.setPixelColor(95, color);
    righteye.setPixelColor(96, color);
    righteye.setPixelColor(97, color);
    righteye.setPixelColor(98, color);
    righteye.setPixelColor(99, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(101, color);
    righteye.setPixelColor(102, color);
    righteye.setPixelColor(103, color);
    righteye.setPixelColor(107, color);
    righteye.setPixelColor(108, color);
    righteye.setPixelColor(115, color);
    righteye.setPixelColor(116, color);
    righteye.setPixelColor(123, color);
    righteye.setPixelColor(124, color);
    righteye.setPixelColor(131, color);
    righteye.setPixelColor(132, color);
    righteye.setPixelColor(139, color);
    righteye.setPixelColor(140, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(148, color);
    righteye.setPixelColor(152, color);
    righteye.setPixelColor(153, color);
    righteye.setPixelColor(154, color);
    righteye.setPixelColor(155, color);
    righteye.setPixelColor(156, color);
    righteye.setPixelColor(157, color);
    righteye.setPixelColor(158, color);
    righteye.setPixelColor(159, color);
    righteye.setPixelColor(160, color);
    righteye.setPixelColor(161, color);
    righteye.setPixelColor(162, color);
    righteye.setPixelColor(163, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(166, color);
    righteye.setPixelColor(167, color);
    righteye.setPixelColor(171, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(179, color);
    righteye.setPixelColor(180, color);
    righteye.setPixelColor(187, color);
    righteye.setPixelColor(188, color);
    righteye.setPixelColor(195, color);
    righteye.setPixelColor(196, color);
    righteye.setPixelColor(203, color);
    righteye.setPixelColor(204, color);
    righteye.setPixelColor(211, color);
    righteye.setPixelColor(212, color);
    righteye.setPixelColor(216, color);
    righteye.setPixelColor(217, color);
    righteye.setPixelColor(218, color);
    righteye.setPixelColor(219, color);
    righteye.setPixelColor(220, color);
    righteye.setPixelColor(221, color);
    righteye.setPixelColor(222, color);
    righteye.setPixelColor(223, color);
    righteye.setPixelColor(224, color);
    righteye.setPixelColor(225, color);
    righteye.setPixelColor(226, color);
    righteye.setPixelColor(227, color);
    righteye.setPixelColor(228, color);
    righteye.setPixelColor(229, color);
    righteye.setPixelColor(230, color);
    righteye.setPixelColor(231, color);
    righteye.setPixelColor(235, color);
    righteye.setPixelColor(236, color);
    righteye.setPixelColor(243, color);
    righteye.setPixelColor(244, color);
    righteye.setPixelColor(251, color);
    righteye.setPixelColor(252, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face010BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face010 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face010BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face011
void Face011()
{
  if (Face011RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(32, color);
    lefteye.setPixelColor(33, color);
    lefteye.setPixelColor(34, color);
    lefteye.setPixelColor(35, color);
    lefteye.setPixelColor(36, color);
    lefteye.setPixelColor(37, color);
    lefteye.setPixelColor(38, color);
    lefteye.setPixelColor(39, color);
    lefteye.setPixelColor(40, color);
    lefteye.setPixelColor(41, color);
    lefteye.setPixelColor(42, color);
    lefteye.setPixelColor(43, color);
    lefteye.setPixelColor(44, color);
    lefteye.setPixelColor(45, color);
    lefteye.setPixelColor(46, color);
    lefteye.setPixelColor(47, color);
    lefteye.setPixelColor(80, color);
    lefteye.setPixelColor(81, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(84, color);
    lefteye.setPixelColor(85, color);
    lefteye.setPixelColor(86, color);
    lefteye.setPixelColor(87, color);
    lefteye.setPixelColor(88, color);
    lefteye.setPixelColor(89, color);
    lefteye.setPixelColor(90, color);
    lefteye.setPixelColor(91, color);
    lefteye.setPixelColor(92, color);
    lefteye.setPixelColor(93, color);
    lefteye.setPixelColor(94, color);
    lefteye.setPixelColor(95, color);
    lefteye.setPixelColor(160, color);
    lefteye.setPixelColor(161, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(163, color);
    lefteye.setPixelColor(164, color);
    lefteye.setPixelColor(165, color);
    lefteye.setPixelColor(166, color);
    lefteye.setPixelColor(167, color);
    lefteye.setPixelColor(168, color);
    lefteye.setPixelColor(169, color);
    lefteye.setPixelColor(170, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(172, color);
    lefteye.setPixelColor(173, color);
    lefteye.setPixelColor(174, color);
    lefteye.setPixelColor(175, color);
    lefteye.setPixelColor(208, color);
    lefteye.setPixelColor(209, color);
    lefteye.setPixelColor(210, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(212, color);
    lefteye.setPixelColor(213, color);
    lefteye.setPixelColor(214, color);
    lefteye.setPixelColor(215, color);
    lefteye.setPixelColor(216, color);
    lefteye.setPixelColor(217, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(219, color);
    lefteye.setPixelColor(220, color);
    lefteye.setPixelColor(221, color);
    lefteye.setPixelColor(222, color);
    lefteye.setPixelColor(223, color);
    righteye.setPixelColor(32, color);
    righteye.setPixelColor(33, color);
    righteye.setPixelColor(34, color);
    righteye.setPixelColor(35, color);
    righteye.setPixelColor(36, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(38, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(40, color);
    righteye.setPixelColor(41, color);
    righteye.setPixelColor(42, color);
    righteye.setPixelColor(43, color);
    righteye.setPixelColor(44, color);
    righteye.setPixelColor(45, color);
    righteye.setPixelColor(46, color);
    righteye.setPixelColor(47, color);
    righteye.setPixelColor(80, color);
    righteye.setPixelColor(81, color);
    righteye.setPixelColor(82, color);
    righteye.setPixelColor(83, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(85, color);
    righteye.setPixelColor(86, color);
    righteye.setPixelColor(87, color);
    righteye.setPixelColor(88, color);
    righteye.setPixelColor(89, color);
    righteye.setPixelColor(90, color);
    righteye.setPixelColor(91, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(94, color);
    righteye.setPixelColor(95, color);
    righteye.setPixelColor(160, color);
    righteye.setPixelColor(161, color);
    righteye.setPixelColor(162, color);
    righteye.setPixelColor(163, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(166, color);
    righteye.setPixelColor(167, color);
    righteye.setPixelColor(168, color);
    righteye.setPixelColor(169, color);
    righteye.setPixelColor(170, color);
    righteye.setPixelColor(171, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(174, color);
    righteye.setPixelColor(175, color);
    righteye.setPixelColor(208, color);
    righteye.setPixelColor(209, color);
    righteye.setPixelColor(210, color);
    righteye.setPixelColor(211, color);
    righteye.setPixelColor(212, color);
    righteye.setPixelColor(213, color);
    righteye.setPixelColor(214, color);
    righteye.setPixelColor(215, color);
    righteye.setPixelColor(216, color);
    righteye.setPixelColor(217, color);
    righteye.setPixelColor(218, color);
    righteye.setPixelColor(219, color);
    righteye.setPixelColor(220, color);
    righteye.setPixelColor(221, color);
    righteye.setPixelColor(222, color);
    righteye.setPixelColor(223, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(32, color);
    lefteye.setPixelColor(33, color);
    lefteye.setPixelColor(34, color);
    lefteye.setPixelColor(35, color);
    lefteye.setPixelColor(36, color);
    lefteye.setPixelColor(37, color);
    lefteye.setPixelColor(38, color);
    lefteye.setPixelColor(39, color);
    lefteye.setPixelColor(40, color);
    lefteye.setPixelColor(41, color);
    lefteye.setPixelColor(42, color);
    lefteye.setPixelColor(43, color);
    lefteye.setPixelColor(44, color);
    lefteye.setPixelColor(45, color);
    lefteye.setPixelColor(46, color);
    lefteye.setPixelColor(47, color);
    lefteye.setPixelColor(80, color);
    lefteye.setPixelColor(81, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(84, color);
    lefteye.setPixelColor(85, color);
    lefteye.setPixelColor(86, color);
    lefteye.setPixelColor(87, color);
    lefteye.setPixelColor(88, color);
    lefteye.setPixelColor(89, color);
    lefteye.setPixelColor(90, color);
    lefteye.setPixelColor(91, color);
    lefteye.setPixelColor(92, color);
    lefteye.setPixelColor(93, color);
    lefteye.setPixelColor(94, color);
    lefteye.setPixelColor(95, color);
    lefteye.setPixelColor(160, color);
    lefteye.setPixelColor(161, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(163, color);
    lefteye.setPixelColor(164, color);
    lefteye.setPixelColor(165, color);
    lefteye.setPixelColor(166, color);
    lefteye.setPixelColor(167, color);
    lefteye.setPixelColor(168, color);
    lefteye.setPixelColor(169, color);
    lefteye.setPixelColor(170, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(172, color);
    lefteye.setPixelColor(173, color);
    lefteye.setPixelColor(174, color);
    lefteye.setPixelColor(175, color);
    lefteye.setPixelColor(208, color);
    lefteye.setPixelColor(209, color);
    lefteye.setPixelColor(210, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(212, color);
    lefteye.setPixelColor(213, color);
    lefteye.setPixelColor(214, color);
    lefteye.setPixelColor(215, color);
    lefteye.setPixelColor(216, color);
    lefteye.setPixelColor(217, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(219, color);
    lefteye.setPixelColor(220, color);
    lefteye.setPixelColor(221, color);
    lefteye.setPixelColor(222, color);
    lefteye.setPixelColor(223, color);
    righteye.setPixelColor(32, color);
    righteye.setPixelColor(33, color);
    righteye.setPixelColor(34, color);
    righteye.setPixelColor(35, color);
    righteye.setPixelColor(36, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(38, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(40, color);
    righteye.setPixelColor(41, color);
    righteye.setPixelColor(42, color);
    righteye.setPixelColor(43, color);
    righteye.setPixelColor(44, color);
    righteye.setPixelColor(45, color);
    righteye.setPixelColor(46, color);
    righteye.setPixelColor(47, color);
    righteye.setPixelColor(80, color);
    righteye.setPixelColor(81, color);
    righteye.setPixelColor(82, color);
    righteye.setPixelColor(83, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(85, color);
    righteye.setPixelColor(86, color);
    righteye.setPixelColor(87, color);
    righteye.setPixelColor(88, color);
    righteye.setPixelColor(89, color);
    righteye.setPixelColor(90, color);
    righteye.setPixelColor(91, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(94, color);
    righteye.setPixelColor(95, color);
    righteye.setPixelColor(160, color);
    righteye.setPixelColor(161, color);
    righteye.setPixelColor(162, color);
    righteye.setPixelColor(163, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(166, color);
    righteye.setPixelColor(167, color);
    righteye.setPixelColor(168, color);
    righteye.setPixelColor(169, color);
    righteye.setPixelColor(170, color);
    righteye.setPixelColor(171, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(174, color);
    righteye.setPixelColor(175, color);
    righteye.setPixelColor(208, color);
    righteye.setPixelColor(209, color);
    righteye.setPixelColor(210, color);
    righteye.setPixelColor(211, color);
    righteye.setPixelColor(212, color);
    righteye.setPixelColor(213, color);
    righteye.setPixelColor(214, color);
    righteye.setPixelColor(215, color);
    righteye.setPixelColor(216, color);
    righteye.setPixelColor(217, color);
    righteye.setPixelColor(218, color);
    righteye.setPixelColor(219, color);
    righteye.setPixelColor(220, color);
    righteye.setPixelColor(221, color);
    righteye.setPixelColor(222, color);
    righteye.setPixelColor(223, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face011BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face011 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face011BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face012
void Face012()
{
  if (Face012RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(41, color);
    lefteye.setPixelColor(42, color);
    lefteye.setPixelColor(43, color);
    lefteye.setPixelColor(44, color);
    lefteye.setPixelColor(45, color);
    lefteye.setPixelColor(48, color);
    lefteye.setPixelColor(49, color);
    lefteye.setPixelColor(50, color);
    lefteye.setPixelColor(51, color);
    lefteye.setPixelColor(52, color);
    lefteye.setPixelColor(53, color);
    lefteye.setPixelColor(54, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(57, color);
    lefteye.setPixelColor(58, color);
    lefteye.setPixelColor(60, color);
    lefteye.setPixelColor(61, color);
    lefteye.setPixelColor(62, color);
    lefteye.setPixelColor(63, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(70, color);
    lefteye.setPixelColor(71, color);
    lefteye.setPixelColor(169, color);
    lefteye.setPixelColor(170, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(172, color);
    lefteye.setPixelColor(173, color);
    lefteye.setPixelColor(176, color);
    lefteye.setPixelColor(177, color);
    lefteye.setPixelColor(178, color);
    lefteye.setPixelColor(179, color);
    lefteye.setPixelColor(180, color);
    lefteye.setPixelColor(181, color);
    lefteye.setPixelColor(182, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(185, color);
    lefteye.setPixelColor(186, color);
    lefteye.setPixelColor(188, color);
    lefteye.setPixelColor(189, color);
    lefteye.setPixelColor(190, color);
    lefteye.setPixelColor(191, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(198, color);
    lefteye.setPixelColor(199, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(47, color);
    righteye.setPixelColor(54, color);
    righteye.setPixelColor(55, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(69, color);
    righteye.setPixelColor(70, color);
    righteye.setPixelColor(77, color);
    righteye.setPixelColor(78, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(85, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(181, color);
    righteye.setPixelColor(182, color);
    righteye.setPixelColor(189, color);
    righteye.setPixelColor(190, color);
    righteye.setPixelColor(198, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(206, color);
    righteye.setPixelColor(207, color);
    righteye.setPixelColor(215, color);
    righteye.setPixelColor(223, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(41, color);
    lefteye.setPixelColor(42, color);
    lefteye.setPixelColor(43, color);
    lefteye.setPixelColor(44, color);
    lefteye.setPixelColor(45, color);
    lefteye.setPixelColor(48, color);
    lefteye.setPixelColor(49, color);
    lefteye.setPixelColor(50, color);
    lefteye.setPixelColor(51, color);
    lefteye.setPixelColor(52, color);
    lefteye.setPixelColor(53, color);
    lefteye.setPixelColor(54, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(57, color);
    lefteye.setPixelColor(58, color);
    lefteye.setPixelColor(60, color);
    lefteye.setPixelColor(61, color);
    lefteye.setPixelColor(62, color);
    lefteye.setPixelColor(63, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(70, color);
    lefteye.setPixelColor(71, color);
    lefteye.setPixelColor(169, color);
    lefteye.setPixelColor(170, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(172, color);
    lefteye.setPixelColor(173, color);
    lefteye.setPixelColor(176, color);
    lefteye.setPixelColor(177, color);
    lefteye.setPixelColor(178, color);
    lefteye.setPixelColor(179, color);
    lefteye.setPixelColor(180, color);
    lefteye.setPixelColor(181, color);
    lefteye.setPixelColor(182, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(185, color);
    lefteye.setPixelColor(186, color);
    lefteye.setPixelColor(188, color);
    lefteye.setPixelColor(189, color);
    lefteye.setPixelColor(190, color);
    lefteye.setPixelColor(191, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(198, color);
    lefteye.setPixelColor(199, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(47, color);
    righteye.setPixelColor(54, color);
    righteye.setPixelColor(55, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(69, color);
    righteye.setPixelColor(70, color);
    righteye.setPixelColor(77, color);
    righteye.setPixelColor(78, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(85, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(181, color);
    righteye.setPixelColor(182, color);
    righteye.setPixelColor(189, color);
    righteye.setPixelColor(190, color);
    righteye.setPixelColor(198, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(206, color);
    righteye.setPixelColor(207, color);
    righteye.setPixelColor(215, color);
    righteye.setPixelColor(223, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face012BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face012 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face012BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face013
void Face013()
{
  if (Face013RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(0, color);
    lefteye.setPixelColor(1, color);
    lefteye.setPixelColor(2, color);
    lefteye.setPixelColor(3, color);
    lefteye.setPixelColor(4, color);
    lefteye.setPixelColor(5, color);
    lefteye.setPixelColor(8, color);
    lefteye.setPixelColor(9, color);
    lefteye.setPixelColor(10, color);
    lefteye.setPixelColor(11, color);
    lefteye.setPixelColor(12, color);
    lefteye.setPixelColor(13, color);
    lefteye.setPixelColor(14, color);
    lefteye.setPixelColor(21, color);
    lefteye.setPixelColor(22, color);
    lefteye.setPixelColor(23, color);
    lefteye.setPixelColor(30, color);
    lefteye.setPixelColor(31, color);
    lefteye.setPixelColor(38, color);
    lefteye.setPixelColor(39, color);
    lefteye.setPixelColor(46, color);
    lefteye.setPixelColor(47, color);
    lefteye.setPixelColor(53, color);
    lefteye.setPixelColor(54, color);
    lefteye.setPixelColor(55, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(57, color);
    lefteye.setPixelColor(58, color);
    lefteye.setPixelColor(59, color);
    lefteye.setPixelColor(60, color);
    lefteye.setPixelColor(61, color);
    lefteye.setPixelColor(62, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(65, color);
    lefteye.setPixelColor(66, color);
    lefteye.setPixelColor(67, color);
    lefteye.setPixelColor(68, color);
    lefteye.setPixelColor(69, color);
    lefteye.setPixelColor(86, color);
    lefteye.setPixelColor(87, color);
    lefteye.setPixelColor(94, color);
    lefteye.setPixelColor(95, color);
    lefteye.setPixelColor(101, color);
    lefteye.setPixelColor(102, color);
    lefteye.setPixelColor(103, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(109, color);
    lefteye.setPixelColor(110, color);
    lefteye.setPixelColor(112, color);
    lefteye.setPixelColor(113, color);
    lefteye.setPixelColor(114, color);
    lefteye.setPixelColor(115, color);
    lefteye.setPixelColor(116, color);
    lefteye.setPixelColor(117, color);
    lefteye.setPixelColor(120, color);
    lefteye.setPixelColor(121, color);
    lefteye.setPixelColor(122, color);
    lefteye.setPixelColor(123, color);
    lefteye.setPixelColor(124, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(129, color);
    lefteye.setPixelColor(130, color);
    lefteye.setPixelColor(131, color);
    lefteye.setPixelColor(132, color);
    lefteye.setPixelColor(136, color);
    lefteye.setPixelColor(137, color);
    lefteye.setPixelColor(138, color);
    lefteye.setPixelColor(139, color);
    lefteye.setPixelColor(140, color);
    lefteye.setPixelColor(141, color);
    lefteye.setPixelColor(148, color);
    lefteye.setPixelColor(149, color);
    lefteye.setPixelColor(150, color);
    lefteye.setPixelColor(157, color);
    lefteye.setPixelColor(158, color);
    lefteye.setPixelColor(159, color);
    lefteye.setPixelColor(166, color);
    lefteye.setPixelColor(167, color);
    lefteye.setPixelColor(174, color);
    lefteye.setPixelColor(175, color);
    lefteye.setPixelColor(182, color);
    lefteye.setPixelColor(183, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(185, color);
    lefteye.setPixelColor(186, color);
    lefteye.setPixelColor(187, color);
    lefteye.setPixelColor(188, color);
    lefteye.setPixelColor(189, color);
    lefteye.setPixelColor(190, color);
    lefteye.setPixelColor(191, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(193, color);
    lefteye.setPixelColor(194, color);
    lefteye.setPixelColor(195, color);
    lefteye.setPixelColor(196, color);
    lefteye.setPixelColor(197, color);
    lefteye.setPixelColor(198, color);
    lefteye.setPixelColor(199, color);
    lefteye.setPixelColor(205, color);
    lefteye.setPixelColor(206, color);
    lefteye.setPixelColor(207, color);
    lefteye.setPixelColor(214, color);
    lefteye.setPixelColor(215, color);
    lefteye.setPixelColor(222, color);
    lefteye.setPixelColor(223, color);
    lefteye.setPixelColor(230, color);
    lefteye.setPixelColor(231, color);
    lefteye.setPixelColor(237, color);
    lefteye.setPixelColor(238, color);
    lefteye.setPixelColor(239, color);
    lefteye.setPixelColor(240, color);
    lefteye.setPixelColor(241, color);
    lefteye.setPixelColor(242, color);
    lefteye.setPixelColor(243, color);
    lefteye.setPixelColor(244, color);
    lefteye.setPixelColor(245, color);
    lefteye.setPixelColor(246, color);
    lefteye.setPixelColor(248, color);
    lefteye.setPixelColor(249, color);
    lefteye.setPixelColor(250, color);
    lefteye.setPixelColor(251, color);
    lefteye.setPixelColor(252, color);
    lefteye.setPixelColor(253, color);
    righteye.setPixelColor(2, color);
    righteye.setPixelColor(3, color);
    righteye.setPixelColor(4, color);
    righteye.setPixelColor(5, color);
    righteye.setPixelColor(6, color);
    righteye.setPixelColor(7, color);
    righteye.setPixelColor(9, color);
    righteye.setPixelColor(10, color);
    righteye.setPixelColor(11, color);
    righteye.setPixelColor(12, color);
    righteye.setPixelColor(13, color);
    righteye.setPixelColor(14, color);
    righteye.setPixelColor(15, color);
    righteye.setPixelColor(16, color);
    righteye.setPixelColor(17, color);
    righteye.setPixelColor(18, color);
    righteye.setPixelColor(24, color);
    righteye.setPixelColor(25, color);
    righteye.setPixelColor(32, color);
    righteye.setPixelColor(33, color);
    righteye.setPixelColor(40, color);
    righteye.setPixelColor(41, color);
    righteye.setPixelColor(48, color);
    righteye.setPixelColor(49, color);
    righteye.setPixelColor(50, color);
    righteye.setPixelColor(56, color);
    righteye.setPixelColor(57, color);
    righteye.setPixelColor(58, color);
    righteye.setPixelColor(59, color);
    righteye.setPixelColor(60, color);
    righteye.setPixelColor(61, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(64, color);
    righteye.setPixelColor(65, color);
    righteye.setPixelColor(66, color);
    righteye.setPixelColor(67, color);
    righteye.setPixelColor(68, color);
    righteye.setPixelColor(69, color);
    righteye.setPixelColor(70, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(72, color);
    righteye.setPixelColor(73, color);
    righteye.setPixelColor(80, color);
    righteye.setPixelColor(81, color);
    righteye.setPixelColor(88, color);
    righteye.setPixelColor(89, color);
    righteye.setPixelColor(96, color);
    righteye.setPixelColor(97, color);
    righteye.setPixelColor(98, color);
    righteye.setPixelColor(105, color);
    righteye.setPixelColor(106, color);
    righteye.setPixelColor(107, color);
    righteye.setPixelColor(114, color);
    righteye.setPixelColor(115, color);
    righteye.setPixelColor(116, color);
    righteye.setPixelColor(117, color);
    righteye.setPixelColor(118, color);
    righteye.setPixelColor(119, color);
    righteye.setPixelColor(123, color);
    righteye.setPixelColor(124, color);
    righteye.setPixelColor(125, color);
    righteye.setPixelColor(126, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(131, color);
    righteye.setPixelColor(132, color);
    righteye.setPixelColor(133, color);
    righteye.setPixelColor(134, color);
    righteye.setPixelColor(135, color);
    righteye.setPixelColor(138, color);
    righteye.setPixelColor(139, color);
    righteye.setPixelColor(140, color);
    righteye.setPixelColor(141, color);
    righteye.setPixelColor(142, color);
    righteye.setPixelColor(143, color);
    righteye.setPixelColor(145, color);
    righteye.setPixelColor(146, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(152, color);
    righteye.setPixelColor(153, color);
    righteye.setPixelColor(154, color);
    righteye.setPixelColor(160, color);
    righteye.setPixelColor(161, color);
    righteye.setPixelColor(168, color);
    righteye.setPixelColor(169, color);
    righteye.setPixelColor(186, color);
    righteye.setPixelColor(187, color);
    righteye.setPixelColor(188, color);
    righteye.setPixelColor(189, color);
    righteye.setPixelColor(190, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(193, color);
    righteye.setPixelColor(194, color);
    righteye.setPixelColor(195, color);
    righteye.setPixelColor(196, color);
    righteye.setPixelColor(197, color);
    righteye.setPixelColor(198, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(200, color);
    righteye.setPixelColor(201, color);
    righteye.setPixelColor(202, color);
    righteye.setPixelColor(208, color);
    righteye.setPixelColor(209, color);
    righteye.setPixelColor(216, color);
    righteye.setPixelColor(217, color);
    righteye.setPixelColor(224, color);
    righteye.setPixelColor(225, color);
    righteye.setPixelColor(232, color);
    righteye.setPixelColor(233, color);
    righteye.setPixelColor(234, color);
    righteye.setPixelColor(241, color);
    righteye.setPixelColor(242, color);
    righteye.setPixelColor(243, color);
    righteye.setPixelColor(244, color);
    righteye.setPixelColor(245, color);
    righteye.setPixelColor(246, color);
    righteye.setPixelColor(247, color);
    righteye.setPixelColor(250, color);
    righteye.setPixelColor(251, color);
    righteye.setPixelColor(252, color);
    righteye.setPixelColor(253, color);
    righteye.setPixelColor(254, color);
    righteye.setPixelColor(255, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(0, color);
    lefteye.setPixelColor(1, color);
    lefteye.setPixelColor(2, color);
    lefteye.setPixelColor(3, color);
    lefteye.setPixelColor(4, color);
    lefteye.setPixelColor(5, color);
    lefteye.setPixelColor(8, color);
    lefteye.setPixelColor(9, color);
    lefteye.setPixelColor(10, color);
    lefteye.setPixelColor(11, color);
    lefteye.setPixelColor(12, color);
    lefteye.setPixelColor(13, color);
    lefteye.setPixelColor(14, color);
    lefteye.setPixelColor(21, color);
    lefteye.setPixelColor(22, color);
    lefteye.setPixelColor(23, color);
    lefteye.setPixelColor(30, color);
    lefteye.setPixelColor(31, color);
    lefteye.setPixelColor(38, color);
    lefteye.setPixelColor(39, color);
    lefteye.setPixelColor(46, color);
    lefteye.setPixelColor(47, color);
    lefteye.setPixelColor(53, color);
    lefteye.setPixelColor(54, color);
    lefteye.setPixelColor(55, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(57, color);
    lefteye.setPixelColor(58, color);
    lefteye.setPixelColor(59, color);
    lefteye.setPixelColor(60, color);
    lefteye.setPixelColor(61, color);
    lefteye.setPixelColor(62, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(65, color);
    lefteye.setPixelColor(66, color);
    lefteye.setPixelColor(67, color);
    lefteye.setPixelColor(68, color);
    lefteye.setPixelColor(69, color);
    lefteye.setPixelColor(86, color);
    lefteye.setPixelColor(87, color);
    lefteye.setPixelColor(94, color);
    lefteye.setPixelColor(95, color);
    lefteye.setPixelColor(101, color);
    lefteye.setPixelColor(102, color);
    lefteye.setPixelColor(103, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(109, color);
    lefteye.setPixelColor(110, color);
    lefteye.setPixelColor(112, color);
    lefteye.setPixelColor(113, color);
    lefteye.setPixelColor(114, color);
    lefteye.setPixelColor(115, color);
    lefteye.setPixelColor(116, color);
    lefteye.setPixelColor(117, color);
    lefteye.setPixelColor(120, color);
    lefteye.setPixelColor(121, color);
    lefteye.setPixelColor(122, color);
    lefteye.setPixelColor(123, color);
    lefteye.setPixelColor(124, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(129, color);
    lefteye.setPixelColor(130, color);
    lefteye.setPixelColor(131, color);
    lefteye.setPixelColor(132, color);
    lefteye.setPixelColor(136, color);
    lefteye.setPixelColor(137, color);
    lefteye.setPixelColor(138, color);
    lefteye.setPixelColor(139, color);
    lefteye.setPixelColor(140, color);
    lefteye.setPixelColor(141, color);
    lefteye.setPixelColor(148, color);
    lefteye.setPixelColor(149, color);
    lefteye.setPixelColor(150, color);
    lefteye.setPixelColor(157, color);
    lefteye.setPixelColor(158, color);
    lefteye.setPixelColor(159, color);
    lefteye.setPixelColor(166, color);
    lefteye.setPixelColor(167, color);
    lefteye.setPixelColor(174, color);
    lefteye.setPixelColor(175, color);
    lefteye.setPixelColor(182, color);
    lefteye.setPixelColor(183, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(185, color);
    lefteye.setPixelColor(186, color);
    lefteye.setPixelColor(187, color);
    lefteye.setPixelColor(188, color);
    lefteye.setPixelColor(189, color);
    lefteye.setPixelColor(190, color);
    lefteye.setPixelColor(191, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(193, color);
    lefteye.setPixelColor(194, color);
    lefteye.setPixelColor(195, color);
    lefteye.setPixelColor(196, color);
    lefteye.setPixelColor(197, color);
    lefteye.setPixelColor(198, color);
    lefteye.setPixelColor(199, color);
    lefteye.setPixelColor(205, color);
    lefteye.setPixelColor(206, color);
    lefteye.setPixelColor(207, color);
    lefteye.setPixelColor(214, color);
    lefteye.setPixelColor(215, color);
    lefteye.setPixelColor(222, color);
    lefteye.setPixelColor(223, color);
    lefteye.setPixelColor(230, color);
    lefteye.setPixelColor(231, color);
    lefteye.setPixelColor(237, color);
    lefteye.setPixelColor(238, color);
    lefteye.setPixelColor(239, color);
    lefteye.setPixelColor(240, color);
    lefteye.setPixelColor(241, color);
    lefteye.setPixelColor(242, color);
    lefteye.setPixelColor(243, color);
    lefteye.setPixelColor(244, color);
    lefteye.setPixelColor(245, color);
    lefteye.setPixelColor(246, color);
    lefteye.setPixelColor(248, color);
    lefteye.setPixelColor(249, color);
    lefteye.setPixelColor(250, color);
    lefteye.setPixelColor(251, color);
    lefteye.setPixelColor(252, color);
    lefteye.setPixelColor(253, color);
    righteye.setPixelColor(2, color);
    righteye.setPixelColor(3, color);
    righteye.setPixelColor(4, color);
    righteye.setPixelColor(5, color);
    righteye.setPixelColor(6, color);
    righteye.setPixelColor(7, color);
    righteye.setPixelColor(9, color);
    righteye.setPixelColor(10, color);
    righteye.setPixelColor(11, color);
    righteye.setPixelColor(12, color);
    righteye.setPixelColor(13, color);
    righteye.setPixelColor(14, color);
    righteye.setPixelColor(15, color);
    righteye.setPixelColor(16, color);
    righteye.setPixelColor(17, color);
    righteye.setPixelColor(18, color);
    righteye.setPixelColor(24, color);
    righteye.setPixelColor(25, color);
    righteye.setPixelColor(32, color);
    righteye.setPixelColor(33, color);
    righteye.setPixelColor(40, color);
    righteye.setPixelColor(41, color);
    righteye.setPixelColor(48, color);
    righteye.setPixelColor(49, color);
    righteye.setPixelColor(50, color);
    righteye.setPixelColor(56, color);
    righteye.setPixelColor(57, color);
    righteye.setPixelColor(58, color);
    righteye.setPixelColor(59, color);
    righteye.setPixelColor(60, color);
    righteye.setPixelColor(61, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(64, color);
    righteye.setPixelColor(65, color);
    righteye.setPixelColor(66, color);
    righteye.setPixelColor(67, color);
    righteye.setPixelColor(68, color);
    righteye.setPixelColor(69, color);
    righteye.setPixelColor(70, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(72, color);
    righteye.setPixelColor(73, color);
    righteye.setPixelColor(80, color);
    righteye.setPixelColor(81, color);
    righteye.setPixelColor(88, color);
    righteye.setPixelColor(89, color);
    righteye.setPixelColor(96, color);
    righteye.setPixelColor(97, color);
    righteye.setPixelColor(98, color);
    righteye.setPixelColor(105, color);
    righteye.setPixelColor(106, color);
    righteye.setPixelColor(107, color);
    righteye.setPixelColor(114, color);
    righteye.setPixelColor(115, color);
    righteye.setPixelColor(116, color);
    righteye.setPixelColor(117, color);
    righteye.setPixelColor(118, color);
    righteye.setPixelColor(119, color);
    righteye.setPixelColor(123, color);
    righteye.setPixelColor(124, color);
    righteye.setPixelColor(125, color);
    righteye.setPixelColor(126, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(131, color);
    righteye.setPixelColor(132, color);
    righteye.setPixelColor(133, color);
    righteye.setPixelColor(134, color);
    righteye.setPixelColor(135, color);
    righteye.setPixelColor(138, color);
    righteye.setPixelColor(139, color);
    righteye.setPixelColor(140, color);
    righteye.setPixelColor(141, color);
    righteye.setPixelColor(142, color);
    righteye.setPixelColor(143, color);
    righteye.setPixelColor(145, color);
    righteye.setPixelColor(146, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(152, color);
    righteye.setPixelColor(153, color);
    righteye.setPixelColor(154, color);
    righteye.setPixelColor(160, color);
    righteye.setPixelColor(161, color);
    righteye.setPixelColor(168, color);
    righteye.setPixelColor(169, color);
    righteye.setPixelColor(186, color);
    righteye.setPixelColor(187, color);
    righteye.setPixelColor(188, color);
    righteye.setPixelColor(189, color);
    righteye.setPixelColor(190, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(193, color);
    righteye.setPixelColor(194, color);
    righteye.setPixelColor(195, color);
    righteye.setPixelColor(196, color);
    righteye.setPixelColor(197, color);
    righteye.setPixelColor(198, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(200, color);
    righteye.setPixelColor(201, color);
    righteye.setPixelColor(202, color);
    righteye.setPixelColor(208, color);
    righteye.setPixelColor(209, color);
    righteye.setPixelColor(216, color);
    righteye.setPixelColor(217, color);
    righteye.setPixelColor(224, color);
    righteye.setPixelColor(225, color);
    righteye.setPixelColor(232, color);
    righteye.setPixelColor(233, color);
    righteye.setPixelColor(234, color);
    righteye.setPixelColor(241, color);
    righteye.setPixelColor(242, color);
    righteye.setPixelColor(243, color);
    righteye.setPixelColor(244, color);
    righteye.setPixelColor(245, color);
    righteye.setPixelColor(246, color);
    righteye.setPixelColor(247, color);
    righteye.setPixelColor(250, color);
    righteye.setPixelColor(251, color);
    righteye.setPixelColor(252, color);
    righteye.setPixelColor(253, color);
    righteye.setPixelColor(254, color);
    righteye.setPixelColor(255, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face013BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face013 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show(); 
    }
    else
    {
    Face013BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face014
void Face014()
{
  if (Face014RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(0, color);
    lefteye.setPixelColor(8, color);
    lefteye.setPixelColor(16, color);
    lefteye.setPixelColor(24, color);
    lefteye.setPixelColor(32, color);
    lefteye.setPixelColor(40, color);
    lefteye.setPixelColor(48, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(72, color);
    lefteye.setPixelColor(80, color);
    lefteye.setPixelColor(88, color);
    lefteye.setPixelColor(112, color);
    lefteye.setPixelColor(120, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(136, color);
    lefteye.setPixelColor(160, color);
    lefteye.setPixelColor(168, color);
    lefteye.setPixelColor(176, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(200, color);
    lefteye.setPixelColor(208, color);
    lefteye.setPixelColor(216, color);
    lefteye.setPixelColor(224, color);
    lefteye.setPixelColor(232, color);
    lefteye.setPixelColor(240, color);
    lefteye.setPixelColor(248, color);
    righteye.setPixelColor(7, color);
    righteye.setPixelColor(15, color);
    righteye.setPixelColor(23, color);
    righteye.setPixelColor(31, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(47, color);
    righteye.setPixelColor(55, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(79, color);
    righteye.setPixelColor(87, color);
    righteye.setPixelColor(95, color);
    righteye.setPixelColor(119, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(135, color);
    righteye.setPixelColor(143, color);
    righteye.setPixelColor(167, color);
    righteye.setPixelColor(175, color);
    righteye.setPixelColor(183, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(207, color);
    righteye.setPixelColor(215, color);
    righteye.setPixelColor(223, color);
    righteye.setPixelColor(231, color);
    righteye.setPixelColor(239, color);
    righteye.setPixelColor(247, color);
    righteye.setPixelColor(255, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(0, color);
    lefteye.setPixelColor(8, color);
    lefteye.setPixelColor(16, color);
    lefteye.setPixelColor(24, color);
    lefteye.setPixelColor(32, color);
    lefteye.setPixelColor(40, color);
    lefteye.setPixelColor(48, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(72, color);
    lefteye.setPixelColor(80, color);
    lefteye.setPixelColor(88, color);
    lefteye.setPixelColor(112, color);
    lefteye.setPixelColor(120, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(136, color);
    lefteye.setPixelColor(160, color);
    lefteye.setPixelColor(168, color);
    lefteye.setPixelColor(176, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(200, color);
    lefteye.setPixelColor(208, color);
    lefteye.setPixelColor(216, color);
    lefteye.setPixelColor(224, color);
    lefteye.setPixelColor(232, color);
    lefteye.setPixelColor(240, color);
    lefteye.setPixelColor(248, color);
    righteye.setPixelColor(7, color);
    righteye.setPixelColor(15, color);
    righteye.setPixelColor(23, color);
    righteye.setPixelColor(31, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(47, color);
    righteye.setPixelColor(55, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(79, color);
    righteye.setPixelColor(87, color);
    righteye.setPixelColor(95, color);
    righteye.setPixelColor(119, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(135, color);
    righteye.setPixelColor(143, color);
    righteye.setPixelColor(167, color);
    righteye.setPixelColor(175, color);
    righteye.setPixelColor(183, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(207, color);
    righteye.setPixelColor(215, color);
    righteye.setPixelColor(223, color);
    righteye.setPixelColor(231, color);
    righteye.setPixelColor(239, color);
    righteye.setPixelColor(247, color);
    righteye.setPixelColor(255, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face014BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face014 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face014BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face015
void Face015()
{
  if (Face015RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(0, color);
    lefteye.setPixelColor(8, color);
    lefteye.setPixelColor(16, color);
    lefteye.setPixelColor(20, color);
    lefteye.setPixelColor(21, color);
    lefteye.setPixelColor(24, color);
    lefteye.setPixelColor(27, color);
    lefteye.setPixelColor(28, color);
    lefteye.setPixelColor(29, color);
    lefteye.setPixelColor(32, color);
    lefteye.setPixelColor(34, color);
    lefteye.setPixelColor(35, color);
    lefteye.setPixelColor(36, color);
    lefteye.setPixelColor(40, color);
    lefteye.setPixelColor(41, color);
    lefteye.setPixelColor(42, color);
    lefteye.setPixelColor(43, color);
    lefteye.setPixelColor(48, color);
    lefteye.setPixelColor(49, color);
    lefteye.setPixelColor(50, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(57, color);
    lefteye.setPixelColor(58, color);
    lefteye.setPixelColor(59, color);
    lefteye.setPixelColor(60, color);
    lefteye.setPixelColor(61, color);
    lefteye.setPixelColor(62, color);
    lefteye.setPixelColor(63, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(65, color);
    lefteye.setPixelColor(66, color);
    lefteye.setPixelColor(67, color);
    lefteye.setPixelColor(68, color);
    lefteye.setPixelColor(69, color);
    lefteye.setPixelColor(70, color);
    lefteye.setPixelColor(71, color);
    lefteye.setPixelColor(72, color);
    lefteye.setPixelColor(73, color);
    lefteye.setPixelColor(74, color);
    lefteye.setPixelColor(80, color);
    lefteye.setPixelColor(81, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(88, color);
    lefteye.setPixelColor(90, color);
    lefteye.setPixelColor(91, color);
    lefteye.setPixelColor(92, color);
    lefteye.setPixelColor(96, color);
    lefteye.setPixelColor(99, color);
    lefteye.setPixelColor(100, color);
    lefteye.setPixelColor(101, color);
    lefteye.setPixelColor(104, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(109, color);
    lefteye.setPixelColor(112, color);
    lefteye.setPixelColor(120, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(136, color);
    lefteye.setPixelColor(144, color);
    lefteye.setPixelColor(148, color);
    lefteye.setPixelColor(149, color);
    lefteye.setPixelColor(152, color);
    lefteye.setPixelColor(155, color);
    lefteye.setPixelColor(156, color);
    lefteye.setPixelColor(157, color);
    lefteye.setPixelColor(160, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(163, color);
    lefteye.setPixelColor(164, color);
    lefteye.setPixelColor(168, color);
    lefteye.setPixelColor(169, color);
    lefteye.setPixelColor(170, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(176, color);
    lefteye.setPixelColor(177, color);
    lefteye.setPixelColor(178, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(185, color);
    lefteye.setPixelColor(186, color);
    lefteye.setPixelColor(187, color);
    lefteye.setPixelColor(188, color);
    lefteye.setPixelColor(189, color);
    lefteye.setPixelColor(190, color);
    lefteye.setPixelColor(191, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(193, color);
    lefteye.setPixelColor(194, color);
    lefteye.setPixelColor(195, color);
    lefteye.setPixelColor(196, color);
    lefteye.setPixelColor(197, color);
    lefteye.setPixelColor(198, color);
    lefteye.setPixelColor(199, color);
    lefteye.setPixelColor(200, color);
    lefteye.setPixelColor(201, color);
    lefteye.setPixelColor(202, color);
    lefteye.setPixelColor(208, color);
    lefteye.setPixelColor(209, color);
    lefteye.setPixelColor(210, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(216, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(219, color);
    lefteye.setPixelColor(220, color);
    lefteye.setPixelColor(224, color);
    lefteye.setPixelColor(227, color);
    lefteye.setPixelColor(228, color);
    lefteye.setPixelColor(229, color);
    lefteye.setPixelColor(232, color);
    lefteye.setPixelColor(236, color);
    lefteye.setPixelColor(237, color);
    lefteye.setPixelColor(240, color);
    lefteye.setPixelColor(248, color);
    righteye.setPixelColor(7, color);
    righteye.setPixelColor(15, color);
    righteye.setPixelColor(18, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(23, color);
    righteye.setPixelColor(26, color);
    righteye.setPixelColor(27, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(31, color);
    righteye.setPixelColor(35, color);
    righteye.setPixelColor(36, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(44, color);
    righteye.setPixelColor(45, color);
    righteye.setPixelColor(46, color);
    righteye.setPixelColor(47, color);
    righteye.setPixelColor(53, color);
    righteye.setPixelColor(54, color);
    righteye.setPixelColor(55, color);
    righteye.setPixelColor(56, color);
    righteye.setPixelColor(57, color);
    righteye.setPixelColor(58, color);
    righteye.setPixelColor(59, color);
    righteye.setPixelColor(60, color);
    righteye.setPixelColor(61, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(64, color);
    righteye.setPixelColor(65, color);
    righteye.setPixelColor(66, color);
    righteye.setPixelColor(67, color);
    righteye.setPixelColor(68, color);
    righteye.setPixelColor(69, color);
    righteye.setPixelColor(70, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(77, color);
    righteye.setPixelColor(78, color);
    righteye.setPixelColor(79, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(85, color);
    righteye.setPixelColor(86, color);
    righteye.setPixelColor(87, color);
    righteye.setPixelColor(91, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(95, color);
    righteye.setPixelColor(98, color);
    righteye.setPixelColor(99, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(103, color);
    righteye.setPixelColor(106, color);
    righteye.setPixelColor(107, color);
    righteye.setPixelColor(111, color);
    righteye.setPixelColor(119, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(135, color);
    righteye.setPixelColor(143, color);
    righteye.setPixelColor(146, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(151, color);
    righteye.setPixelColor(154, color);
    righteye.setPixelColor(155, color);
    righteye.setPixelColor(156, color);
    righteye.setPixelColor(159, color);
    righteye.setPixelColor(163, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(167, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(174, color);
    righteye.setPixelColor(175, color);
    righteye.setPixelColor(181, color);
    righteye.setPixelColor(182, color);
    righteye.setPixelColor(183, color);
    righteye.setPixelColor(184, color);
    righteye.setPixelColor(185, color);
    righteye.setPixelColor(186, color);
    righteye.setPixelColor(187, color);
    righteye.setPixelColor(188, color);
    righteye.setPixelColor(189, color);
    righteye.setPixelColor(190, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(192, color);
    righteye.setPixelColor(193, color);
    righteye.setPixelColor(194, color);
    righteye.setPixelColor(195, color);
    righteye.setPixelColor(196, color);
    righteye.setPixelColor(197, color);
    righteye.setPixelColor(198, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(205, color);
    righteye.setPixelColor(206, color);
    righteye.setPixelColor(207, color);
    righteye.setPixelColor(212, color);
    righteye.setPixelColor(213, color);
    righteye.setPixelColor(214, color);
    righteye.setPixelColor(215, color);
    righteye.setPixelColor(219, color);
    righteye.setPixelColor(220, color);
    righteye.setPixelColor(221, color);
    righteye.setPixelColor(223, color);
    righteye.setPixelColor(226, color);
    righteye.setPixelColor(227, color);
    righteye.setPixelColor(228, color);
    righteye.setPixelColor(231, color);
    righteye.setPixelColor(234, color);
    righteye.setPixelColor(235, color);
    righteye.setPixelColor(239, color);
    righteye.setPixelColor(247, color);
    righteye.setPixelColor(255, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(0, color);
    lefteye.setPixelColor(8, color);
    lefteye.setPixelColor(16, color);
    lefteye.setPixelColor(20, color);
    lefteye.setPixelColor(21, color);
    lefteye.setPixelColor(24, color);
    lefteye.setPixelColor(27, color);
    lefteye.setPixelColor(28, color);
    lefteye.setPixelColor(29, color);
    lefteye.setPixelColor(32, color);
    lefteye.setPixelColor(34, color);
    lefteye.setPixelColor(35, color);
    lefteye.setPixelColor(36, color);
    lefteye.setPixelColor(40, color);
    lefteye.setPixelColor(41, color);
    lefteye.setPixelColor(42, color);
    lefteye.setPixelColor(43, color);
    lefteye.setPixelColor(48, color);
    lefteye.setPixelColor(49, color);
    lefteye.setPixelColor(50, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(57, color);
    lefteye.setPixelColor(58, color);
    lefteye.setPixelColor(59, color);
    lefteye.setPixelColor(60, color);
    lefteye.setPixelColor(61, color);
    lefteye.setPixelColor(62, color);
    lefteye.setPixelColor(63, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(65, color);
    lefteye.setPixelColor(66, color);
    lefteye.setPixelColor(67, color);
    lefteye.setPixelColor(68, color);
    lefteye.setPixelColor(69, color);
    lefteye.setPixelColor(70, color);
    lefteye.setPixelColor(71, color);
    lefteye.setPixelColor(72, color);
    lefteye.setPixelColor(73, color);
    lefteye.setPixelColor(74, color);
    lefteye.setPixelColor(80, color);
    lefteye.setPixelColor(81, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(88, color);
    lefteye.setPixelColor(90, color);
    lefteye.setPixelColor(91, color);
    lefteye.setPixelColor(92, color);
    lefteye.setPixelColor(96, color);
    lefteye.setPixelColor(99, color);
    lefteye.setPixelColor(100, color);
    lefteye.setPixelColor(101, color);
    lefteye.setPixelColor(104, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(109, color);
    lefteye.setPixelColor(112, color);
    lefteye.setPixelColor(120, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(136, color);
    lefteye.setPixelColor(144, color);
    lefteye.setPixelColor(148, color);
    lefteye.setPixelColor(149, color);
    lefteye.setPixelColor(152, color);
    lefteye.setPixelColor(155, color);
    lefteye.setPixelColor(156, color);
    lefteye.setPixelColor(157, color);
    lefteye.setPixelColor(160, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(163, color);
    lefteye.setPixelColor(164, color);
    lefteye.setPixelColor(168, color);
    lefteye.setPixelColor(169, color);
    lefteye.setPixelColor(170, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(176, color);
    lefteye.setPixelColor(177, color);
    lefteye.setPixelColor(178, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(185, color);
    lefteye.setPixelColor(186, color);
    lefteye.setPixelColor(187, color);
    lefteye.setPixelColor(188, color);
    lefteye.setPixelColor(189, color);
    lefteye.setPixelColor(190, color);
    lefteye.setPixelColor(191, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(193, color);
    lefteye.setPixelColor(194, color);
    lefteye.setPixelColor(195, color);
    lefteye.setPixelColor(196, color);
    lefteye.setPixelColor(197, color);
    lefteye.setPixelColor(198, color);
    lefteye.setPixelColor(199, color);
    lefteye.setPixelColor(200, color);
    lefteye.setPixelColor(201, color);
    lefteye.setPixelColor(202, color);
    lefteye.setPixelColor(208, color);
    lefteye.setPixelColor(209, color);
    lefteye.setPixelColor(210, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(216, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(219, color);
    lefteye.setPixelColor(220, color);
    lefteye.setPixelColor(224, color);
    lefteye.setPixelColor(227, color);
    lefteye.setPixelColor(228, color);
    lefteye.setPixelColor(229, color);
    lefteye.setPixelColor(232, color);
    lefteye.setPixelColor(236, color);
    lefteye.setPixelColor(237, color);
    lefteye.setPixelColor(240, color);
    lefteye.setPixelColor(248, color);
    righteye.setPixelColor(7, color);
    righteye.setPixelColor(15, color);
    righteye.setPixelColor(18, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(23, color);
    righteye.setPixelColor(26, color);
    righteye.setPixelColor(27, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(31, color);
    righteye.setPixelColor(35, color);
    righteye.setPixelColor(36, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(44, color);
    righteye.setPixelColor(45, color);
    righteye.setPixelColor(46, color);
    righteye.setPixelColor(47, color);
    righteye.setPixelColor(53, color);
    righteye.setPixelColor(54, color);
    righteye.setPixelColor(55, color);
    righteye.setPixelColor(56, color);
    righteye.setPixelColor(57, color);
    righteye.setPixelColor(58, color);
    righteye.setPixelColor(59, color);
    righteye.setPixelColor(60, color);
    righteye.setPixelColor(61, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(64, color);
    righteye.setPixelColor(65, color);
    righteye.setPixelColor(66, color);
    righteye.setPixelColor(67, color);
    righteye.setPixelColor(68, color);
    righteye.setPixelColor(69, color);
    righteye.setPixelColor(70, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(77, color);
    righteye.setPixelColor(78, color);
    righteye.setPixelColor(79, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(85, color);
    righteye.setPixelColor(86, color);
    righteye.setPixelColor(87, color);
    righteye.setPixelColor(91, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(95, color);
    righteye.setPixelColor(98, color);
    righteye.setPixelColor(99, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(103, color);
    righteye.setPixelColor(106, color);
    righteye.setPixelColor(107, color);
    righteye.setPixelColor(111, color);
    righteye.setPixelColor(119, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(135, color);
    righteye.setPixelColor(143, color);
    righteye.setPixelColor(146, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(151, color);
    righteye.setPixelColor(154, color);
    righteye.setPixelColor(155, color);
    righteye.setPixelColor(156, color);
    righteye.setPixelColor(159, color);
    righteye.setPixelColor(163, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(165, color);
    righteye.setPixelColor(167, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(174, color);
    righteye.setPixelColor(175, color);
    righteye.setPixelColor(181, color);
    righteye.setPixelColor(182, color);
    righteye.setPixelColor(183, color);
    righteye.setPixelColor(184, color);
    righteye.setPixelColor(185, color);
    righteye.setPixelColor(186, color);
    righteye.setPixelColor(187, color);
    righteye.setPixelColor(188, color);
    righteye.setPixelColor(189, color);
    righteye.setPixelColor(190, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(192, color);
    righteye.setPixelColor(193, color);
    righteye.setPixelColor(194, color);
    righteye.setPixelColor(195, color);
    righteye.setPixelColor(196, color);
    righteye.setPixelColor(197, color);
    righteye.setPixelColor(198, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(205, color);
    righteye.setPixelColor(206, color);
    righteye.setPixelColor(207, color);
    righteye.setPixelColor(212, color);
    righteye.setPixelColor(213, color);
    righteye.setPixelColor(214, color);
    righteye.setPixelColor(215, color);
    righteye.setPixelColor(219, color);
    righteye.setPixelColor(220, color);
    righteye.setPixelColor(221, color);
    righteye.setPixelColor(223, color);
    righteye.setPixelColor(226, color);
    righteye.setPixelColor(227, color);
    righteye.setPixelColor(228, color);
    righteye.setPixelColor(231, color);
    righteye.setPixelColor(234, color);
    righteye.setPixelColor(235, color);
    righteye.setPixelColor(239, color);
    righteye.setPixelColor(247, color);
    righteye.setPixelColor(255, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face015BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face015 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face015BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face016
void Face016()
{
  if (Face016RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(24, color);
    lefteye.setPixelColor(209, color);
    lefteye.setPixelColor(210, color);
    lefteye.setPixelColor(216, color);
    lefteye.setPixelColor(217, color);
    lefteye.setPixelColor(224, color);
    righteye.setPixelColor(31, color);
    righteye.setPixelColor(213, color);
    righteye.setPixelColor(214, color);
    righteye.setPixelColor(222, color);
    righteye.setPixelColor(223, color);
    righteye.setPixelColor(231, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(24, color);
    lefteye.setPixelColor(209, color);
    lefteye.setPixelColor(210, color);
    lefteye.setPixelColor(216, color);
    lefteye.setPixelColor(217, color);
    lefteye.setPixelColor(224, color);
    righteye.setPixelColor(31, color);
    righteye.setPixelColor(213, color);
    righteye.setPixelColor(214, color);
    righteye.setPixelColor(222, color);
    righteye.setPixelColor(223, color);
    righteye.setPixelColor(231, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face016BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face016 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
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
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(16, color);
    lefteye.setPixelColor(24, color);
    lefteye.setPixelColor(96, color);
    lefteye.setPixelColor(104, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(136, color);
    lefteye.setPixelColor(144, color);
    lefteye.setPixelColor(152, color);
    lefteye.setPixelColor(224, color);
    lefteye.setPixelColor(232, color);
    righteye.setPixelColor(23, color);
    righteye.setPixelColor(31, color);
    righteye.setPixelColor(103, color);
    righteye.setPixelColor(111, color);
    righteye.setPixelColor(119, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(151, color);
    righteye.setPixelColor(159, color);
    righteye.setPixelColor(231, color);
    righteye.setPixelColor(239, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(16, color);
    lefteye.setPixelColor(24, color);
    lefteye.setPixelColor(96, color);
    lefteye.setPixelColor(104, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(136, color);
    lefteye.setPixelColor(144, color);
    lefteye.setPixelColor(152, color);
    lefteye.setPixelColor(224, color);
    lefteye.setPixelColor(232, color);
    righteye.setPixelColor(23, color);
    righteye.setPixelColor(31, color);
    righteye.setPixelColor(103, color);
    righteye.setPixelColor(111, color);
    righteye.setPixelColor(119, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(151, color);
    righteye.setPixelColor(159, color);
    righteye.setPixelColor(231, color);
    righteye.setPixelColor(239, color);
    righteye.show();
    lefteye.show();
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face017BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face017 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face017BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face018
void Face018()
{
  if (Face018RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(97, color);
    lefteye.setPixelColor(98, color);
    lefteye.setPixelColor(105, color);
    lefteye.setPixelColor(106, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(129, color);
    lefteye.setPixelColor(130, color);
    lefteye.setPixelColor(137, color);
    lefteye.setPixelColor(138, color);
    lefteye.setPixelColor(139, color);
    lefteye.setPixelColor(146, color);
    lefteye.setPixelColor(147, color);
    lefteye.setPixelColor(148, color);
    lefteye.setPixelColor(155, color);
    lefteye.setPixelColor(156, color);
    lefteye.setPixelColor(163, color);
    lefteye.setPixelColor(164, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(172, color);
    lefteye.setPixelColor(179, color);
    lefteye.setPixelColor(180, color);
    lefteye.setPixelColor(187, color);
    lefteye.setPixelColor(188, color);
    lefteye.setPixelColor(195, color);
    lefteye.setPixelColor(196, color);
    lefteye.setPixelColor(203, color);
    lefteye.setPixelColor(204, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(212, color);
    lefteye.setPixelColor(219, color);
    lefteye.setPixelColor(220, color);
    lefteye.setPixelColor(227, color);
    lefteye.setPixelColor(228, color);
    lefteye.setPixelColor(234, color);
    lefteye.setPixelColor(235, color);
    lefteye.setPixelColor(236, color);
    lefteye.setPixelColor(241, color);
    lefteye.setPixelColor(242, color);
    lefteye.setPixelColor(243, color);
    lefteye.setPixelColor(248, color);
    lefteye.setPixelColor(249, color);
    lefteye.setPixelColor(250, color);
    righteye.setPixelColor(5, color);
    righteye.setPixelColor(6, color);
    righteye.setPixelColor(7, color);
    righteye.setPixelColor(12, color);
    righteye.setPixelColor(13, color);
    righteye.setPixelColor(14, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(20, color);
    righteye.setPixelColor(21, color);
    righteye.setPixelColor(27, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(35, color);
    righteye.setPixelColor(36, color);
    righteye.setPixelColor(43, color);
    righteye.setPixelColor(44, color);
    righteye.setPixelColor(51, color);
    righteye.setPixelColor(52, color);
    righteye.setPixelColor(59, color);
    righteye.setPixelColor(60, color);
    righteye.setPixelColor(67, color);
    righteye.setPixelColor(68, color);
    righteye.setPixelColor(75, color);
    righteye.setPixelColor(76, color);
    righteye.setPixelColor(83, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(91, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(99, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(107, color);
    righteye.setPixelColor(108, color);
    righteye.setPixelColor(109, color);
    righteye.setPixelColor(116, color);
    righteye.setPixelColor(117, color);
    righteye.setPixelColor(118, color);
    righteye.setPixelColor(125, color);
    righteye.setPixelColor(126, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(149, color);
    righteye.setPixelColor(150, color);
    righteye.setPixelColor(157, color);
    righteye.setPixelColor(158, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(97, color);
    lefteye.setPixelColor(98, color);
    lefteye.setPixelColor(105, color);
    lefteye.setPixelColor(106, color);
    lefteye.setPixelColor(128, color);
    lefteye.setPixelColor(129, color);
    lefteye.setPixelColor(130, color);
    lefteye.setPixelColor(137, color);
    lefteye.setPixelColor(138, color);
    lefteye.setPixelColor(139, color);
    lefteye.setPixelColor(146, color);
    lefteye.setPixelColor(147, color);
    lefteye.setPixelColor(148, color);
    lefteye.setPixelColor(155, color);
    lefteye.setPixelColor(156, color);
    lefteye.setPixelColor(163, color);
    lefteye.setPixelColor(164, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(172, color);
    lefteye.setPixelColor(179, color);
    lefteye.setPixelColor(180, color);
    lefteye.setPixelColor(187, color);
    lefteye.setPixelColor(188, color);
    lefteye.setPixelColor(195, color);
    lefteye.setPixelColor(196, color);
    lefteye.setPixelColor(203, color);
    lefteye.setPixelColor(204, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(212, color);
    lefteye.setPixelColor(219, color);
    lefteye.setPixelColor(220, color);
    lefteye.setPixelColor(227, color);
    lefteye.setPixelColor(228, color);
    lefteye.setPixelColor(234, color);
    lefteye.setPixelColor(235, color);
    lefteye.setPixelColor(236, color);
    lefteye.setPixelColor(241, color);
    lefteye.setPixelColor(242, color);
    lefteye.setPixelColor(243, color);
    lefteye.setPixelColor(248, color);
    lefteye.setPixelColor(249, color);
    lefteye.setPixelColor(250, color);
    righteye.setPixelColor(5, color);
    righteye.setPixelColor(6, color);
    righteye.setPixelColor(7, color);
    righteye.setPixelColor(12, color);
    righteye.setPixelColor(13, color);
    righteye.setPixelColor(14, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(20, color);
    righteye.setPixelColor(21, color);
    righteye.setPixelColor(27, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(35, color);
    righteye.setPixelColor(36, color);
    righteye.setPixelColor(43, color);
    righteye.setPixelColor(44, color);
    righteye.setPixelColor(51, color);
    righteye.setPixelColor(52, color);
    righteye.setPixelColor(59, color);
    righteye.setPixelColor(60, color);
    righteye.setPixelColor(67, color);
    righteye.setPixelColor(68, color);
    righteye.setPixelColor(75, color);
    righteye.setPixelColor(76, color);
    righteye.setPixelColor(83, color);
    righteye.setPixelColor(84, color);
    righteye.setPixelColor(91, color);
    righteye.setPixelColor(92, color);
    righteye.setPixelColor(99, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(107, color);
    righteye.setPixelColor(108, color);
    righteye.setPixelColor(109, color);
    righteye.setPixelColor(116, color);
    righteye.setPixelColor(117, color);
    righteye.setPixelColor(118, color);
    righteye.setPixelColor(125, color);
    righteye.setPixelColor(126, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(149, color);
    righteye.setPixelColor(150, color);
    righteye.setPixelColor(157, color);
    righteye.setPixelColor(158, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face018BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face018 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
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
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(0, color);
    lefteye.setPixelColor(1, color);
    lefteye.setPixelColor(2, color);
    lefteye.setPixelColor(9, color);
    lefteye.setPixelColor(10, color);
    lefteye.setPixelColor(11, color);
    lefteye.setPixelColor(18, color);
    lefteye.setPixelColor(19, color);
    lefteye.setPixelColor(20, color);
    lefteye.setPixelColor(27, color);
    lefteye.setPixelColor(28, color);
    lefteye.setPixelColor(35, color);
    lefteye.setPixelColor(36, color);
    lefteye.setPixelColor(43, color);
    lefteye.setPixelColor(44, color);
    lefteye.setPixelColor(51, color);
    lefteye.setPixelColor(52, color);
    lefteye.setPixelColor(59, color);
    lefteye.setPixelColor(60, color);
    lefteye.setPixelColor(67, color);
    lefteye.setPixelColor(68, color);
    lefteye.setPixelColor(75, color);
    lefteye.setPixelColor(76, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(84, color);
    lefteye.setPixelColor(91, color);
    lefteye.setPixelColor(92, color);
    lefteye.setPixelColor(99, color);
    lefteye.setPixelColor(100, color);
    lefteye.setPixelColor(106, color);
    lefteye.setPixelColor(107, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(113, color);
    lefteye.setPixelColor(114, color);
    lefteye.setPixelColor(115, color);
    lefteye.setPixelColor(120, color);
    lefteye.setPixelColor(121, color);
    lefteye.setPixelColor(122, color);
    lefteye.setPixelColor(145, color);
    lefteye.setPixelColor(146, color);
    lefteye.setPixelColor(153, color);
    lefteye.setPixelColor(154, color);
    righteye.setPixelColor(101, color);
    righteye.setPixelColor(102, color);
    righteye.setPixelColor(109, color);
    righteye.setPixelColor(110, color);
    righteye.setPixelColor(133, color);
    righteye.setPixelColor(134, color);
    righteye.setPixelColor(135, color);
    righteye.setPixelColor(140, color);
    righteye.setPixelColor(141, color);
    righteye.setPixelColor(142, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(148, color);
    righteye.setPixelColor(149, color);
    righteye.setPixelColor(155, color);
    righteye.setPixelColor(156, color);
    righteye.setPixelColor(163, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(171, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(179, color);
    righteye.setPixelColor(180, color);
    righteye.setPixelColor(187, color);
    righteye.setPixelColor(188, color);
    righteye.setPixelColor(195, color);
    righteye.setPixelColor(196, color);
    righteye.setPixelColor(203, color);
    righteye.setPixelColor(204, color);
    righteye.setPixelColor(211, color);
    righteye.setPixelColor(212, color);
    righteye.setPixelColor(219, color);
    righteye.setPixelColor(220, color);
    righteye.setPixelColor(227, color);
    righteye.setPixelColor(228, color);
    righteye.setPixelColor(235, color);
    righteye.setPixelColor(236, color);
    righteye.setPixelColor(237, color);
    righteye.setPixelColor(244, color);
    righteye.setPixelColor(245, color);
    righteye.setPixelColor(246, color);
    righteye.setPixelColor(253, color);
    righteye.setPixelColor(254, color);
    righteye.setPixelColor(255, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(0, color);
    lefteye.setPixelColor(1, color);
    lefteye.setPixelColor(2, color);
    lefteye.setPixelColor(9, color);
    lefteye.setPixelColor(10, color);
    lefteye.setPixelColor(11, color);
    lefteye.setPixelColor(18, color);
    lefteye.setPixelColor(19, color);
    lefteye.setPixelColor(20, color);
    lefteye.setPixelColor(27, color);
    lefteye.setPixelColor(28, color);
    lefteye.setPixelColor(35, color);
    lefteye.setPixelColor(36, color);
    lefteye.setPixelColor(43, color);
    lefteye.setPixelColor(44, color);
    lefteye.setPixelColor(51, color);
    lefteye.setPixelColor(52, color);
    lefteye.setPixelColor(59, color);
    lefteye.setPixelColor(60, color);
    lefteye.setPixelColor(67, color);
    lefteye.setPixelColor(68, color);
    lefteye.setPixelColor(75, color);
    lefteye.setPixelColor(76, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(84, color);
    lefteye.setPixelColor(91, color);
    lefteye.setPixelColor(92, color);
    lefteye.setPixelColor(99, color);
    lefteye.setPixelColor(100, color);
    lefteye.setPixelColor(106, color);
    lefteye.setPixelColor(107, color);
    lefteye.setPixelColor(108, color);
    lefteye.setPixelColor(113, color);
    lefteye.setPixelColor(114, color);
    lefteye.setPixelColor(115, color);
    lefteye.setPixelColor(120, color);
    lefteye.setPixelColor(121, color);
    lefteye.setPixelColor(122, color);
    lefteye.setPixelColor(145, color);
    lefteye.setPixelColor(146, color);
    lefteye.setPixelColor(153, color);
    lefteye.setPixelColor(154, color);
    righteye.setPixelColor(101, color);
    righteye.setPixelColor(102, color);
    righteye.setPixelColor(109, color);
    righteye.setPixelColor(110, color);
    righteye.setPixelColor(133, color);
    righteye.setPixelColor(134, color);
    righteye.setPixelColor(135, color);
    righteye.setPixelColor(140, color);
    righteye.setPixelColor(141, color);
    righteye.setPixelColor(142, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(148, color);
    righteye.setPixelColor(149, color);
    righteye.setPixelColor(155, color);
    righteye.setPixelColor(156, color);
    righteye.setPixelColor(163, color);
    righteye.setPixelColor(164, color);
    righteye.setPixelColor(171, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(179, color);
    righteye.setPixelColor(180, color);
    righteye.setPixelColor(187, color);
    righteye.setPixelColor(188, color);
    righteye.setPixelColor(195, color);
    righteye.setPixelColor(196, color);
    righteye.setPixelColor(203, color);
    righteye.setPixelColor(204, color);
    righteye.setPixelColor(211, color);
    righteye.setPixelColor(212, color);
    righteye.setPixelColor(219, color);
    righteye.setPixelColor(220, color);
    righteye.setPixelColor(227, color);
    righteye.setPixelColor(228, color);
    righteye.setPixelColor(235, color);
    righteye.setPixelColor(236, color);
    righteye.setPixelColor(237, color);
    righteye.setPixelColor(244, color);
    righteye.setPixelColor(245, color);
    righteye.setPixelColor(246, color);
    righteye.setPixelColor(253, color);
    righteye.setPixelColor(254, color);
    righteye.setPixelColor(255, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face019BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face019 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face019BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face020
void Face020()
{
  if (Face020RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(16, color);
    lefteye.setPixelColor(17, color);
    lefteye.setPixelColor(18, color);
    lefteye.setPixelColor(24, color);
    lefteye.setPixelColor(25, color);
    lefteye.setPixelColor(26, color);
    lefteye.setPixelColor(27, color);
    lefteye.setPixelColor(34, color);
    lefteye.setPixelColor(35, color);
    lefteye.setPixelColor(36, color);
    lefteye.setPixelColor(43, color);
    lefteye.setPixelColor(44, color);
    lefteye.setPixelColor(45, color);
    lefteye.setPixelColor(52, color);
    lefteye.setPixelColor(53, color);
    lefteye.setPixelColor(60, color);
    lefteye.setPixelColor(61, color);
    lefteye.setPixelColor(68, color);
    lefteye.setPixelColor(69, color);
    lefteye.setPixelColor(76, color);
    lefteye.setPixelColor(77, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(84, color);
    lefteye.setPixelColor(85, color);
    lefteye.setPixelColor(90, color);
    lefteye.setPixelColor(91, color);
    lefteye.setPixelColor(92, color);
    lefteye.setPixelColor(96, color);
    lefteye.setPixelColor(97, color);
    lefteye.setPixelColor(98, color);
    lefteye.setPixelColor(99, color);
    lefteye.setPixelColor(104, color);
    lefteye.setPixelColor(105, color);
    lefteye.setPixelColor(106, color);
    lefteye.setPixelColor(144, color);
    lefteye.setPixelColor(145, color);
    lefteye.setPixelColor(146, color);
    lefteye.setPixelColor(152, color);
    lefteye.setPixelColor(153, color);
    lefteye.setPixelColor(154, color);
    lefteye.setPixelColor(155, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(163, color);
    lefteye.setPixelColor(164, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(172, color);
    lefteye.setPixelColor(173, color);
    lefteye.setPixelColor(180, color);
    lefteye.setPixelColor(181, color);
    lefteye.setPixelColor(188, color);
    lefteye.setPixelColor(189, color);
    lefteye.setPixelColor(196, color);
    lefteye.setPixelColor(197, color);
    lefteye.setPixelColor(204, color);
    lefteye.setPixelColor(205, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(212, color);
    lefteye.setPixelColor(213, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(219, color);
    lefteye.setPixelColor(220, color);
    lefteye.setPixelColor(224, color);
    lefteye.setPixelColor(225, color);
    lefteye.setPixelColor(226, color);
    lefteye.setPixelColor(227, color);
    lefteye.setPixelColor(232, color);
    lefteye.setPixelColor(233, color);
    lefteye.setPixelColor(234, color);
    righteye.setPixelColor(3, color);
    righteye.setPixelColor(4, color);
    righteye.setPixelColor(5, color);
    righteye.setPixelColor(6, color);
    righteye.setPixelColor(7, color);
    righteye.setPixelColor(10, color);
    righteye.setPixelColor(11, color);
    righteye.setPixelColor(12, color);
    righteye.setPixelColor(13, color);
    righteye.setPixelColor(14, color);
    righteye.setPixelColor(15, color);
    righteye.setPixelColor(17, color);
    righteye.setPixelColor(18, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(24, color);
    righteye.setPixelColor(25, color);
    righteye.setPixelColor(26, color);
    righteye.setPixelColor(32, color);
    righteye.setPixelColor(33, color);
    righteye.setPixelColor(40, color);
    righteye.setPixelColor(41, color);
    righteye.setPixelColor(48, color);
    righteye.setPixelColor(49, color);
    righteye.setPixelColor(56, color);
    righteye.setPixelColor(57, color);
    righteye.setPixelColor(64, color);
    righteye.setPixelColor(65, color);
    righteye.setPixelColor(72, color);
    righteye.setPixelColor(73, color);
    righteye.setPixelColor(80, color);
    righteye.setPixelColor(81, color);
    righteye.setPixelColor(88, color);
    righteye.setPixelColor(89, color);
    righteye.setPixelColor(96, color);
    righteye.setPixelColor(97, color);
    righteye.setPixelColor(98, color);
    righteye.setPixelColor(105, color);
    righteye.setPixelColor(106, color);
    righteye.setPixelColor(107, color);
    righteye.setPixelColor(114, color);
    righteye.setPixelColor(115, color);
    righteye.setPixelColor(116, color);
    righteye.setPixelColor(117, color);
    righteye.setPixelColor(118, color);
    righteye.setPixelColor(119, color);
    righteye.setPixelColor(123, color);
    righteye.setPixelColor(124, color);
    righteye.setPixelColor(125, color);
    righteye.setPixelColor(126, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(131, color);
    righteye.setPixelColor(132, color);
    righteye.setPixelColor(133, color);
    righteye.setPixelColor(134, color);
    righteye.setPixelColor(135, color);
    righteye.setPixelColor(138, color);
    righteye.setPixelColor(139, color);
    righteye.setPixelColor(140, color);
    righteye.setPixelColor(141, color);
    righteye.setPixelColor(142, color);
    righteye.setPixelColor(143, color);
    righteye.setPixelColor(145, color);
    righteye.setPixelColor(146, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(152, color);
    righteye.setPixelColor(153, color);
    righteye.setPixelColor(154, color);
    righteye.setPixelColor(160, color);
    righteye.setPixelColor(161, color);
    righteye.setPixelColor(168, color);
    righteye.setPixelColor(169, color);
    righteye.setPixelColor(176, color);
    righteye.setPixelColor(177, color);
    righteye.setPixelColor(184, color);
    righteye.setPixelColor(185, color);
    righteye.setPixelColor(192, color);
    righteye.setPixelColor(193, color);
    righteye.setPixelColor(200, color);
    righteye.setPixelColor(201, color);
    righteye.setPixelColor(208, color);
    righteye.setPixelColor(209, color);
    righteye.setPixelColor(216, color);
    righteye.setPixelColor(217, color);
    righteye.setPixelColor(224, color);
    righteye.setPixelColor(225, color);
    righteye.setPixelColor(226, color);
    righteye.setPixelColor(233, color);
    righteye.setPixelColor(234, color);
    righteye.setPixelColor(235, color);
    righteye.setPixelColor(242, color);
    righteye.setPixelColor(243, color);
    righteye.setPixelColor(244, color);
    righteye.setPixelColor(245, color);
    righteye.setPixelColor(246, color);
    righteye.setPixelColor(247, color);
    righteye.setPixelColor(251, color);
    righteye.setPixelColor(252, color);
    righteye.setPixelColor(253, color);
    righteye.setPixelColor(254, color);
    righteye.setPixelColor(255, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(16, color);
    lefteye.setPixelColor(17, color);
    lefteye.setPixelColor(18, color);
    lefteye.setPixelColor(24, color);
    lefteye.setPixelColor(25, color);
    lefteye.setPixelColor(26, color);
    lefteye.setPixelColor(27, color);
    lefteye.setPixelColor(34, color);
    lefteye.setPixelColor(35, color);
    lefteye.setPixelColor(36, color);
    lefteye.setPixelColor(43, color);
    lefteye.setPixelColor(44, color);
    lefteye.setPixelColor(45, color);
    lefteye.setPixelColor(52, color);
    lefteye.setPixelColor(53, color);
    lefteye.setPixelColor(60, color);
    lefteye.setPixelColor(61, color);
    lefteye.setPixelColor(68, color);
    lefteye.setPixelColor(69, color);
    lefteye.setPixelColor(76, color);
    lefteye.setPixelColor(77, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(84, color);
    lefteye.setPixelColor(85, color);
    lefteye.setPixelColor(90, color);
    lefteye.setPixelColor(91, color);
    lefteye.setPixelColor(92, color);
    lefteye.setPixelColor(96, color);
    lefteye.setPixelColor(97, color);
    lefteye.setPixelColor(98, color);
    lefteye.setPixelColor(99, color);
    lefteye.setPixelColor(104, color);
    lefteye.setPixelColor(105, color);
    lefteye.setPixelColor(106, color);
    lefteye.setPixelColor(144, color);
    lefteye.setPixelColor(145, color);
    lefteye.setPixelColor(146, color);
    lefteye.setPixelColor(152, color);
    lefteye.setPixelColor(153, color);
    lefteye.setPixelColor(154, color);
    lefteye.setPixelColor(155, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(163, color);
    lefteye.setPixelColor(164, color);
    lefteye.setPixelColor(171, color);
    lefteye.setPixelColor(172, color);
    lefteye.setPixelColor(173, color);
    lefteye.setPixelColor(180, color);
    lefteye.setPixelColor(181, color);
    lefteye.setPixelColor(188, color);
    lefteye.setPixelColor(189, color);
    lefteye.setPixelColor(196, color);
    lefteye.setPixelColor(197, color);
    lefteye.setPixelColor(204, color);
    lefteye.setPixelColor(205, color);
    lefteye.setPixelColor(211, color);
    lefteye.setPixelColor(212, color);
    lefteye.setPixelColor(213, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(219, color);
    lefteye.setPixelColor(220, color);
    lefteye.setPixelColor(224, color);
    lefteye.setPixelColor(225, color);
    lefteye.setPixelColor(226, color);
    lefteye.setPixelColor(227, color);
    lefteye.setPixelColor(232, color);
    lefteye.setPixelColor(233, color);
    lefteye.setPixelColor(234, color);
    righteye.setPixelColor(3, color);
    righteye.setPixelColor(4, color);
    righteye.setPixelColor(5, color);
    righteye.setPixelColor(6, color);
    righteye.setPixelColor(7, color);
    righteye.setPixelColor(10, color);
    righteye.setPixelColor(11, color);
    righteye.setPixelColor(12, color);
    righteye.setPixelColor(13, color);
    righteye.setPixelColor(14, color);
    righteye.setPixelColor(15, color);
    righteye.setPixelColor(17, color);
    righteye.setPixelColor(18, color);
    righteye.setPixelColor(19, color);
    righteye.setPixelColor(24, color);
    righteye.setPixelColor(25, color);
    righteye.setPixelColor(26, color);
    righteye.setPixelColor(32, color);
    righteye.setPixelColor(33, color);
    righteye.setPixelColor(40, color);
    righteye.setPixelColor(41, color);
    righteye.setPixelColor(48, color);
    righteye.setPixelColor(49, color);
    righteye.setPixelColor(56, color);
    righteye.setPixelColor(57, color);
    righteye.setPixelColor(64, color);
    righteye.setPixelColor(65, color);
    righteye.setPixelColor(72, color);
    righteye.setPixelColor(73, color);
    righteye.setPixelColor(80, color);
    righteye.setPixelColor(81, color);
    righteye.setPixelColor(88, color);
    righteye.setPixelColor(89, color);
    righteye.setPixelColor(96, color);
    righteye.setPixelColor(97, color);
    righteye.setPixelColor(98, color);
    righteye.setPixelColor(105, color);
    righteye.setPixelColor(106, color);
    righteye.setPixelColor(107, color);
    righteye.setPixelColor(114, color);
    righteye.setPixelColor(115, color);
    righteye.setPixelColor(116, color);
    righteye.setPixelColor(117, color);
    righteye.setPixelColor(118, color);
    righteye.setPixelColor(119, color);
    righteye.setPixelColor(123, color);
    righteye.setPixelColor(124, color);
    righteye.setPixelColor(125, color);
    righteye.setPixelColor(126, color);
    righteye.setPixelColor(127, color);
    righteye.setPixelColor(131, color);
    righteye.setPixelColor(132, color);
    righteye.setPixelColor(133, color);
    righteye.setPixelColor(134, color);
    righteye.setPixelColor(135, color);
    righteye.setPixelColor(138, color);
    righteye.setPixelColor(139, color);
    righteye.setPixelColor(140, color);
    righteye.setPixelColor(141, color);
    righteye.setPixelColor(142, color);
    righteye.setPixelColor(143, color);
    righteye.setPixelColor(145, color);
    righteye.setPixelColor(146, color);
    righteye.setPixelColor(147, color);
    righteye.setPixelColor(152, color);
    righteye.setPixelColor(153, color);
    righteye.setPixelColor(154, color);
    righteye.setPixelColor(160, color);
    righteye.setPixelColor(161, color);
    righteye.setPixelColor(168, color);
    righteye.setPixelColor(169, color);
    righteye.setPixelColor(176, color);
    righteye.setPixelColor(177, color);
    righteye.setPixelColor(184, color);
    righteye.setPixelColor(185, color);
    righteye.setPixelColor(192, color);
    righteye.setPixelColor(193, color);
    righteye.setPixelColor(200, color);
    righteye.setPixelColor(201, color);
    righteye.setPixelColor(208, color);
    righteye.setPixelColor(209, color);
    righteye.setPixelColor(216, color);
    righteye.setPixelColor(217, color);
    righteye.setPixelColor(224, color);
    righteye.setPixelColor(225, color);
    righteye.setPixelColor(226, color);
    righteye.setPixelColor(233, color);
    righteye.setPixelColor(234, color);
    righteye.setPixelColor(235, color);
    righteye.setPixelColor(242, color);
    righteye.setPixelColor(243, color);
    righteye.setPixelColor(244, color);
    righteye.setPixelColor(245, color);
    righteye.setPixelColor(246, color);
    righteye.setPixelColor(247, color);
    righteye.setPixelColor(251, color);
    righteye.setPixelColor(252, color);
    righteye.setPixelColor(253, color);
    righteye.setPixelColor(254, color);
    righteye.setPixelColor(255, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face020BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face020 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    delay(10);
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face020BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face021
void Face021()
{
  if (Face021RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(48, color);
    lefteye.setPixelColor(49, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(57, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(65, color);
    lefteye.setPixelColor(72, color);
    lefteye.setPixelColor(73, color);
    lefteye.setPixelColor(176, color);
    lefteye.setPixelColor(177, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(185, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(193, color);
    lefteye.setPixelColor(200, color);
    lefteye.setPixelColor(201, color);
    righteye.setPixelColor(54, color);
    righteye.setPixelColor(55, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(70, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(78, color);
    righteye.setPixelColor(79, color);
    righteye.setPixelColor(182, color);
    righteye.setPixelColor(183, color);
    righteye.setPixelColor(190, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(198, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(206, color);
    righteye.setPixelColor(207, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(48, color);
    lefteye.setPixelColor(49, color);
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(57, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(65, color);
    lefteye.setPixelColor(72, color);
    lefteye.setPixelColor(73, color);
    lefteye.setPixelColor(176, color);
    lefteye.setPixelColor(177, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(185, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(193, color);
    lefteye.setPixelColor(200, color);
    lefteye.setPixelColor(201, color);
    righteye.setPixelColor(54, color);
    righteye.setPixelColor(55, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(70, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(78, color);
    righteye.setPixelColor(79, color);
    righteye.setPixelColor(182, color);
    righteye.setPixelColor(183, color);
    righteye.setPixelColor(190, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(198, color);
    righteye.setPixelColor(199, color);
    righteye.setPixelColor(206, color);
    righteye.setPixelColor(207, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face021BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face021 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face021BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face022
void Face022()
{
  if (Face022RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(57, color);
    lefteye.setPixelColor(58, color);
    lefteye.setPixelColor(59, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(65, color);
    lefteye.setPixelColor(66, color);
    lefteye.setPixelColor(67, color);
    lefteye.setPixelColor(74, color);
    lefteye.setPixelColor(75, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(185, color);
    lefteye.setPixelColor(186, color);
    lefteye.setPixelColor(187, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(193, color);
    lefteye.setPixelColor(194, color);
    lefteye.setPixelColor(195, color);
    righteye.setPixelColor(60, color);
    righteye.setPixelColor(61, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(68, color);
    righteye.setPixelColor(69, color);
    righteye.setPixelColor(70, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(180, color);
    righteye.setPixelColor(181, color);
    righteye.setPixelColor(188, color);
    righteye.setPixelColor(189, color);
    righteye.setPixelColor(190, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(196, color);
    righteye.setPixelColor(197, color);
    righteye.setPixelColor(198, color);
    righteye.setPixelColor(199, color);
    righteye.show();
    lefteye.show();
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(56, color);
    lefteye.setPixelColor(57, color);
    lefteye.setPixelColor(58, color);
    lefteye.setPixelColor(59, color);
    lefteye.setPixelColor(64, color);
    lefteye.setPixelColor(65, color);
    lefteye.setPixelColor(66, color);
    lefteye.setPixelColor(67, color);
    lefteye.setPixelColor(74, color);
    lefteye.setPixelColor(75, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(184, color);
    lefteye.setPixelColor(185, color);
    lefteye.setPixelColor(186, color);
    lefteye.setPixelColor(187, color);
    lefteye.setPixelColor(192, color);
    lefteye.setPixelColor(193, color);
    lefteye.setPixelColor(194, color);
    lefteye.setPixelColor(195, color);
    righteye.setPixelColor(60, color);
    righteye.setPixelColor(61, color);
    righteye.setPixelColor(62, color);
    righteye.setPixelColor(63, color);
    righteye.setPixelColor(68, color);
    righteye.setPixelColor(69, color);
    righteye.setPixelColor(70, color);
    righteye.setPixelColor(71, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(180, color);
    righteye.setPixelColor(181, color);
    righteye.setPixelColor(188, color);
    righteye.setPixelColor(189, color);
    righteye.setPixelColor(190, color);
    righteye.setPixelColor(191, color);
    righteye.setPixelColor(196, color);
    righteye.setPixelColor(197, color);
    righteye.setPixelColor(198, color);
    righteye.setPixelColor(199, color);
    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face022BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face022 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face022BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}
// Draw Face023
void Face023()
{
  if (Face022RunningCheck == false) // Check to see if this face is currently running or was recently started - this was added to prevent double blinking when switching faces
  {
    Serial.println("Blink Paused");
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(32, color);
    lefteye.setPixelColor(33, color);
    lefteye.setPixelColor(41, color);
    lefteye.setPixelColor(42, color);
    lefteye.setPixelColor(43, color);
    lefteye.setPixelColor(51, color);
    lefteye.setPixelColor(52, color);
    lefteye.setPixelColor(53, color);
    lefteye.setPixelColor(61, color);
    lefteye.setPixelColor(62, color);
    lefteye.setPixelColor(63, color);
    lefteye.setPixelColor(69, color);
    lefteye.setPixelColor(70, color);
    lefteye.setPixelColor(71, color);
    lefteye.setPixelColor(75, color);
    lefteye.setPixelColor(76, color);
    lefteye.setPixelColor(77, color);
    lefteye.setPixelColor(81, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(88, color);
    lefteye.setPixelColor(89, color);
    lefteye.setPixelColor(155, color);
    lefteye.setPixelColor(156, color);
    lefteye.setPixelColor(157, color);
    lefteye.setPixelColor(160, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(166, color);
    lefteye.setPixelColor(174, color);
    lefteye.setPixelColor(182, color);
    lefteye.setPixelColor(189, color);
    lefteye.setPixelColor(197, color);
    lefteye.setPixelColor(206, color);
    lefteye.setPixelColor(214, color);
    lefteye.setPixelColor(216, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(222, color);
    lefteye.setPixelColor(227, color);
    lefteye.setPixelColor(228, color);
    lefteye.setPixelColor(229, color);
    righteye.setPixelColor(26, color);
    righteye.setPixelColor(27, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(33, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(41, color);
    righteye.setPixelColor(49, color);
    righteye.setPixelColor(58, color);
    righteye.setPixelColor(66, color);
    righteye.setPixelColor(73, color);
    righteye.setPixelColor(81, color);
    righteye.setPixelColor(89, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(95, color);
    righteye.setPixelColor(98, color);
    righteye.setPixelColor(99, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(166, color);
    righteye.setPixelColor(167, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(174, color);
    righteye.setPixelColor(178, color);
    righteye.setPixelColor(179, color);
    righteye.setPixelColor(180, color);
    righteye.setPixelColor(184, color);
    righteye.setPixelColor(185, color);
    righteye.setPixelColor(186, color);
    righteye.setPixelColor(192, color);
    righteye.setPixelColor(193, color);
    righteye.setPixelColor(194, color);
    righteye.setPixelColor(202, color);
    righteye.setPixelColor(203, color);
    righteye.setPixelColor(204, color);
    righteye.setPixelColor(212, color);
    righteye.setPixelColor(213, color);
    righteye.setPixelColor(214, color);
    righteye.setPixelColor(222, color);
    righteye.setPixelColor(223, color);
    righteye.show();
    lefteye.show(); 
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
    lefteye.clear();
    righteye.clear();
    lefteye.setPixelColor(32, color);
    lefteye.setPixelColor(33, color);
    lefteye.setPixelColor(41, color);
    lefteye.setPixelColor(42, color);
    lefteye.setPixelColor(43, color);
    lefteye.setPixelColor(51, color);
    lefteye.setPixelColor(52, color);
    lefteye.setPixelColor(53, color);
    lefteye.setPixelColor(61, color);
    lefteye.setPixelColor(62, color);
    lefteye.setPixelColor(63, color);
    lefteye.setPixelColor(69, color);
    lefteye.setPixelColor(70, color);
    lefteye.setPixelColor(71, color);
    lefteye.setPixelColor(75, color);
    lefteye.setPixelColor(76, color);
    lefteye.setPixelColor(77, color);
    lefteye.setPixelColor(81, color);
    lefteye.setPixelColor(82, color);
    lefteye.setPixelColor(83, color);
    lefteye.setPixelColor(88, color);
    lefteye.setPixelColor(89, color);
    lefteye.setPixelColor(155, color);
    lefteye.setPixelColor(156, color);
    lefteye.setPixelColor(157, color);
    lefteye.setPixelColor(160, color);
    lefteye.setPixelColor(162, color);
    lefteye.setPixelColor(166, color);
    lefteye.setPixelColor(174, color);
    lefteye.setPixelColor(182, color);
    lefteye.setPixelColor(189, color);
    lefteye.setPixelColor(197, color);
    lefteye.setPixelColor(206, color);
    lefteye.setPixelColor(214, color);
    lefteye.setPixelColor(216, color);
    lefteye.setPixelColor(218, color);
    lefteye.setPixelColor(222, color);
    lefteye.setPixelColor(227, color);
    lefteye.setPixelColor(228, color);
    lefteye.setPixelColor(229, color);
    righteye.setPixelColor(26, color);
    righteye.setPixelColor(27, color);
    righteye.setPixelColor(28, color);
    righteye.setPixelColor(33, color);
    righteye.setPixelColor(37, color);
    righteye.setPixelColor(39, color);
    righteye.setPixelColor(41, color);
    righteye.setPixelColor(49, color);
    righteye.setPixelColor(58, color);
    righteye.setPixelColor(66, color);
    righteye.setPixelColor(73, color);
    righteye.setPixelColor(81, color);
    righteye.setPixelColor(89, color);
    righteye.setPixelColor(93, color);
    righteye.setPixelColor(95, color);
    righteye.setPixelColor(98, color);
    righteye.setPixelColor(99, color);
    righteye.setPixelColor(100, color);
    righteye.setPixelColor(166, color);
    righteye.setPixelColor(167, color);
    righteye.setPixelColor(172, color);
    righteye.setPixelColor(173, color);
    righteye.setPixelColor(174, color);
    righteye.setPixelColor(178, color);
    righteye.setPixelColor(179, color);
    righteye.setPixelColor(180, color);
    righteye.setPixelColor(184, color);
    righteye.setPixelColor(185, color);
    righteye.setPixelColor(186, color);
    righteye.setPixelColor(192, color);
    righteye.setPixelColor(193, color);
    righteye.setPixelColor(194, color);
    righteye.setPixelColor(202, color);
    righteye.setPixelColor(203, color);
    righteye.setPixelColor(204, color);
    righteye.setPixelColor(212, color);
    righteye.setPixelColor(213, color);
    righteye.setPixelColor(214, color);
    righteye.setPixelColor(222, color);
    righteye.setPixelColor(223, color);

    righteye.show();
    lefteye.show();
    }
    else if((ledState == 1) && (currentMillis - previousMillis >= OnTime)) // Blink off the LEDs 
   {
    ledState = 0;  // Set the LEDs status to off
    previousMillis = currentMillis; // Remember the time
     if(Face023BlinkCheck == true) // Checking BlinkCheck for this face to false to prevent an intermittent double flash when changing faces
    {
    Serial.println("Face022 LEDs OFF - Blink"); // Debug output
    lefteye.clear();
    righteye.clear();
    lefteye.show(); 
    righteye.show();
    }
    else
    {
    Face023BlinkCheck= true; // Setting BlinkCheck to allow blinking
    Serial.println("Blink Resumed"); // Debug output
   } 
   } 
}

// Umbrella Logo
void Umbrella()
{
  Serial.println("Umbrella LEDs ON");
  lefteye.clear();
  righteye.clear();
  lefteye.setPixelColor(2, colorRed);
  lefteye.setPixelColor(4, colorWhite);
  lefteye.setPixelColor(8, colorRed);
  lefteye.setPixelColor(9, colorRed);
  lefteye.setPixelColor(10, colorRed);
  lefteye.setPixelColor(12, colorWhite);
  lefteye.setPixelColor(13, colorWhite);
  lefteye.setPixelColor(16, colorRed);
  lefteye.setPixelColor(17, colorRed);
  lefteye.setPixelColor(20, colorWhite);
  lefteye.setPixelColor(21, colorWhite);
  lefteye.setPixelColor(22, colorWhite);
  lefteye.setPixelColor(24, colorRed);
  lefteye.setPixelColor(25, colorRed);
  lefteye.setPixelColor(27, colorWhite);
  lefteye.setPixelColor(28, colorWhite);
  lefteye.setPixelColor(29, colorWhite);
  lefteye.setPixelColor(30, colorWhite);
  lefteye.setPixelColor(31, colorWhite);
  lefteye.setPixelColor(32, colorRed);
  lefteye.setPixelColor(34, colorWhite);
  lefteye.setPixelColor(35, colorWhite);
  lefteye.setPixelColor(36, colorWhite);
  lefteye.setPixelColor(40, colorRed);
  lefteye.setPixelColor(42, colorWhite);
  lefteye.setPixelColor(43, colorWhite);
  lefteye.setPixelColor(46, colorRed);
  lefteye.setPixelColor(47, colorRed);
  lefteye.setPixelColor(52, colorRed);
  lefteye.setPixelColor(53, colorRed);
  lefteye.setPixelColor(54, colorRed);
  lefteye.setPixelColor(58, colorRed);
  lefteye.setPixelColor(59, colorRed);
  lefteye.setPixelColor(60, colorRed);
  lefteye.setPixelColor(61, colorRed);
  lefteye.setPixelColor(62, colorRed);
  lefteye.setPixelColor(66, colorRed);
  lefteye.setPixelColor(67, colorRed);
  lefteye.setPixelColor(68, colorRed);
  lefteye.setPixelColor(69, colorRed);
  lefteye.setPixelColor(70, colorRed);
  lefteye.setPixelColor(76, colorRed);
  lefteye.setPixelColor(77, colorRed);
  lefteye.setPixelColor(78, colorRed);
  lefteye.setPixelColor(80, colorRed);
  lefteye.setPixelColor(82, colorWhite);
  lefteye.setPixelColor(83, colorWhite);
  lefteye.setPixelColor(86, colorRed);
  lefteye.setPixelColor(87, colorRed);
  lefteye.setPixelColor(88, colorRed);
  lefteye.setPixelColor(90, colorWhite);
  lefteye.setPixelColor(91, colorWhite);
  lefteye.setPixelColor(92, colorWhite);
  lefteye.setPixelColor(96, colorRed);
  lefteye.setPixelColor(97, colorRed);
  lefteye.setPixelColor(99, colorWhite);
  lefteye.setPixelColor(100, colorWhite);
  lefteye.setPixelColor(101, colorWhite);
  lefteye.setPixelColor(102, colorWhite);
  lefteye.setPixelColor(103, colorWhite);
  lefteye.setPixelColor(104, colorRed);
  lefteye.setPixelColor(105, colorRed);
  lefteye.setPixelColor(108, colorWhite);
  lefteye.setPixelColor(109, colorWhite);
  lefteye.setPixelColor(110, colorWhite);
  lefteye.setPixelColor(112, colorRed);
  lefteye.setPixelColor(113, colorRed);
  lefteye.setPixelColor(114, colorRed);
  lefteye.setPixelColor(116, colorWhite);
  lefteye.setPixelColor(117, colorWhite);
  lefteye.setPixelColor(122, colorRed);
  lefteye.setPixelColor(124, colorWhite);
  lefteye.setPixelColor(130, colorRed);
  lefteye.setPixelColor(132, colorWhite);
  lefteye.setPixelColor(136, colorRed);
  lefteye.setPixelColor(137, colorRed);
  lefteye.setPixelColor(138, colorRed);
  lefteye.setPixelColor(140, colorWhite);
  lefteye.setPixelColor(141, colorWhite);
  lefteye.setPixelColor(144, colorRed);
  lefteye.setPixelColor(145, colorRed);
  lefteye.setPixelColor(148, colorWhite);
  lefteye.setPixelColor(149, colorWhite);
  lefteye.setPixelColor(150, colorWhite);
  lefteye.setPixelColor(152, colorRed);
  lefteye.setPixelColor(153, colorRed);
  lefteye.setPixelColor(155, colorWhite);
  lefteye.setPixelColor(156, colorWhite);
  lefteye.setPixelColor(157, colorWhite);
  lefteye.setPixelColor(158, colorWhite);
  lefteye.setPixelColor(159, colorWhite);
  lefteye.setPixelColor(160, colorRed);
  lefteye.setPixelColor(162, colorWhite);
  lefteye.setPixelColor(163, colorWhite);
  lefteye.setPixelColor(164, colorWhite);
  lefteye.setPixelColor(168, colorRed);
  lefteye.setPixelColor(174, colorRed);
  lefteye.setPixelColor(175, colorRed);
  lefteye.setPixelColor(180, colorRed);
  lefteye.setPixelColor(181, colorRed);
  lefteye.setPixelColor(182, colorRed);
  lefteye.setPixelColor(186, colorRed);
  lefteye.setPixelColor(187, colorRed);
  lefteye.setPixelColor(188, colorRed);
  lefteye.setPixelColor(189, colorRed);
  lefteye.setPixelColor(190, colorRed);
  lefteye.setPixelColor(194, colorRed);
  lefteye.setPixelColor(195, colorRed);
  lefteye.setPixelColor(196, colorRed);
  lefteye.setPixelColor(197, colorRed);
  lefteye.setPixelColor(198, colorRed);
  lefteye.setPixelColor(204, colorRed);
  lefteye.setPixelColor(205, colorRed);
  lefteye.setPixelColor(206, colorRed);
  lefteye.setPixelColor(208, colorRed);
  lefteye.setPixelColor(210, colorWhite);
  lefteye.setPixelColor(211, colorWhite);
  lefteye.setPixelColor(214, colorRed);
  lefteye.setPixelColor(215, colorRed);
  lefteye.setPixelColor(216, colorRed);
  lefteye.setPixelColor(218, colorWhite);
  lefteye.setPixelColor(219, colorWhite);
  lefteye.setPixelColor(220, colorWhite);
  lefteye.setPixelColor(224, colorRed);
  lefteye.setPixelColor(225, colorRed);
  lefteye.setPixelColor(227, colorWhite);
  lefteye.setPixelColor(228, colorWhite);
  lefteye.setPixelColor(229, colorWhite);
  lefteye.setPixelColor(230, colorWhite);
  lefteye.setPixelColor(231, colorWhite);
  lefteye.setPixelColor(232, colorRed);
  lefteye.setPixelColor(233, colorRed);
  lefteye.setPixelColor(236, colorWhite);
  lefteye.setPixelColor(237, colorWhite);
  lefteye.setPixelColor(238, colorWhite);
  lefteye.setPixelColor(240, colorRed);
  lefteye.setPixelColor(241, colorRed);
  lefteye.setPixelColor(242, colorRed);
  lefteye.setPixelColor(244, colorWhite);
  lefteye.setPixelColor(245, colorWhite);
  lefteye.setPixelColor(250, colorRed);
  lefteye.setPixelColor(252, colorWhite);
  righteye.setPixelColor(3, colorWhite);
  righteye.setPixelColor(5, colorRed);
  righteye.setPixelColor(10, colorWhite);
  righteye.setPixelColor(11, colorWhite);
  righteye.setPixelColor(13, colorRed);
  righteye.setPixelColor(14, colorRed);
  righteye.setPixelColor(15, colorRed);
  righteye.setPixelColor(17, colorWhite);
  righteye.setPixelColor(18, colorWhite);
  righteye.setPixelColor(19, colorWhite);
  righteye.setPixelColor(22, colorRed);
  righteye.setPixelColor(23, colorRed);
  righteye.setPixelColor(24, colorWhite);
  righteye.setPixelColor(25, colorWhite);
  righteye.setPixelColor(26, colorWhite);
  righteye.setPixelColor(27, colorWhite);
  righteye.setPixelColor(28, colorWhite);
  righteye.setPixelColor(30, colorRed);
  righteye.setPixelColor(31, colorRed);
  righteye.setPixelColor(35, colorWhite);
  righteye.setPixelColor(36, colorWhite);
  righteye.setPixelColor(37, colorWhite);
  righteye.setPixelColor(39, colorRed);
  righteye.setPixelColor(40, colorRed);
  righteye.setPixelColor(41, colorRed);
  righteye.setPixelColor(44, colorWhite);
  righteye.setPixelColor(45, colorWhite);
  righteye.setPixelColor(47, colorRed);
  righteye.setPixelColor(49, colorRed);
  righteye.setPixelColor(50, colorRed);
  righteye.setPixelColor(51, colorRed);
  righteye.setPixelColor(57, colorRed);
  righteye.setPixelColor(58, colorRed);
  righteye.setPixelColor(59, colorRed);
  righteye.setPixelColor(60, colorRed);
  righteye.setPixelColor(61, colorRed);
  righteye.setPixelColor(65, colorRed);
  righteye.setPixelColor(66, colorRed);
  righteye.setPixelColor(67, colorRed);
  righteye.setPixelColor(68, colorRed);
  righteye.setPixelColor(69, colorRed);
  righteye.setPixelColor(73, colorRed);
  righteye.setPixelColor(74, colorRed);
  righteye.setPixelColor(75, colorRed);
  righteye.setPixelColor(80, colorRed);
  righteye.setPixelColor(81, colorRed);
  righteye.setPixelColor(84, colorWhite);
  righteye.setPixelColor(85, colorWhite);
  righteye.setPixelColor(87, colorRed);
  righteye.setPixelColor(91, colorWhite);
  righteye.setPixelColor(92, colorWhite);
  righteye.setPixelColor(93, colorWhite);
  righteye.setPixelColor(95, colorRed);
  righteye.setPixelColor(96, colorWhite);
  righteye.setPixelColor(97, colorWhite);
  righteye.setPixelColor(98, colorWhite);
  righteye.setPixelColor(99, colorWhite);
  righteye.setPixelColor(100, colorWhite);
  righteye.setPixelColor(102, colorRed);
  righteye.setPixelColor(103, colorRed);
  righteye.setPixelColor(105, colorWhite);
  righteye.setPixelColor(106, colorWhite);
  righteye.setPixelColor(107, colorWhite);
  righteye.setPixelColor(110, colorRed);
  righteye.setPixelColor(111, colorRed);
  righteye.setPixelColor(114, colorWhite);
  righteye.setPixelColor(115, colorWhite);
  righteye.setPixelColor(117, colorRed);
  righteye.setPixelColor(118, colorRed);
  righteye.setPixelColor(119, colorRed);
  righteye.setPixelColor(123, colorWhite);
  righteye.setPixelColor(125, colorRed);
  righteye.setPixelColor(131, colorWhite);
  righteye.setPixelColor(133, colorRed);
  righteye.setPixelColor(138, colorWhite);
  righteye.setPixelColor(139, colorWhite);
  righteye.setPixelColor(141, colorRed);
  righteye.setPixelColor(142, colorRed);
  righteye.setPixelColor(143, colorRed);
  righteye.setPixelColor(145, colorWhite);
  righteye.setPixelColor(146, colorWhite);
  righteye.setPixelColor(147, colorWhite);
  righteye.setPixelColor(150, colorRed);
  righteye.setPixelColor(151, colorRed);
  righteye.setPixelColor(152, colorWhite);
  righteye.setPixelColor(153, colorWhite);
  righteye.setPixelColor(154, colorWhite);
  righteye.setPixelColor(155, colorWhite);
  righteye.setPixelColor(156, colorWhite);
  righteye.setPixelColor(158, colorRed);
  righteye.setPixelColor(159, colorRed);
  righteye.setPixelColor(163, colorWhite);
  righteye.setPixelColor(164, colorWhite);
  righteye.setPixelColor(165, colorWhite);
  righteye.setPixelColor(167, colorRed);
  righteye.setPixelColor(168, colorRed);
  righteye.setPixelColor(169, colorRed);
  righteye.setPixelColor(172, colorWhite);
  righteye.setPixelColor(173, colorWhite);
  righteye.setPixelColor(175, colorRed);
  righteye.setPixelColor(177, colorRed);
  righteye.setPixelColor(178, colorRed);
  righteye.setPixelColor(179, colorRed);
  righteye.setPixelColor(185, colorRed);
  righteye.setPixelColor(186, colorRed);
  righteye.setPixelColor(187, colorRed);
  righteye.setPixelColor(188, colorRed);
  righteye.setPixelColor(189, colorRed);
  righteye.setPixelColor(193, colorRed);
  righteye.setPixelColor(194, colorRed);
  righteye.setPixelColor(195, colorRed);
  righteye.setPixelColor(196, colorRed);
  righteye.setPixelColor(197, colorRed);
  righteye.setPixelColor(201, colorRed);
  righteye.setPixelColor(202, colorRed);
  righteye.setPixelColor(203, colorRed);
  righteye.setPixelColor(208, colorRed);
  righteye.setPixelColor(209, colorRed);
  righteye.setPixelColor(212, colorWhite);
  righteye.setPixelColor(213, colorWhite);
  righteye.setPixelColor(215, colorRed);
  righteye.setPixelColor(219, colorWhite);
  righteye.setPixelColor(220, colorWhite);
  righteye.setPixelColor(221, colorWhite);
  righteye.setPixelColor(223, colorRed);
  righteye.setPixelColor(224, colorWhite);
  righteye.setPixelColor(225, colorWhite);
  righteye.setPixelColor(226, colorWhite);
  righteye.setPixelColor(227, colorWhite);
  righteye.setPixelColor(228, colorWhite);
  righteye.setPixelColor(230, colorRed);
  righteye.setPixelColor(231, colorRed);
  righteye.setPixelColor(233, colorWhite);
  righteye.setPixelColor(234, colorWhite);
  righteye.setPixelColor(235, colorWhite);
  righteye.setPixelColor(238, colorRed);
  righteye.setPixelColor(239, colorRed);
  righteye.setPixelColor(242, colorWhite);
  righteye.setPixelColor(243, colorWhite);
  righteye.setPixelColor(245, colorRed);
  righteye.setPixelColor(246, colorRed);
  righteye.setPixelColor(247, colorRed);
  righteye.setPixelColor(251, colorWhite);
  righteye.setPixelColor(253, colorRed);

  righteye.show();
  lefteye.show();
  
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
  Face023RunningCheck = false; // Face023 
}
