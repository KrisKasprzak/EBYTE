#include <SoftwareSerial.h>
#include "EBYTE.h"
 
struct DATA {
  unsigned long Count;
  int Bits;
  float Volts;
  float Amps;

};
int Chan;
DATA MyData;

SoftwareSerial ESerial(2, 3);

EBYTE Transceiver(&ESerial, 4, 5, 6);

void setup() {

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, INPUT);

  Serial.begin(9600);

  ESerial.begin(9600);
  Serial.println("Starting Reader");

  Transceiver.init();
  // Transceiver.Reset();
  // Transceiver.SetAirDataRate(ADR_8K);
  //Transceiver.SetAddressH(0);
  //Transceiver.SetAddressL(0);
  //Transceiver.SetChannel(2);

  // Transceiver.SaveParameters(PERMANENT);

   Transceiver.PrintParameters();

}

void loop() {

  if (ESerial.available()) {

    Transceiver.GetStruct(&MyData, sizeof(MyData));

    Serial.print("Count: "); Serial.println(MyData.Count);
    Serial.print("Bits: "); Serial.println(MyData.Bits);
    Serial.print("Volts: "); Serial.println(MyData.Volts);
  }
  else {
    if (Serial.available()) {
      Chan = Serial.read();

      if (Chan > 47) {
        Serial.println(Chan - 48);
        Transceiver.SetChannel(Chan - 48);
        Transceiver.SaveParameters(PERMANENT);
        Transceiver.PrintParameters();
      }
    }
    delay(1000);
    Serial.println("Searching: ");
  }
}
