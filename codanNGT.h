/*
  CodanNGT.h - Library for controlling a CODAN NGT (2012 RF unit) radio for personal HAM radio use.
  Created by Fazlay Rabby S21RC, April 16, 2022.
  Released into the public domain.
*/
#ifndef codanNGT_h
#define codanNGT_h

#include "Arduino.h"
#include "Wire.h"





class codanNGT
{
  public:
    void uPaddress(byte _dds, byte _filter, byte _tone);
    byte i2c_dds;
    byte i2c_filter;
    byte i2c_tone;
    void update(unsigned long _freq, unsigned long _bfreq, byte _sideband, bool _ENsplit, byte _VFO);
    byte _frequency[8];
    byte _filterband[4];
    unsigned long _filterfreq;
    byte _codanMode;
    int _codanRX;
    int _codanTX;
    
    
  private:
};

      

#endif