# EBYTE
 

This library is intended to be used with UART type EBYTE transceivers (E44-TTL-100 for example), small wireless units for MCU's such as
Teensy and Arduino. This library lets users program the operating parameters and both send and receive data.
This company makes several modules with different capabilities, but most #defines here should be compatible with them
All constants were extracted from several data sheets and listed in binary as that's how the data sheet represented each setting.
Hopefully, any changes or additions to constants can be a matter of copying the data sheet constants directly into the #defines

Old style model numbers
E30-TTL-100
E30-490T20D
E31-TTL-100
E32-TTL-100
E32-TTL-500
E32-TTL-1W
E41-TTL-100
E41-TTL-100
E44-TTL-100
E44-TTL-500
E44-TTL-1W
E50-TTL-100
E50-TTL-500
E51-TTL-100
E51-TTL-500
E51-TTL-1W

new model number scheme

Model No. Core IC Frequency power dBm Distance (km) Antenna Size (mm) Interface
E22-900T22S SX1262 868M 915M 22 7 SMD 16*26 UART
E22-230T22S SX1262 230M 22 7 SMD 16*26 UART
E22-400T22S SX1268 430M 470M 22 7 SMD 16*26 UART
E22-230T30S SX1262 230M 30 10 SMD 40.5*25 UART
E22-400T30S SX1268 430M 470M 30 10 SMD 40.5*25 UART
E22-900T30S SX1262 868M 915M 30 10 SMD 40.5*25 UART

Model	Interface	IC	Frequency Hz	Power dBm	Distance km	Air data rate	Package	Size (mm)	Feature
E30-433T20S3	UART	SI4438	433M	20	2.5	1k~25k	SMD	16*26	FEC , WOR	
E30-DTU-433M37	232/485	SI4438	433M	37	15	1k~25k	Screwing/DB9	124*105*25	FEC , WOR	
E30-DTU-490M20	232/485	SI4438	490M	20	2.5	1k~25k	Screwing/DB9	82*62*25	FEC , WOR	
E30-170T20D	UART	SI4463	170M	20	2.0	1k~25k	DIP	21*36	Diffraction , FEC	
E30-170T27D	UART	SI4463	170M	27	5.0	1k~25k	DIP	24*43	Diffraction , FEC	
E30-780T20S	UART	SI4463	780M	20	2.5	1k~25k	SMD	17*30	FEC , WOR	
E30-868T20S	UART	SI4463	868M	20	2.5	1k~25k	SMD	17*30	FEC , WOR	
E30-868T20D	UART	SI4463	868M	20	2.5	1k~25k	DIP	21*36	FEC , WOR	
E30-915T20D	UART	SI4463	915M	20	2.5	1k~25k	DIP	21*36	FEC , WOR	
E30-490T20D	UART	SI4438	490M	20	2.5	1k~25k	DIP	21*36	FEC , WOR	
E30-433T20S	UART	SI4438	433M	20	2.0	1k~25k	SMD	17*30	FEC , WOR	
E30-433T20D	UART	SI4438	433M	20	2.0	1k~25k	DIP	21*36	FEC , WOR	
E30-915T20S	UART	SI4463	915M	20	2.5	1k~25k	SMD	17*30	FEC , WOR	
E30-490T20S	UART	SI4438	490M	20	2.5	1k~25k	SMD	17*30	FEC , WOR	

Model Interface IC Frequency Power dBm	Distance km	Air data rate	Antenna	Size(mm)	Feature
E31-433T30S	UART	AX5243	433M	30	8.6	1.2k~70k	SMD	25*40.5	Narrow-band transmission,WOR	
E31-433T17S3	UART	AX5243	433M	17	2.0	1.2k~70k	SMD	16*26	Narrow-band transmission,WOR	
E31-230T33D	UART	AX5243	230M	33	8.0	1.2k~70k	DIP	37*60	Diffraction , WOR	
E31-DTU-230N17	232/485	AX5243	230M	17	2.0	1.2k~70k	Screwing/DB9	82*62*25	Diffraction , WOR	
E31-DTU-230N27	232/485	AX5243	230M	27	5.0	1.2k~70k	Screwing/DB9	82*62*25	Diffraction , WOR	
E31-230T17D	UART	AX5243	230M	17	2.0	1.2k~70k	DIP	21*36	Diffraction , WOR	
E31-230T27D	UART	AX5243	230M	27	5.0	1.2k~70k	DIP	24*43	Diffraction , WOR	
E31-433T17S	UART	AX5243	433M	17	2.0	1.2k~70k	SMD	17*30	Narrow-band transmission	
E31-433T17D	UART	AX5243	433M	17	2.1	1.2k~70k	DIP	21*36	Narrow-band transmission	
E31-433T27D	UART	AX5243	433M	27	4.0	1.2k~70k	DIP	24*43	Narrow-band transmission	
E31-433T30D	UART	AX5243	433M	30	6.0	1.2k~70k	DIP	24*43	Narrow-band transmission	
E31-433T33D	UART	AX5243	433M	33	8.0	1.2k~70k	DIP	37*60	Narrow-band transmission	

Model Frequency TX Power Range  Antenna
E32 (433T20DC) 433M 20dBm 3000m DIP SMA-K
E32 (433T20S) 433M 20dBm 3000m SMD Spring
E32 (433T20S2T) 433M 20dBm 3000m SMD IPEX/Spring
E32 (433T27D) 433M 27dBm 5000m DIP SMA-K
E32 (433T30D) 433M 30dBm 8000m DIP SMA-K
E32 (433T30S) 433M 30dBm 8000m SMD IPEX/Spring
E32 (868T20D) 868M 20dBm 3000m DIP SMA-K
E32 (868T20S) 868M 20dBm 3000m SMD IPEX/Spring
E32 (868T30D) 868M 30dBm 8000m DIP SMA-K
E32 (868T30S) 868M 30dBm 8000m SMD IPEX/Spring
E32 (915T20D) 915M 20dBm 3000m DIP SMA-K
E32 (915T20S) 915M 20dBm 3000m SMD IPEX/Spring
E32 (915T30D) 915M 30dBm 8000m DIP SMA-K
E32 (915T30S) 915M 30dBm 8000m SMD IPEX/Spring
E32 (170T30D) 170M 30dBm 8000m DIP SMA-K
E32 (400T20S) 470M 20dBm 3000m SMD IPEX/Spring

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
