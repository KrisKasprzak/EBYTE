/*

  This example shows how to connect to an EBYTE transceiver
  using an Arduino Nano

  This code for for the sender


  connections
  Module      Arduino
  M0          4
  M1          5
  Rx          2 (This is the MCU Tx lined)
  Tx          3 (This is the MCU Rx line)
  Aux         6
  Vcc         3V3
  Gnd         Gnd

*/

#include <SoftwareSerial.h>
#include "EBYTE.h"

#define PIN_RX 2
#define PIN_TX 3
#define PIN_M0 4
#define PIN_M1 5
#define PIN_AX 6

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

// you will need to define the pins to create the serial port
SoftwareSerial ESerial(PIN_RX, PIN_TX);


// create the transceiver object, passing in the serial and pins
EBYTE Transceiver(&ESerial, PIN_M0, PIN_M1, PIN_AX);

void setup() {

  Serial.begin(9600);

  // start the transceiver serial port--i have yet to get a different
  // baud rate to work--data sheet says to keep on 9600
  ESerial.begin(9600);

  Serial.println("Starting Sender");

  // this init will set the pinModes for you
  Transceiver.init();

  // all these calls are optional but shown to give examples of what you can do

  // Serial.println(Transceiver.GetAirDataRate());
  // Serial.println(Transceiver.GetChannel());

  // Transceiver.SetAddressH(1);
  // Transceiver.SetAddressL(0);
  // Chan = 5;
  // Transceiver.SetChannel(Chan);
  // save the parameters to the unit,
  // Transceiver.SaveParameters(PERMANENT);

  // you can print all parameters and is good for debugging
  // if your units will not communicate, print the parameters
  // for both sender and receiver and make sure air rates, channel
  // and address is the same
  Transceiver.PrintParameters();

}

void loop() {

  // measure some data and save to the structure
  MyData.Count++;
  MyData.Bits = analogRead(A0);
  MyData.Volts = MyData.Bits * ( 5.0 / 1024.0 );

  // i highly suggest you send data using structures and not
  // a parsed data--i've always had a hard time getting reliable data using
  // a parsing method
  Transceiver.SendStruct(&MyData, sizeof(MyData));
  
  // You only really need this library to program these EBYTE units. 
  // for writing data structures you can call write directly on the EBYTE Serial object
  // ESerial.write((uint8_t*) &Data, PacketSize );
  

  // let the use know something was sent
  Serial.print("Sending: "); Serial.println(MyData.Count);
  delay(1000);


}
