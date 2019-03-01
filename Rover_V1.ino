
/*
   Demonstrate using an http server and an HTML form to control an LED.
   The http server runs on the ESP8266.

   Connect to "http://esp8266WebForm.local" or "http://<IP address>"
   to bring up an HTML form to control the LED connected GPIO#0. This works
   for the Adafruit ESP8266 HUZZAH but the LED may be on a different pin on
   other breakout boards.

   Imperatives to turn the LED on/off using a non-browser http client.
   For example, using wget.
   $ wget http://esp8266webform.local/ledon
   $ wget http://esp8266webform.local/ledoff
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

#include "Control_system_for_wemos_V1.h"

#define UDP_PORT 8266 //Ezt a portot kell beállítani a "WiFi TCP/UDP Controller"-ben
#define debug_udp_on 198 //az UDP-n érkező adatok megjelenítése bekapcsolva
#define debug_udp_off 199 //az UDP-n érkező adatok megjelenítése kikapcsolva
#define DEBUG_UDP_DEFAULT 1 //teszt üzemben1, egyébként 0

volatile long  absCounterLeft=0;
volatile long  relCounterLeft=0;
volatile long  absCounterRight=0;
volatile long  relCounterRight=0;
boolean status_displayed=0;
boolean debug_udp=DEBUG_UDP_DEFAULT; //UDP teszteléshez
const char hostname[]="robotond"; // ezzel adunk nevet a robotunknak, és a helyi hálózaton
    //Ha nem akarod mindig megnézni az IP-címed, <hostname>.local néven is elérheted a robotot.
    //Adj neki nevet, de figyelj, hogy a domain nevekre érvényes megkötéseket vedd figylembe!
String cmsg1="Open a browser and try to connect to http://";
String cmsg2=".local or http://";
//<hostname>.local címre hivatkozva érhetjük el a böngészőben
String CONNECT_MSG=cmsg1+String(hostname)+cmsg2;

// Fill in your WiFi router SSID and password
const char* ssid = "ArduWiFi";
const char* password = "";

char charlie;
unsigned char PWM;

MDNSResponder mdns;

boolean wifiConnected = false;

// UDP variables
unsigned int localPort = UDP_PORT;
WiFiUDP UDP; //Itt hozzuk létre az UDP szerver példányát
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged"; // a string to send back

// connect to UDP – returns true if successful or false if not
boolean connectUDP() //Ez a főggvény indítja el az UDP szervert
{
  boolean state = false;
  Serial.println("");
  Serial.println("Connecting to UDP");
  if(UDP.begin(localPort) == 1){
  Serial.println("Connection successful");
  state = true;
    }
    else{
    Serial.println("Connection failed");
    }
  return state;
}



// Start of checkUDP code
char checkUDP(void) //Ezt a függvényt hívja meg a timer interrupt 10 milliszekundumonként
                    //A lehető legrövidebbre kell venni az itt elvégzendő feladatokat
                    //mert különben kiakad az ESP8266
{

    if(udpConnected)
        {
          // if there’s data available, read a packet
          int packetSize = UDP.parsePacket();
          if(packetSize) //azaz: packetSize nem nulla
            {
                UDP.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
                int udpCommand=atoi(packetBuffer);
                return(packetBuffer[0]);
          

            }
            else return 0;
      }
}
// End of checkUDP code
  void setup(void)
  {
    Serial.begin(115200);
    delay(10);
  
    //pinMode(MINA,OUTPUT);
    pinMode(MIN1,OUTPUT);
    pinMode(MIN2,OUTPUT);
  
    pinMode(MIN3,OUTPUT);
    pinMode(MIN4,OUTPUT);
    //pinMode(MINB,OUTPUT);
    
    Serial.println("Initialization starts...");

    WiFi.begin(ssid, password);
    Serial.print("Trying to connect to WiFi AP & DHCP server named: \"");
    Serial.print(ssid);
    Serial.println("\"");
    

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    if (mdns.begin(hostname, WiFi.localIP())) {
      Serial.println("MDNS responder started");
    }
    wifiConnected = WL_CONNECTED; 
    Serial.println("Tying to start UDP protocol...");
    udpConnected = connectUDP();
    if (udpConnected) Serial.println("UDP OK.");
      else Serial.println("UDP failed");
    //
   
    Serial.print(CONNECT_MSG);
    Serial.println(WiFi.localIP());
    
    Serial.println("Initialization ended.");

  }



  void loop(void)
{
// server.handleClient();

//display_status();
Terminal();
}

void Terminal()
{
  char input;

  while(1)
  {
    if(Serial.available())
    {input = Serial.read();
    switch(input)
    {
      case 'f': Forward();
                Serial.println("Forward");
                break;
                
      case 'b': Backward();
                Serial.println("Backward");
                break;
                
      case 'l': Left();
                Serial.println("Left");
                break;     
                            
      case 'r': Right();
                Serial.println("Right");
                
                break;
                                          
      case 's': Stop();
                Serial.println("Stop");
                break;                
                
    } //end switch()
    } //end if()
    charlie = checkUDP();
    if (charlie)
      {
input = Serial.read();
        switch(charlie)
        {
          case 'f': Forward();
                    Serial.println("Forward");
                    break;
                    
          case 'b': Backward();
                    Serial.println("Backward");
                    break;
                    
          case 'l': Left();
                    Serial.println("Left");
                    break;     
                                
          case 'r': Right();
                    Serial.println("Right");
                    break;
                                              
          case 's': Stop();
                    Serial.println("Stop");

                    break;
                    
        } //end switch()        
      } //end if
  } //end while()
} //end Terminal()
