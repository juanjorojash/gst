#include <SPI.h>
#include <LoRa.h>

uint8_t packet[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0xFF};
uint8_t i = 0;
bool send = false;
char val = {};
#define message "Listen to Bad Bunny!!"

void setup() {
  // serial configuration
  Serial.begin(9600);
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

  delay(500);
}

void loop() {
  // 16 byte
  i++;
  val = Serial.read();
  switch (val){
    case 0x74: // t (transmit)
      send = true;
      break;
    case 0x72: // r (receive)
      send = false;
      break;
  }
  
  if (send){
    Serial.print("Sending... ");
    Serial.println(message);
    LoRa.beginPacket();
    //LoRa.write(packet,16);
    LoRa.print(message);
    LoRa.endPacket();
    Serial.println("LoRa packet transmitted");
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

  delay(5000);

  

}
