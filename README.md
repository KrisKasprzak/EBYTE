# EBYTE
 

This library is intended to be used with UART type EBYTE transceivers (E44-TTL-100 for example), small wireless units for MCU's such as
Teensy and Arduino. This library lets users program the operating parameters and both send and receive data.
This company makes several modules with different capabilities, but most #defines here should be compatible with them
All constants were extracted from several data sheets and listed in binary as that's how the data sheet represented each setting.
Hopefully, any changes or additions to constants can be a matter of copying the data sheet constants directly into the #defines

Supported module types (not the complete list)
E30
E31
E32
E41
E44
E50
E51

Usage of this library consumes around 970 bytes

  

Module connection
Module	MCU						Description
MO		Any digital pin*		pin to control working/program modes
M1		Any digital pin*		pin to control working/program modes
Rx		Any digital pin			pin to MCU TX pin (module transmits to MCU, hence MCU must receive data from module
Tx		Any digital pin			pin to MCU RX pin (module transmits to MCU, hence MCU must receive data from module
AUX		Any digital pin			pin to indicate when an operation is complete (low is busy, high is done)
Vcc		+3v3 or 5V0				
Vcc		Ground					Ground must be common to module and MCU		

notes
1. caution in connecting to Arduino pin 0 and 1 as those pins are for USB connection to PC, You may need a 4K7 pullup to Rx and AUX pins (possibly Tx) if using and Arduino, OR you may need a series 4K7 resistor between MCU Tx and the transceiver Rx.
2. In some of my applications, I did not have enough digital pins to connect the Aux pin. No worries (just pass -1 in the argument list in the object create code). Then you will need to provide an appropriate delay() to let the transmission complete--experiment with the amount.

Module source
http://www.ebyte.com/en/
example module this library is intended to be used with
http://www.ebyte.com/en/product-view-news.aspx?id=174

General code usage
1. Create a serial object
2. Create EBYTE object that uses the serial object
3. begin the serial object
4. init() the EBYTE object
5. set parameters (optional but required if sender and receiver are different)
6. send or listen to sent data (single byte)
7. Create and send a data structure

Tips on usage

For best range:
1. data sheet indicates best results are with antennas 2meters off of ground
2. line of sight ideal, but my personal testing, transmission still successful with some obstructions
3. Slow air data rates can improve range, but due to longer transmission time, amount of data will be sacrificed
4. consider high gain antennas (can be purchased from the manufacturer) see their web site

Data transmission packets
1. this library has a method for sending single bytes but if more data is to be sent, create a data structure and send using the SendStruct(&struct, sizeof(struct)) method. Note pass by ref so include the & before structure name
2. again slow data rates take longer, you will need to experiment with ideal air data rate range based on data size

Debugging
1. If you are using their 1W units, power the unit separately from the MCU's onboard power supply. The current draw may exceed the onboard rating resulting in destroying the MCU.
2. If transmitter and receiver are different MCU (Arduino <-> Teensy), data structures cannot have a mix of data types, due to how an 8-bit processor and 32-bit processor handle ints, floats, etc. If floats and ints are needed to be sent considering multiplying a float to 100 (and recasting to an int), then divide that value by 100 on the receiving end (recasting to a float)
