/*

  This example shows how to connect to an EBYTE transceiver
  using a Teensy 3.2

  This code for for the receiver


  connections
  Module      Teensy
  M0          2
  M1          3
  Rx          1 (MCU Tx line)
  Tx          0 (MCU Rx line)
  Aux         4
  Vcc         3V3
  Gnd         Gnd

*/

#include "EBYTE.h"

// connect to any of the Teensy Serial ports
#define ESerial Serial1

#define PIN_M0 2
#define PIN_M1 3
#define PIN_AX 4

// i recommend putting this code in a .h file and including it
// from both the receiver and sender modules

// these are just dummy variables, replace with your own
struct DATA {
  unsigned long Count;
  int Bits;
  float Volts;
  float Amps;

};

int Chan;
DATA MyData;
unsigned long Last;

// create the transceiver object, passing in the serial and pins
EBYTE Transceiver(&ESerial, PIN_M0, PIN_M1, PIN_AX);

void setup() {

  Serial.begin(9600);

  // wait for the serial to connect
  while (!Serial) {}

  // start the transceiver serial port--i have yet to get a different
  // baud rate to work--data sheet says to keep on 9600

  ESerial.begin(9600);

  Serial.println("Starting Reader");

  // this init will set the pinModes for you
  Transceiver.init();

  // all these calls are optional but shown to give examples of what you can do

  // Serial.println(Transceiver.GetAirDataRate());

  // Transceiver.SetAddressH(4);
  // Transceiver.SetAddressL(0);
  // Chan = 15;
  // Transceiver.SetChannel(Chan);
  // save the parameters to the unit,
  // Transceiver.SaveParameters(PERMANENT);

  // you can print all parameters and is good for debugging
  // if your units will not communicate, print the parameters
  // for both sender and receiver and make sure air rates, channel
  // and address is the same
  // Transceiver.PrintParameters();

}

void loop() {

  // if the transceiver serial is available, proces incoming data
  // you can also use ESerial.available()
  if (Transceiver.available()) {

    // i highly suggest you send data using structures and not
    // a parsed data--i've always had a hard time getting reliable data using
    // a parsing method
    Transceiver.GetStruct(&MyData, sizeof(MyData));

    // dump out what was just received
    Serial.print("Count: "); Serial.println(MyData.Count);
    Serial.print("Bits: "); Serial.println(MyData.Bits);
    Serial.print("Volts: "); Serial.println(MyData.Volts);
    // if you got data, update the checker
    Last = millis();
  }
  else {
    // if the time checker is over some prescribed amount
    // let the user know there is no incoming data
    if ((millis() - Last) > 1000) {
      Serial.println("Searching: ");
      Last = millis();
    }

  }

}
