/*








*/

#include <SoftwareSerial.h>
#include "EBYTE.h"

#define PIN_M0 4
#define PIN_M1 5
#define PIN_AX 6


int Chan;

struct DATA {
  unsigned long Count;
  int Bits;
  float Volts;
  float Amps;

};

DATA MyData;

SoftwareSerial ESerial(2, 3);

EBYTE Transceiver(&ESerial, 4, 5, 6);

void setup() {

  Serial.begin(9600);

  ESerial.begin(9600);

  Serial.println("Starting Sender");

  Transceiver.init();
  // Transceiver.SetMode(MODE_NORMAL);
  // Transceiver.Reset();

  // Transceiver.SetTransmitPower(OPT_TP20);

  // Transceiver.SetAirDataRate(ADR_8K);
  //Transceiver.SetAddressH(0);
  //Transceiver.SetAddressL(0);
  //Transceiver.SetChannel(2);
  //Transceiver.SaveParameters(PERMANENT);

  // Transceiver.PrintParameters();

  // SetWORTIming(OPT_WAKEUP250);

}

void loop() {

  MyData.Count++;
  MyData.Bits = analogRead(A0);
  MyData.Volts = MyData.Bits * ( 5.0 / 1024.0 );

  // Transceiver.SendByte('A');
  Transceiver.SendStruct(&MyData, sizeof(MyData));
  //Serial.print("Sending: "); Serial.println(MyData.Count);
  delay(1000);
  if (Serial.available()) {
    Chan = Serial.read();

    if (Chan > 47) {
      Serial.println(Chan-48);
      Transceiver.SetChannel(Chan-48);
      Transceiver.SaveParameters(PERMANENT);
      Transceiver.PrintParameters();
    }
  }

}
