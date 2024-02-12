/*

  This example shows how to connect to an EBYTE transceiver
  using an ESP32


  This example shows how to connect to an EBYTE transceiver
  using an ESP32

  This code for for the sender

  ESP32 won't allow SoftwareSerial (at least I can't get that lib to work
  so you will just hardwire your EBYTE directly to the Serial2 port


*/

#include "EBYTE.h"


/*
WARNING: IF USING AN ESP32
DO NOT USE THE PIN NUMBERS PRINTED ON THE BOARD
YOU MUST USE THE ACTUAL GPIO NUMBER
*/
#define PIN_RX 16   // Serial2 RX (connect this to the EBYTE Tx pin)
#define PIN_TX 17   // Serial2 TX pin (connect this to the EBYTE Rx pin)

#define PIN_M0 4    // D4 on the board (possibly pin 24)
#define PIN_M1 22   // D2 on the board (possibly called pin 22)
#define PIN_AX 21   // D15 on the board (possibly called pin 21)


// i recommend putting this code in a .h file and including it
// from both the receiver and sender modules
struct DATA {
  unsigned long Count;
  int Bits;
  float Volts;
  float Amps;

};

// these are just dummy variables, replace with your own
int Chan;
DATA MyData;
unsigned long Last;


// create the transceiver object, passing in the serial and pins
EBYTE Transceiver(&Serial2, PIN_M0, PIN_M1, PIN_AX);

void setup() {


  Serial.begin(9600);

  Serial2.begin(9600);

  Serial.println("Starting Reader");

  // this init will set the pinModes for you
  Serial.println(Transceiver.init());

  // all these calls are optional but shown to give examples of what you can do

  // Serial.println(Transceiver.GetAirDataRate());
  // Serial.println(Transceiver.GetChannel());
  // Transceiver.SetAddressH(1);
  // Transceiver.SetAddressL(1);
  // Chan = 15;
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

    // note, you only really need this library to program these EBYTE units
    // you can call write directly on the EBYTE Serial object
    // Serial2.write((uint8_t*) &Data, PacketSize );


  // let the use know something was sent
  Serial.print("Sending: "); Serial.println(MyData.Count);
  delay(1000);


}
