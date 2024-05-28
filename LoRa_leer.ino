#include <SPI.h>
#include <LoRa.h> 
String inString = "";    // string to hold input
String message = "";
int val = 0;
int rssi = 0;
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(433E6)) { // or 915E6
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}
 
void loop() {
  
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    // read packet    
    while (LoRa.available())
    {
      int inChar = LoRa.read();
      inString += (char)inChar;
      message= inString; 
            
    }
    
    inString = "";     
    LoRa.packetRssi(); 
    int rssi = LoRa.packetRssi();   
  }
    Serial.print(message);   
}
