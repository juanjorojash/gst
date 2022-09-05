#include <SPI.h>
#include <LoRa.h>

bool send = false;
char val = {};
String message = String('NO');

void setup() {
  // serial configuration
  Serial.begin(9600);
  Serial.setTimeout(10000);
  //while(!Serial);  //not needed in real GST

  // LoRa configuration
  if (LoRa.begin(434.25E6)) {
    Serial.println("LoRa successful!");
  }else {
    Serial.println("LoRa not successful!");
  }

  LoRa.setSpreadingFactor(8);       // from 6 to 12
  LoRa.setCodingRate4(4);           // from 4 to 8
  LoRa.setSignalBandwidth(41.7E3);  // values: 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, 250E3, and 500E3

  LoRa.setTxPower(18,PA_OUTPUT_PA_BOOST_PIN); //18dbm, 63mW

  delay(2000);
}

void loop() {
  // 16 byte
  if (Serial.available()){
    val = Serial.read();
    switch (val){
    case 0x73:
      send = true;
      break;
    case 0x72:
      send = false;
      break;
    }
  } 
  
  while(Serial.available()) {
    Serial.read();
  }

  

  
  
  if (send){
    Serial.println("Write the message:");
    message = Serial.readStringUntil('\n');
    Serial.print("Sending... ");
    Serial.println(message);
    LoRa.beginPacket();
    //LoRa.write(packet,16);
    LoRa.print(message);
    LoRa.endPacket();
    Serial.println("LoRa packet transmitted");
    send = false;
  } else{
    int packetSize = LoRa.parsePacket();
      if (packetSize) {
      // received a packet
      Serial.print("Received packet '");
  
      // read packet
      while (LoRa.available()) {
        Serial.print((char)LoRa.read());
      }
  
      // print RSSI of packet
      Serial.print("' with RSSI ");
      Serial.println(LoRa.packetRssi());
    }
  }
}
