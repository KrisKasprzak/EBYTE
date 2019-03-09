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
  

  Module connection
  Module	MCU						Description
  MO		Any digital pin*		pin to control working/program modes
  M1		Any digital pin*		pin to control working/program modes
  Rx		Any digital pin			pin to MCU TX pin (module transmits to MCU, hence MCU must recieve data from module
  Tx		Any digital pin			pin to MCU RX pin (module transmits to MCU, hence MCU must recieve data from module
  AUX		Any digital pin			pin to indicate when an operation is complete (low is busy, high is done)
  Vcc		+3v3 or 5V0				
  Vcc		Ground					Ground must be common to module and MCU		

  notes:
  * caution in connecting to Arduino pin 0 and 1 as those pins are for USB connection to PC
  you may need a 4K7 pullup to Rx and AUX pins (possibly Tx) if using and Arduino

  Module source
  http://www.ebyte.com/en/
  example module this library is intended to be used with
  http://www.ebyte.com/en/product-view-news.aspx?id=174

  Code usage
  1. Create a serial object
  2. Create EBYTE object that uses the serail object
  3. begin the serial object
  4. init the EBYTE object
  5. set parameters (optional but required if sender and reciever are different)
  6. send or listen to sent data
  
*/


#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// modes NORMAL send and recieve for example
#define MODE_NORMAL 0			// can send and recieve
#define MODE_WAKEUP 1			// sends a preamble to waken receiver
#define MODE_POWERDOWN 2		// can't transmit but receive works only in wake up mode
#define MODE_PROGRAM 3			// for programming

// options to save change permanently or temp (power down and restart will restore settings to last saved options
#define PERMANENT 0xC0
#define TEMPORARY 0xC2

// parity bit options (must be the same for transmitter and reveiver)
#define PB_8N1 0b00			// default
#define PB_8O1 0b01
#define PB_8E1 0b11

//UART data rates
// (can be different for transmitter and reveiver)
#define UDR_1200 0b000		// 1200 baud
#define UDR_2400 0b001		// 2400 baud
#define UDR_4800 0b010		// 4800 baud
#define UDR_9600 0b011		// 9600 baud default
#define UDR_19200 0b100		// 19200 baud
#define UDR_38400 0b101		// 34800 baud
#define UDR_57600 0b110		// 57600 baud
#define UDR_115200 0b111	// 115200 baud

// air data rates (certian types of modules)
// (must be the same for transmitter and reveiver)
#define ADR_300 0b000		// 300 baud
#define ADR_1200 0b001		// 1200 baud
#define ADR_2400 0b010		// 2400 baud
#define ADR_4800 0b011		// 4800 baud
#define ADR_9600 0b100		// 9600 baud
#define ADR_19200 0b101		// 19200 baud

// air data rates (other types of modules)
#define ADR_1K 0b000		// 1k baud
#define ADR_2K 0b001		// 2K baud
#define ADR_5K 0b010		// 4K baud
#define ADR_8K 0b011		// 8K baud
#define ADR_10K 0b100		// 10K baud
#define ADR_15K 0b101		// 15K baud
#define ADR_20K 0b110		// 20K baud
#define ADR_25K 0b111		// 25K baud

// various options
// (can be different for transmitter and reveiver)
#define OPT_FMDISABLE 0b0	//default
#define OPT_FMENABLE 0b1
#define OPT_IOOPENDRAIN 0b0	 
#define OPT_IOPUSHPULL  0b1
#define OPT_WAKEUP250  0b000 
#define OPT_WAKEUP500  0b001
#define OPT_WAKEUP750  0b010
#define OPT_WAKEUP1000 0b011
#define OPT_WAKEUP1250 0b100
#define OPT_WAKEUP1500 0b101
#define OPT_WAKEUP1750 0b110
#define OPT_WAKEUP2000 0b111
#define OPT_FECDISABLE  0b0
#define OPT_FECENABLE 0b1	

// transmitter output power--check government regulations on legal transmit power
// refer to the data sheet as not all modules support these power levels
// constants for 1W units
// (can be different for transmitter and reveiver)
#define OPT_TP30 0b00		// 30 db
#define OPT_TP27 0b01		// 27 db
#define OPT_TP24 0b10		// 24 db
#define OPT_TP21 0b11		// 21 db

// constants or 500 mW units
#define OPT_TP27 0b00		// 27 db
#define OPT_TP24 0b01		// 24 db
#define OPT_TP21 0b10		// 21 db
#define OPT_TP18 0b11		// 17 db
#define OPT_TP17 0b11		// 17 db

// constants or 100 mW units
#define OPT_TP20 0b00		// 20 db
#define OPT_TP17 0b01		// 17 db
#define OPT_TP14 0b10		// 14 db
#define OPT_TP11 0b11		// 10 db
#define OPT_TP10 0b11		// 10 db



class Stream;

class EBYTE {

public:

	EBYTE(Stream *s, uint8_t PIN_M0 = 4, uint8_t PIN_M1 = 5, uint8_t PIN_AUX = 6, unsigned long ReadTimeout = 1000);

	// code to initialize the library
	// this method reads all parameters from the module and stores them
	// library modifications could be made to only read upon a change at a savings of 30 or so bytes
	// the issue with these modules are some parameters are a collection of several options AND
	// ALL parameters must be sent even if only one option is changed--hence get all parameters initially
	// so you know what the non changed parameters are know for resending back
	// seems a waste of memory and could be optimized to save a few bytes
	bool init();
	
	// methods to set modules working parameters NOTHING WILL BE SAVED UNLESS SaveParameters() is called
	void SetMode(uint8_t mode = MODE_NORMAL);
	void SetAddress(uint16_t val = 0);
	void SetAddressH(uint8_t val = 0);
	void SetAddressL(uint8_t val = 0);
	void SetAirDataRate(uint8_t val);
	void SetUARTBaudRate(uint8_t val);
	void SetChannel(uint8_t val);
	void Reset();
	void SetParityBit(uint8_t val);
	void SetTransmitPower(uint8_t val);
	void SetWORTIming(uint8_t val);
	bool available();
	void flush();
	// methods to get some operating parameters
	uint16_t GetAddress();

	// methods to get module data
	uint8_t GetModel();
	uint8_t GetVersion();
	uint8_t GetFeatures();
		
	// methods to get data from sending unit
	uint8_t GetByte();
	bool GetStruct(const void *TheStructure, uint16_t size_);
	
	// method to send to data to receiving unit
	void SendByte(uint8_t TheByte);
	bool SendStruct(const void *TheStructure, uint16_t size_);
	
	// mehod to print parameters
	void PrintParameters();
	
	// parameters are set above but NOT saved, here's how you save parameters
	// notion here is you can set several but save once as opposed to saving on each parameter change
	// you can save permanently (retained at start up, or temp which is ideal for dynamically changing the address or frequency
	void SaveParameters(uint8_t val = PERMANENT);
	
protected:

	// method to let method know of module is busy doing something (timeout provided to avoid lockups)
	void CompleteTask(unsigned long timeout = 0);

	// function to read modules parameters
	bool ReadParameters();

	// utility funciton to build the "speed byte" which is a collection of a few different parameters
	void BuildSpeedByte();

	// utility funciton to build the "options byte" which is a collection of a few different parameters
	void BuildOptionByte();

	// delay that does NOT lockup the MCU
	void SmartDelay(unsigned long val = 20);

	bool ReadModelData();


private:

	// variable for the serial stream
	Stream*  _s;
	Stream*  _TD;

	// pin variables
	uint8_t _M0;
	uint8_t _M1;
	uint8_t _AUX;

	// variable for the 6 bytes that are sent to the module to program it
	// or bytes received to indicate modules programmed settings
	uint8_t _Params[6];

	// indicidual variables for each of the 6 bytes
	// _Params could be used as the main variable storage, but since some bytes
	// are a collection of several options, let's just make storage consistent
	// also Param[1] is different data depending on the _Save variable
	uint8_t _Save;
	uint8_t _AddressHigh;
	uint8_t _AddressLow;
	uint8_t _Speed;
	uint8_t _Channel;
	uint8_t _Options;
	
	// individual variables for all the options
	uint8_t _ParityBit;
	uint8_t _UARTDataRate;
	uint8_t _AirDataRate;
	uint8_t _OptionTrans;
	uint8_t _OptionPullup;
	uint8_t _OptionWakeup;
	uint8_t _OptionFEC;
	uint8_t _OptionPower;
	uint16_t _Address;
	uint8_t _Model;
	uint8_t _Version;
	uint8_t _Features;
	uint8_t _buf;
	unsigned long _rt;

};
