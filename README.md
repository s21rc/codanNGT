# codanNGT
HAM RADIO VFO for CODAN NGT transceiver using Arduino
** ONLY for PERSONAL HAM radio use. Use at your own risk. Author is not responsible for any loss/malfunction of equipment/property or legal issues. **
** This is an add on board interfacing HAM radio CAT protocol and Codan NGT hardware.  **


This library will send necessary command to the Frequency Controller PIC microcontroller and filer board microcontroller
to select frequency and band filter based on the data provided by the CAT software.

The  i2c command codes/protocols are not available and thus recorded using logic analyzer from the main i2c bus.

I was able to figure out most of the data (RX and TX freq, Band filter selection, Mode(LSB/USB/AM), Easytalk/NB on/off, Volume level etc), 
but some of the data I couldn't figure out, and used same from guess work.

Tested with Hamradio Deluxe, WSJTx.

I have used the IC746 CAT emulator by Dean Souleles KK4DAS https://github.com/kk4das/IC746CAT 
But any other CAT emulator should work.

There are two functions:

inside Setup():
 NGT.uPaddress(DDS_I2C, FILTER_I2C, TONE_I2C); // setting the i2c addresses of different microcontroller inside Codan RF Unit.

inside differrent callback functions:
NGT.update(freq_of_first_vfo, freq_of_second_vfo, sideband_mode, split_Active, active_VFO);

DO NOT OPERATE CROSS BAND as I have not tested yet if the LPF relay switches with different band TX or not.

Planned future work:

1. Analyze the Serial data beetween microphone and RF unit to control other functions.
2. A dedicated control head with Color TFT touch display and Encoder for frequency change.


** SETUP **
After the arduino board is uploaded with the code, connect the +5V, GND, SCL and SDA of the arduino to same labeled test pad inside the RF Unit of codan NGT. Most of the board has all these test pad
I found it is suitably located inside the audio/interconnect board (TOP front side) where there is also plenty of space to fix a Arduino pro Mini using both side tape/sticker

Programm the radio using NSP in a single channel 7000.000 MHz (Not required but the radio will come up to this frequency on boot)

connect the RX and TX and GND of the arduino to TX and RX and GND of a remote/digital interface (Isolated).
(You can cut two unused wire of the GPIO and route the RX/TX through the GPIO connector) 

Use the microphone to control volume, mute and PTT. DO NOT press channel UP/DOWN as that will change the VFO freq and radio set to that channel.
