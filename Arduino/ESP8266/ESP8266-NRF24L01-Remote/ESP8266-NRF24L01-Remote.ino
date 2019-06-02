#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Esp.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <Keypad_MC17.h>
#include <Keypad.h>
#include <Wire.h>

// Keypad Configuration

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

//initialize an instance of class NewKeypad
Keypad_MC17 customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR);

// NRF24L01+ Radio Configuration

RF24 radio(16, 15);
byte address[6] = "01337";

// Wi-Fi Configuration

MDNSResponder mdns;
const char *ssid = "*";
const char *password = "*";

// HTTP Server Configuration - Part 1

ESP8266WebServer server(80);

String webPage = "";


void setup()
{

// HTTP Server Configuration - Part 2
// Build the web page
  webPage += "<HEAD>";
  webPage +="<META HTTP-EQUIV=\"refresh\" CONTENT=\"15;URL=/\">";  
  webPage += "<title>Wrench Mask LED Control</title>";
  webPage += "</HEAD>";
  webPage += "<body bgcolor=black text=white>";
  webPage += "<center>";
  webPage += "<h1>Wrench Mask LED Control</h1>";
// Color Options
  webPage += "<p><a href=\"Red\"><button style=\"height:100px;width:250px;font-size:50px;color:red\">RED</button></a>";
  webPage += "<a href=\"Green\"><button style=\"height:100px;width:250px;font-size:50px;color:green\">GREEN</button></a>";
  webPage += "<a href=\"Blue\"><button style=\"height:100px;width:250px;font-size:50px;color:blue\">BLUE</button></a>";
  webPage += "<a href=\"White\"><button style=\"height:100px;width:250px;font-size:50px;color:white\">WHITE</button></a></p>";
// Face Options 1-3
  webPage += "<p>";
  webPage += "<a href=\"Face001\"><button style=\"height:100px;width:250px;font-size:50px\">X_X</button></a>";
  webPage += "<a href=\"Face002\"><button style=\"height:100px;width:250px;font-size:50px\">>_<</button></a>";
  webPage += "<a href=\"Face003\"><button style=\"height:100px;width:250px;font-size:50px\">^_^</button></a>";
  webPage += "</p>";
// Face Options 4-6
  webPage += "<p>";
  webPage += "<a href=\"Face004\"><button style=\"height:100px;width:250px;font-size:50px\">@_@</button></a>";
  webPage += "<a href=\"Face005\"><button style=\"height:100px;width:250px;font-size:50px\">/_\\</button></a>";
  webPage += "<a href=\"Face006\"><button style=\"height:100px;width:250px;font-size:50px\">?_?</button></a>";
  webPage += "</p>";
// Face Options 7-9
  webPage += "<p>";
  webPage += "<a href=\"Face007\"><button style=\"height:100px;width:250px;font-size:50px\">O_o</button></a>";
  webPage += "<a href=\"Face008\"><button style=\"height:100px;width:250px;font-size:50px\">\\_/</button></a>";
  webPage += "<a href=\"Face009\"><button style=\"height:100px;width:250px;font-size:50px\">Z_Z</button></a>";
  webPage += "</p>";
// Face Options 10-12
  webPage += "<p>";
  webPage += "<a href=\"Face010\"><button style=\"height:100px;width:250px;font-size:50px\">#_#</button></a>";
  webPage += "<a href=\"Face011\"><button style=\"height:100px;width:250px;font-size:50px\">=_=</button></a>";
  webPage += "<a href=\"Face012\"><button style=\"height:100px;width:250px;font-size:50px\">~_^</button></a>";
  webPage += "</p>";
// Face Options 13-15
  webPage += "<p>";
  webPage += "<a href=\"Face013\"><button style=\"height:100px;width:250px;font-size:50px\">9_9</button></a>";
  webPage += "<a href=\"Face014\"><button style=\"height:100px;width:250px;font-size:50px\">!_!</button></a>";
  webPage += "<a href=\"Face015\"><button style=\"height:100px;width:250px;font-size:50px\">*_*</button></a>";
  webPage += "</p>";
// Face Options 16-18
  webPage += "<p>";
  webPage += "<a href=\"Face016\"><button style=\"height:100px;width:250px;font-size:50px\">'_'</button></a>";
  webPage += "<a href=\"Face017\"><button style=\"height:100px;width:250px;font-size:50px\">;_;</button></a>";
  webPage += "<a href=\"Face018\"><button style=\"height:100px;width:250px;font-size:50px\">.)_.)</button></a>";
  webPage += "</p>";
// Face Options 19-21
  webPage += "<p>";
  webPage += "<a href=\"Face019\"><button style=\"height:100px;width:250px;font-size:50px\">(._(.</button></a>";
  webPage += "<a href=\"Face020\"><button style=\"height:100px;width:250px;font-size:50px\">o_O</button></a>";
  webPage += "<a href=\"Face021\"><button style=\"height:100px;width:250px;font-size:50px\">._.</button></a>";
  webPage += "</p>";
// Face Options 22-23
  webPage += "<p>";
  webPage += "<a href=\"Face022\"><button style=\"height:100px;width:250px;font-size:50px\">&#8976_&#8976</button></a>";
  webPage += "<a href=\"Face023\"><button style=\"height:100px;width:250px;font-size:50px\"><3_<3</button></a>";
  webPage += "</p>"; 

// Initialize all the things

  delay(1000);
  Wire.begin( );
  customKeypad.begin( );
  Serial.begin(115200);
  Serial.println("Starting Remote");
  Serial.println("");
  Serial.print("Initializing NRF24 Radio...");
  radio.begin();
  radio.openWritingPipe(address); 
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
  Serial.println("Done");
//  radio.printDetails();
  Serial.print("Initializing WiFi Radio..");
  WiFi.begin(ssid, password);
// Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Done");
  Serial.println("");
  Serial.print("Connected to: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
  if (mdns.begin("esp8266", WiFi.localIP())) 
  {
    Serial.println("MDNS responder started");
  }
  Serial.println("");
  Serial.println("Remote Startup Complete");

// HTTP Server Configuration - Part 3

  server.on("/", []()
  {
    server.send(200, "text/html", webPage);
  });
  server.on("/Red", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("HTTP Button Press Detected");
    Serial.println("Button: RED");
    Serial.println("Setting Color to Red");
    char ColorRed = 'R';
    radio.write(&ColorRed, sizeof(ColorRed));
  });
  server.on("/Green", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("HTTP Button Press Detected");
    Serial.println("Button: GREEN");
    Serial.println("Setting Color to Green");
    char ColorGreen = 'G';
    radio.write(&ColorGreen, sizeof(ColorGreen));
  });
  server.on("/Blue", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("HTTP Button Press Detected");
    Serial.println("Button: BLUE");
    Serial.println("Setting Color to Blue");
    char ColorBlue = 'B';
    radio.write(&ColorBlue, sizeof(ColorBlue));
  });
  server.on("/White", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("HTTP Button Press Detected");
    Serial.println("Button: WHITE");
    Serial.println("Setting Color to White");
    char ColorWhite = 'W';
    radio.write(&ColorWhite, sizeof(ColorWhite));
  });
  server.on("/Face001", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 001");
    char Face001 = '1' ;
    radio.write(&Face001, sizeof(Face001));
  });
  server.on("/Face002", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 002");
    char Face002 = '2';
    radio.write(&Face002, sizeof(Face002));
  });
  server.on("/Face003", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 003");
    char Face003 = '3';
    radio.write(&Face003, sizeof(Face003));
  });
  server.on("/Face004", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 004");
    char Face004 = '4';
    radio.write(&Face004, sizeof(Face004));
  });
  server.on("/Face005", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 005");
    char Face005 = '5';
    radio.write(&Face005, sizeof(Face005));
  });
  server.on("/Face006", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 006");
    char Face006 = '6';
    radio.write(&Face006, sizeof(Face006));
  });
  server.on("/Face007", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 007");
    char Face007 = '7';
    radio.write(&Face007, sizeof(Face007));
  });
  server.on("/Face008", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 008");
    char Face008 = '8';
    radio.write(&Face008, sizeof(Face008));
  });
  server.on("/Face009", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 009");
    char Face009 = '9';
    radio.write(&Face009, sizeof(Face009));
  });
  server.on("/Face010", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 010");
    char Face010 = '0';
    radio.write(&Face010, sizeof(Face010));
  });
  server.on("/Face011", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 011");
    char Face011 = 'A';
    radio.write(&Face011, sizeof(Face011));
  });
  server.on("/Face012", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 012");
    char Face012 = 'I';
    radio.write(&Face012, sizeof(Face012));
  });
  server.on("/Face013", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 013");
    char Face013 = 'C';
    radio.write(&Face013, sizeof(Face013));
  });
  server.on("/Face014", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 014");
    char Face014 = 'D';
    radio.write(&Face014, sizeof(Face014));
  });
  server.on("/Face015", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 015");
    char Face015 = 'E';
    radio.write(&Face015, sizeof(Face015));
  });
  server.on("/Face016", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 016");
    char Face016 = 'F';
    radio.write(&Face016, sizeof(Face016));
  });
  server.on("/Face017", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 017");
    char Face017 = 'H';
    radio.write(&Face017, sizeof(Face017));
  });
  server.on("/Face018", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 018");
    char Face018 = 'J';
    radio.write(&Face018, sizeof(Face018));
  });
  server.on("/Face019", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 019");
    char Face019 = 'K';
    radio.write(&Face019, sizeof(Face019));
  });
  server.on("/Face020", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 020");
    char Face020 = 'L';
    radio.write(&Face020, sizeof(Face020));
  });
  server.on("/Face021", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 021");
    char Face021 = 'M';
    radio.write(&Face021, sizeof(Face021));
  });
  server.on("/Face022", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 022");
    char Face022 = 'N';
    radio.write(&Face022, sizeof(Face022));
  });
  server.on("/Face023", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("Drawing Face 023");
    char Face023 = 'O';
    radio.write(&Face023, sizeof(Face023));
  });
  server.begin();
  Serial.println("HTTP server started");
}
  
void loop(){
  server.handleClient();
  char key = customKeypad.getKey();
  
  if (key != NO_KEY){
    Serial.println("Keypad Button Press Detected");
    Serial.print("Button : ");
    Serial.println(key);
    radio.write(&key, sizeof(key));
    if (key == '1'){
      Serial.println("Drawing Face 001");
    }
    else if(key == '2'){
      Serial.println("Drawing Face 002");
    }
    else if(key == '3'){
      Serial.println("Drawing Face 003");
    }
    else if(key == '4'){
      Serial.println("Drawing Face 004");
    }
    else if(key == '5'){
      Serial.println("Drawing Face 005");
    }
    else if(key == '6'){
      Serial.println("Drawing Face 006");
    }
    else if(key == '7'){
      Serial.println("Drawing Face 007");
    }
    else if(key == '8'){
      Serial.println("Drawing Face 008");
    }
    else if(key == '9'){
      Serial.println("Drawing Face 009");
    }
    else if(key == '0'){
      Serial.println("Drawing Face 010");
    }
    else if(key == 'A'){
      Serial.println("Drawing Face 011");
    }
    else if(key == 'I'){
      Serial.println("Drawing Face 011");
    }
    else if(key == 'C'){
      Serial.println("Drawing Face 013");
    }
    else if(key == 'D'){
      Serial.println("Drawing Face 014");
    }
    else if(key == 'E'){
      Serial.println("Drawing Face 015");
    }
    else if(key == 'F'){
      Serial.println("Drawing Face 016");
    }
    else if(key == 'H'){
      Serial.println("Drawing Face 017");
    }
    else if(key == 'J'){
      Serial.println("Drawing Face 018");
    }
    else if(key == 'K'){
      Serial.println("Drawing Face 019");
    }
    else if(key == 'L'){
      Serial.println("Drawing Face 020");
    }
    else if(key == 'M'){
      Serial.println("Drawing Face 021");
    }
    else if(key == 'N'){
      Serial.println("Drawing Face 022");
    }
    else if(key == 'O'){
      Serial.println("Drawing Face 023");
    }
    else if(key == 'R'){
      Serial.println("Setting Color to Red");
    }
    else if(key == 'G'){
      Serial.println("Setting Color to Greed");
    }
    else if(key == 'B'){
      Serial.println("Setting Color to Blue");
    }
    else if(key == 'W'){
      Serial.println("Setting Color to White");
    }
  }
  yield();
}
