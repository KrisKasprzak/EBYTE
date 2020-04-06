# EBYTE
 
<b><h2><center>EBYTE Transceivers</center></h1></b>

This library is intended to be used with UART type EBYTE transceivers (E44-TTL-100 for example), small wireless units for MCU's such as
Teensy and Arduino. This library lets users program the operating parameters and both send and receive data.
This company makes several modules with different capabilities, but most #defines here should be compatible with them
All constants were extracted from several data sheets and listed in binary as that's how the data sheet represented each setting.
Hopefully, any changes or additions to constants can be a matter of copying the data sheet constants directly into the #defines

Usage of this library consumes around 970 bytes

<b> EBYTE Model numbers (only a partial list shown)</b>
Old model number scheme 
E30-TTL-100,E30-490T20D, E31-TTL-100, E32-TTL-100, E32-TTL-500, E32-TTL-1W, E41-TTL-100, E41-TTL-100, E44-TTL-100, E44-TTL-500, E44-TTL-1W, E50-TTL-100, E50-TTL-500, E51-TTL-100, E51-TTL-500, E51-TTL-1W

New model number scheme 
E22-900T22S, E22-230T22S, E22-400T22S, E22-230T30S, E22-400T30S, E22-900T30S, E30-433T20S3, E30-170T20D, E30-170T27D, E30-780T20S, E30-868T20S, E30-868T20D, E30-915T20D, E30-490T20D, E30-433T20S, E30-433T20D, E30-915T20S, E30-490T20S, E31-433T30S, E31-433T17S3, E31-230T33D, E31-230T17D, E31-230T27D, E31-433T17S, E31-433T17D, E31-433T27D, E31-433T30D, E31-433T33D, E32-433T20DC, E32-433T20S, E32-433T20S2T, E32-433T27D, E32-433T30D, E32-433T30S, E32-868T20D, E32-868T20S, E32-868T30D, E32-868T30S,
E32-915T20D, E32-915T20S, E32-915T30D, E32-915T30S, E32-170T30D, E32-400T20S, 

Here is a YouTube video on library usage: https://youtu.be/hMjArKGucFA
  
<b><h3> Module connection </b></h3>
Module	MCU						Description
1. MO		Any digital pin*		pin to control working/program modes 
2. M1		Any digital pin*		pin to control working/program modes
3. Rx		Any digital pin			pin to MCU TX pin (module transmits to MCU, hence MCU must receive data from module
4. Tx		Any digital pin			pin to MCU RX pin (module transmits to MCU, hence MCU must receive data from module
5. AUX		Any digital pin			pin to indicate when an operation is complete (low is busy, high is done) (you can omit with -1, but fixed recovery time used and may not be long enough to complete the operation)
6. Vcc		+3v3 or 5V0, note the units may run warmer with 5V0 and consume more power				
7. Vcc		Ground					Ground must be common to module and MCU		

notes

1. caution in connecting to Arduino pin 0 and 1 as those pins are for USB connection to PC so you can't have the EBYTE connected during programming. I recommend NOT using Arduino pins 0 and 1
2. The signal lines for these units are 3V3 but are 5 volt tolerant, however 5 volts may result in communication failures. If using a 5 volt MCU such as arduino, you may need to do the following. 
  a) You may need a 4K7-10K pullup to Rx and AUX pins (possibly Tx) if using and Arduino
  b) If using an Arduino you may need a series 4K7 resistor between MCU Tx and the transceiver Rx.
4. In some of my applications, I did not have enough digital pins to connect the Aux pin. No worries (just pass -1 in the argument list in the object create code). Then you will need to provide an appropriate delay() to let the transmission complete--experiment with the amount.

<b><h3>Manufacturers website</b></h3> 
http://www.ebyte.com/en/
example module this library is intended to be used with
http://www.ebyte.com/en/product-view-news.aspx?id=174

<b><h3>General code usage</b></h3> 
1. Create a serial object
2. Create EBYTE object that uses the serial object
3. begin the serial object
4. init() the EBYTE object
5. set parameters (optional but required if sender and receiver are different)
6. send or listen to sent data (single byte) OR create and send a data structure

<b><h3>Tips on usage</b></h3> 

For best range:
<ul>
<li> Data sheet indicates best results are with antennas 2meters off of ground</li>
<li> Line of sight ideal, but my personal testing, transmission still successful with some obstructions</li>
<li> Slow air data rates can improve range, but due to longer transmission time, how often data can be sent will be sacrificed</li>
<li> Consider high gain antennas (can be purchased from the manufacturer) see their web site for details</li>
<li> The data sheet says for max range, power the units with 5.0 volts (keep 3V3 on the signal lines). I personaly found little range differene with higher supply voltage</li>
 <li> The data sheet says for max range, set the air data rate to 2.4 bps. I personaly found little range differene with low data rates, and low data rates may limit how often you can send data. </li>
</ul>

<b><h3>Data transmission packets</b></h3>
<ul>
<li> this library has a method for sending single bytes but if more data is to be sent, create a data structure and send the data structure using the librarys SendStruct(&struct, sizeof(struct)) method. Note pass by ref so include the & before structure name</li>
<li> again slow data rates take longer, you will need to experiment with ideal air data rate range based on data size</li>
</ul>
<b><h3>Debugging</b></h3>
<ul>
<li> If you are using their 1W units, power the unit separately from the MCU's onboard power supply. The current draw may exceed the onboard rating resulting in destroying the MCU.</li>
<li> If transmitter and receiver are different MCU (Arduino <-> Teensy), data structures cannot have a mix of data types, due to how an 8-bit processor and 32-bit processor handle ints, floats, etc. If floats and ints are needed to be sent considering multiplying a float to 100 (and recasting to an int), then divide that value by 100 on the receiving end (recasting to a float)</li>
</ul>
