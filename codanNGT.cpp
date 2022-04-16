/*
  CodanNGT.h - Library for controlling a CODAN NGT (2012 RF unit) radio for personal HAM radio use.
  Created by Fazlay Rabby S21RC, April 16, 2022.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Wire.h"
#include "codanNGT.h"

/* setting i2c address of different PIC microcontroller of the codan RF unit
// For a NGT SRX 2012 RF UNIT:
// 0x2C volume control IC4 AD5242 (audio and inteconnect board)
// 0x3F	Frequency control IC405 PIC (RF/IF)
// 0x68	Filter control IC PA and filter control
// 0x24	IC9 PIC control of switches and tone (audio and inteconnect board)

// 0x48 	Tone generator IC3 PCD3312C (audio and inteconnect board)
// 0xD0 	RTC Clock IC10 MK41t56 (audio and inteconnect board)
//
// From NGT SRX 2012 RF unit using a i2c analyzer
// Sample data for setting TX: 21234.768  and RX: 21448.526 with mode:USB
// 
// write to 0x3F ack data: 0x45 0x0D 0x50 0x04 0x44 0x01 0x4E 0x47 0x47 0x01 0x02 0xDC 0x05 0xC4 0x09
// write to 0x68 ack data: 0x16 0x40 0x00 0x03 0x01 0x47 0x47 0x4E 0x01 0x44 0x04 0x50 0x00 0xCE 0xEC 
//
*/

 void codanNGT::uPaddress(byte _dds, byte _filter, byte _tone){
    i2c_dds = _dds;
    i2c_filter = _filter;
    i2c_tone = _tone;
 }

void codanNGT::update(unsigned long _freq, unsigned long _bfreq, byte _sideband, bool _ENsplit, byte _VFO){

  
if (_sideband == 01) _codanMode = 0x02;
  else if (_sideband == 00) _codanMode = 0x01;

if (_ENsplit){
  _codanRX = 4;
  _codanTX = 0;
  _filterfreq = _bfreq;
}
else if (_VFO == 0 && !_ENsplit){
  _codanRX = 0;
  _codanTX = 0;
  _filterfreq = _freq;
  }
  else if (_VFO == 1 && !_ENsplit){
    _codanRX = 4;
    _codanTX = 4;
    _filterfreq = _bfreq;
    }
    

if (_filterfreq >= 1600000 && _filterfreq < 2400000){
    _filterband[0] = 0x04 ;
    _filterband[1] = 0x9C ;
    _filterband[2] = 0x4C ;
}
else if (_filterfreq >= 2400000 && _filterfreq < 3700000){
    _filterband[0] = 0x05 ;
    _filterband[1] = 0x18 ;
    _filterband[2] = 0x69 ;
}
else if (_filterfreq >= 3700000 && _filterfreq < 5600000){
    _filterband[0] = 0x06 ;
    _filterband[1] = 0xC3 ;
    _filterband[2] = 0x50 ;
}
else if (_filterfreq >= 5600000 && _filterfreq < 8500000){
    _filterband[0] = 0x07 ;
    _filterband[1] = 0xA2 ;
    _filterband[2] = 0x6F ;
}
else if (_filterfreq >= 8500000 && _filterfreq < 13000000){
    _filterband[0] = 0x08 ;
    _filterband[1] = 0x57 ;
    _filterband[2] = 0x96 ;
}
else if (_filterfreq >= 13000000 && _filterfreq < 19700000){
    _filterband[0] = 0x09 ;
    _filterband[1] = 0xC5 ;
    _filterband[2] = 0x24 ;
}
else if (_filterfreq >= 19700000 && _filterfreq < 30000000){
    _filterband[0] = 0x0A ;
    _filterband[1] = 0x41 ;
    _filterband[2] = 0x39 ;
}



_frequency[0] = _freq & 0xFF; // 0x78
_frequency[1] = (_freq >> 8) & 0xFF; // 0x56
_frequency[2] = (_freq >> 16) & 0xFF; // 0x34
_frequency[3] = (_freq >> 24) & 0xFF; // 0x12
_frequency[4] = _bfreq & 0xFF; // 0x78
_frequency[5] = (_bfreq >> 8) & 0xFF; // 0x56
_frequency[6] = (_bfreq >> 16) & 0xFF; // 0x34
_frequency[7] = (_bfreq >> 24) & 0xFF; // 0x12


  Wire.begin(); 
  Wire.beginTransmission(i2c_dds); 
  Wire.write(0x45);
  Wire.write(0x0D);
  Wire.write(_frequency[0+_codanTX]);
  Wire.write(_frequency[1+_codanTX]);
  Wire.write(_frequency[2+_codanTX]);
  Wire.write(_frequency[3+_codanTX]);
  Wire.write(_frequency[0+_codanRX]);
  Wire.write(_frequency[1+_codanRX]);
  Wire.write(_frequency[2+_codanRX]);
  Wire.write(_frequency[3+_codanRX]);
  Wire.write(_codanMode);
  Wire.write(0xDC);
  Wire.write(0x05);
  Wire.write(0xc4);
  Wire.write(0x09);
  Wire.endTransmission();

  Wire.beginTransmission(i2c_filter); 
  Wire.write(0x08);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.write(_filterband[0]);
  Wire.write(_frequency[3+_codanRX]);
  Wire.write(_frequency[2+_codanRX]);
  Wire.write(_frequency[1+_codanRX]);
  Wire.write(_frequency[0+_codanRX]);
  Wire.write(_frequency[3+_codanTX]);
  Wire.write(_frequency[2+_codanTX]);
  Wire.write(_frequency[1+_codanTX]);
  Wire.write(_frequency[0+_codanTX]);
  Wire.write(0x00);
  Wire.write(_filterband[1]);
  Wire.write(_filterband[2]);
  Wire.endTransmission();

}