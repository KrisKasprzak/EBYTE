/*
  The MIT License (MIT)
  Copyright (c) 2019 Kris Kasrpzak
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  On a personal note, if you develop an application or product using this library 
  and make millions of dollars, I'm happy for you!
*/

/* 
  Code by Kris Kasprzak kris.kasprzak@yahoo.com
  This library is intended to be used with EBYTE transcievers, small wireless units for MCU's such as
  Teensy and Arduino. This library let's users program the operating parameters and both send and recieve data.
  This company makes several modules with different capabilities, but most #defines here should be compatible with them
  All constants were extracted from several data sheets and listed in binary as that's how the data sheet represented each setting
  Hopefully, any changes or additions to constants can be a matter of copying the data sheet constants directly into these #defines
  Usage of this library consumes around 970 bytes
  Revision		Data		Author			Description
  1.0			3/6/2019	Kasprzak		Initial creation
  2.0			10/21/2019	Kasprzak		modified code to allow no MO or M1 for use with limited wires
   
*/

#include <EBYTE.h>
#include <Stream.h>

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/*
create the transciever object
*/

EBYTE::EBYTE(Stream *s, int8_t PIN_M0, int8_t PIN_M1, int8_t PIN_AUX, unsigned long ReadTimeout )

{

	_prog = true;

	_s = s;
	_M0 = PIN_M0;
	_M1 = PIN_M1;
	_AUX = PIN_AUX;
	_rt = ReadTimeout;

	if ((_M0 == -1) & (_M1 == -1)){
		_prog = false;
	}

		
}

/*
Initialize the unit--basicall this reads the modules parameters and stores the parameters
for potential future module programming
*/

bool EBYTE::init() {

	bool ok = true;

	if (_prog){

		pinMode(_AUX, INPUT);
		pinMode(_M0, OUTPUT);
		pinMode(_M1, OUTPUT);
		SetMode(MODE_NORMAL);
	}

	_s->setTimeout(_rt);


	// first get the module data (must be called first for some odd reason
	if (_prog){
		ok = ReadModelData();
	}

	if (!ok) {
		return false;
	}

	// now get parameters to put unit defaults into the class variables
	if (_prog){
		ok = ReadParameters();
	}

	if (!ok) {
		return false;
	}

	return true;
}


/*
Method to indicate availability
*/

bool EBYTE::available() {

	return _s->available();
	
}

/*
Method to indicate availability
*/

void EBYTE::flush() {

	_s->flush();
	
}

/*
Method to write a single byte...not sure how useful this really is. If you need to send 
more that one byte, put the data into a data structure and send it in a big chunk
*/

void EBYTE::SendByte( uint8_t TheByte) {

	_s->write(TheByte);
	
}


/*
Method to get a single byte...not sure how useful this really is. If you need to get 
more that one byte, put the data into a data structure and send/receive it in a big chunk
*/

uint8_t EBYTE::GetByte() {

	return _s->read();

}


/*
Method to send a chunk of data provided data is in a struct--my personal favorite as you 
need not parse or worry about sprintf() inability to handle floats
TTP: put your structure definition into a .h file and include in both the sender and reciever
sketches
NOTE: of your sender and receiver MCU's are different (Teensy and Arduino) caution on the data
types each handle ints floats differently
*/

bool EBYTE::SendStruct(const void *TheStructure, uint16_t size_) {

		_buf = _s->write((uint8_t *) TheStructure, size_);
		
		CompleteTask(1000);
		
		return (_buf == size_);

}


/*
Method to get a chunk of data provided data is in a struct--my personal favorite as you 
need not parse or worry about sprintf() inability to handle floats
TTP: put your structure definition into a .h file and include in both the sender and reciever
sketches
NOTE: of your sender and receiver MCU's are different (Teensy and Arduino) caution on the data
types each handle ints floats differently
*/


bool EBYTE::GetStruct(const void *TheStructure, uint16_t size_) {
	
	_buf = _s->readBytes((uint8_t *) TheStructure, size_);

	CompleteTask(1000);

	return (_buf == size_);
}


/*
Utility method to wait until module is doen tranmitting
a timeout is provided to avoid an infinite loop
*/

void EBYTE::CompleteTask(unsigned long timeout) {

	unsigned long t = millis();

	// make darn sure millis() is not about to reach max data type limit and start over
	if (((unsigned long) (t + timeout)) == 0){
		t = 0;
	}

	// if AUX pin was supplied and look for HIGH state
	// note you can omit using AUX if no pins are available, but you will have to use delay() to let module finish
	if (_AUX != -1) {
		while (digitalRead(_AUX) == LOW) {
			if ((millis() - t) > timeout){
				break;
			}
		}
	}
	else {
		// if you can't use aux pin, use 4K7 pullup with Arduino
		// you may need to adjust this value if transmissions fail
		SmartDelay(100);

	}


	// per data sheet control after aux goes high is 2ms so delay for at least that long)
	SmartDelay(20);
	
}

/*
method to set the mode (program, normal, etc.)
*/

void EBYTE::SetMode(uint8_t mode) {
	
	// data sheet claims module needs some extra time after mode setting (2ms)
	// most of my projects uses 10 ms, but 40ms is safer
	if (!_prog){
		//Serial.println("M0 and M1 pins set to -1, mode setting can't be done");
		return;
	}

	SmartDelay(40);
	
	if (mode == MODE_NORMAL) {
		digitalWrite(_M0, LOW);
		digitalWrite(_M1, LOW);
	}
	else if (mode == MODE_WAKEUP) {
		digitalWrite(_M0, HIGH);
		digitalWrite(_M1, LOW);
	}
	else if (mode == MODE_POWERDOWN) {
		digitalWrite(_M0, LOW);
		digitalWrite(_M1, HIGH);
	}
	else if (mode == MODE_PROGRAM) {
		digitalWrite(_M0, HIGH);
		digitalWrite(_M1, HIGH);
	}

	// data sheet says 2ms later control is returned, let's give just a bit more time
	// these modules can take time to activate pins
	SmartDelay(40);
	
	// wait until aux pin goes back low
	CompleteTask(1000);
	
}


/*
delay() in a library is not a good idea as it can stop interrupts
just poll internal time until timeout is reached
*/


void EBYTE::SmartDelay(unsigned long timeout) {
	
	unsigned long t = millis();
	
	// make darn sure millis() is not about to reach max data type limit and start over
	if (((unsigned long) (t + timeout)) == 0){
		t = 0;
	}
	
	while ((millis() - t) < timeout) 	{ 	}
	
}

/*
ya get in a bind and can't remember the factory defaults, call this
NOTE: EBYTE modules for 100mW, 500mW and 1W will have different defaults
Per the data sheet just send 3x 0x4C to the unit
you will need to chanage mainly the Air data rate and power options--see the .h file
you would think we could look at _version and set defaults but version seems to not indicate the power
*/
void EBYTE::Reset() {

	// debug later.. i've yet to get this process to work
	/*
	SetMode(MODE_PROGRAM);
	_s->write(0xC4);
	_s->write(0xC4);
	_s->write(0xC4);
	SetMode(MODE_NORMAL);
	ReadParameters();
	SaveParameters(PERMANENT);
	*/


}


/*
method to set the high bit of the address
*/


void EBYTE::SetAddressH(uint8_t val) {
	_AddressHigh = val;
}

/*
method to set the lo bit of the address
*/

void EBYTE::SetAddressL(uint8_t val) {
	_AddressLow = val;
}


/*
method to set the channel
*/

void EBYTE::SetChannel(uint8_t val) {
	_Channel = val;
}

/*
method to set the air data rate
*/

void EBYTE::SetAirDataRate(uint8_t val) {
	_AirDataRate = val;
	BuildSpeedByte();
}

/*
method to set the transmit power
*/

void EBYTE::SetTransmitPower(uint8_t val) {
	_OptionPower = val;
	BuildOptionByte();
}

/*
method to set the parity bit
*/


void EBYTE::SetParityBit(uint8_t val) {
	_ParityBit = val;
	BuildSpeedByte();
}

/*
method to set the wake up on timing bit
*/

void EBYTE::SetWORTIming(uint8_t val) {
	_OptionWakeup = val;
	BuildOptionByte();
}


/*
method to compute the address based on high and low bits
*/

void EBYTE::SetAddress(uint16_t Val) {
	_AddressHigh = ((Val & 0xFFFF) >> 8);
	_AddressLow = (Val & 0xFF);
}


/*
method to get the address which is a collection of hi and lo bytes
*/


uint16_t EBYTE::GetAddress() {
	return (_AddressHigh << 8) | (_AddressLow );
}

/*
set the UART baud rate
*/

void EBYTE::SetUARTBaudRate(uint8_t val) {
	_UARTDataRate = val;
	BuildSpeedByte();
}


/*
method to build the byte for programming (notice it's a collection of a few variables)
*/


void EBYTE::BuildSpeedByte() {
	_Speed = ((_ParityBit & 0xFF) << 6) | ((_UARTDataRate & 0xFF) << 3) | (_AirDataRate);
}


/*
method to build the option byte for programming (notice it's a collection of a few variables)
*/

void EBYTE::BuildOptionByte() {
	_Options = ((_OptionTrans & 0xFF) << 7) | ((_OptionPullup & 0xFF) << 6) | ((_OptionWakeup & 0xFF) << 3) | ((_OptionFEC & 0xFF) << 2) | (_OptionPower);
}


/*
method to save parameters to the module
*/

void EBYTE::SaveParameters(uint8_t val) {
	
	if (!_prog){
		return;
	}

	SetMode(MODE_PROGRAM);

	// here you can save permanenly or temp
	_Save = val;
	_s->write(_Save);
	_s->write(_AddressHigh);
	_s->write(_AddressLow);
	_s->write(_Speed);
	_s->write(_Channel);
	_s->write(_Options);

	CompleteTask(100);

	SetMode(MODE_NORMAL);
	
}

/*
method to print parameters, this can be called anytime after init(), because init gets parameters
andy any method updates the variables
however...to make darn sure what you see is real, we'll call ReadParameters once again
*/

void EBYTE::PrintParameters() {

	if (!_prog){
		Serial.println("M0 and M1 pins set to -1, reading can't be done");
		return;
	}

	ReadParameters();		

	_ParityBit = (_Speed & 0XC0) >> 6;
	_UARTDataRate = (_Speed & 0X38) >> 3;
	_AirDataRate = _Speed & 0X07;

	_OptionTrans = (_Options & 0X80) >> 7;
	_OptionPullup = (_Options & 0X40) >> 6;
	_OptionWakeup = (_Options & 0X38) >> 3;
	_OptionFEC = (_Options & 0X07) >> 2;
	_OptionPower = (_Options & 0X03);

	Serial.println("----------------------------------------");
	Serial.print(F("Model no.: "));  Serial.println(_Model, HEX);
	Serial.print(F("Version  : "));  Serial.println(_Version, HEX);
	Serial.print(F("Features : "));  Serial.println(_Features, HEX);
	Serial.println(F(" "));
	Serial.print(F("Mode (HEX/DEC/BIN): "));  Serial.print(_Save, HEX); Serial.print(F("/"));  Serial.print(_Save, DEC); Serial.print(F("/"));  Serial.println(_Save, BIN);
	Serial.print(F("AddH (HEX/DEC/BIN): "));  Serial.print(_AddressHigh, HEX); Serial.print(F("/")); Serial.print(_AddressHigh, DEC); Serial.print(F("/"));  Serial.println(_AddressHigh, BIN);
	Serial.print(F("AddL (HEX/DEC/BIN): "));  Serial.print(_AddressLow, HEX); Serial.print(F("/")); Serial.print(_AddressLow, DEC); Serial.print(F("/"));  Serial.println(_AddressLow, BIN);
	Serial.print(F("Sped (HEX/DEC/BIN): "));  Serial.print(_Speed, HEX); Serial.print(F("/")); Serial.print(_Speed, DEC); Serial.print(F("/"));  Serial.println(_Speed, BIN);
	Serial.print(F("Chan (HEX/DEC/BIN): "));  Serial.print(_Channel, HEX); Serial.print(F("/")); Serial.print(_Channel, DEC); Serial.print(F("/"));  Serial.println(_Channel, BIN);
	Serial.print(F("Optn (HEX/DEC/BIN): "));  Serial.print(_Options, HEX); Serial.print(F("/")); Serial.print(_Options, DEC); Serial.print(F("/"));  Serial.println(_Options, BIN);
	Serial.print(F("Addr (HEX/DEC/BIN): "));  Serial.print(_Address, HEX); Serial.print(F("/")); Serial.print(_Address, DEC); Serial.print(F("/"));  Serial.println(_Address, BIN);
	Serial.println(F(" "));
	Serial.print(F("SpeedParityBit (HEX/DEC/BIN)    : "));  Serial.print(_ParityBit, HEX); Serial.print(F("/"));  Serial.print(_ParityBit, DEC); Serial.print(F("/"));  Serial.println(_ParityBit, BIN);
	Serial.print(F("SpeedUARTDataRate (HEX/DEC/BIN) : "));  Serial.print(_UARTDataRate, HEX); Serial.print(F("/"));  Serial.print(_UARTDataRate, DEC); Serial.print(F("/"));  Serial.println(_UARTDataRate, BIN);
	Serial.print(F("SpeedAirDataRate (HEX/DEC/BIN)  : "));  Serial.print(_AirDataRate, HEX); Serial.print(F("/"));  Serial.print(_AirDataRate, DEC); Serial.print(F("/"));  Serial.println(_AirDataRate, BIN);
	Serial.print(F("OptionTrans (HEX/DEC/BIN)       : "));  Serial.print(_OptionTrans, HEX); Serial.print(F("/"));  Serial.print(_OptionTrans, DEC); Serial.print(F("/"));  Serial.println(_OptionTrans, BIN);
	Serial.print(F("OptionPullup (HEX/DEC/BIN)      : "));  Serial.print(_OptionPullup, HEX); Serial.print(F("/"));  Serial.print(_OptionPullup, DEC); Serial.print(F("/"));  Serial.println(_OptionPullup, BIN);
	Serial.print(F("OptionWakeup (HEX/DEC/BIN)      : "));  Serial.print(_OptionWakeup, HEX); Serial.print(F("/"));  Serial.print(_OptionWakeup, DEC); Serial.print(F("/"));  Serial.println(_OptionWakeup, BIN);
	Serial.print(F("OptionFEC (HEX/DEC/BIN)         : "));  Serial.print(_OptionFEC, HEX); Serial.print(F("/"));  Serial.print(_OptionFEC, DEC); Serial.print(F("/"));  Serial.println(_OptionFEC, BIN);
	Serial.print(F("OptionPower (HEX/DEC/BIN)       : "));  Serial.print(_OptionPower, HEX); Serial.print(F("/"));  Serial.print(_OptionPower, DEC); Serial.print(F("/"));  Serial.println(_OptionPower, BIN);

	Serial.println("----------------------------------------");

}

/*
method to read parameters, 
*/

bool EBYTE::ReadParameters() {

	if (!_prog){
		return;
	}

	// read basic parameters
	_Params[0] = 0;
	_Params[1] = 0;
	_Params[2] = 0;
	_Params[3] = 0;
	_Params[4] = 0;
	_Params[5] = 0;

	SetMode(MODE_PROGRAM);

	_s->write(0xC1);
	_s->write(0xC1);
	_s->write(0xC1);

	_s->readBytes((uint8_t*)&_Params, (uint16_t) sizeof(_Params));

	_Save = _Params[0];
	_AddressHigh = _Params[1];
	_AddressLow = _Params[2];
	_Speed = _Params[3];
	_Channel = _Params[4];
	_Options = _Params[5];

	_Address =  (_AddressHigh << 8) | (_AddressLow);
	_ParityBit = (_Speed & 0XC0) >> 6;
	_UARTDataRate = (_Speed & 0X38) >> 3;
	_AirDataRate = _Speed & 0X07;

	_OptionTrans = (_Options & 0X80) >> 7;
	_OptionPullup = (_Options & 0X40) >> 6;
	_OptionWakeup = (_Options & 0X38) >> 3;
	_OptionFEC = (_Options & 0X07) >> 2;
	_OptionPower = (_Options & 0X03);
	
	SetMode(MODE_NORMAL);

	if (0xC0 != _Params[0]){
		
		return false;
	}

	return true;
	
}


bool EBYTE::ReadModelData() {

	if (!_prog){
		return;
	}

	_Params[0] = 0;
	_Params[1] = 0;
	_Params[2] = 0;
	_Params[3] = 0;

	SetMode(MODE_PROGRAM);

	_s->write(0xC3);
	_s->write(0xC3);
	_s->write(0xC3);

	_s->readBytes((uint8_t*)& _Params, (uint16_t) sizeof(_Params));
		
	_Model = _Params[1];
	_Version = _Params[2];
	_Features = _Params[3];

	SetMode(MODE_NORMAL);

	if (0xC3 != _Params[0]) {
		
		return false;
	}

	return true;
	
}

/*
method to get module model and E50-TTL-100 will return 50
*/

uint8_t EBYTE::GetModel() {

	return _Model;
	
}

/*
method to get module version (undocumented as to the value)
*/

uint8_t EBYTE::GetVersion() {

	return _Version;
	
}

/*
method to get module version (undocumented as to the value)
*/


uint8_t EBYTE::GetFeatures() {

	return _Features;

}
