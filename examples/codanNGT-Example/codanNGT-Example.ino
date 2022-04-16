/*
  CodanNGT.h - Library for controlling a CODAN NGT (2012 RF unit) radio for personal HAM radio use only.
  Created by Fazlay Rabby S21RC, April 16, 2022.
  Released into the public domain.
*/


/* SCL, SDA, GND (and +5v) of Arduino will be connected to 12c bus of codan NGT radio RF Unit (and +5v pad).
 *  SCL, SDA, GND, +5v test pad can be found inside Audio/Interconnect board.
 *  Serial port of Arduino (RX, TX, and GND) of ardiono will be connected to computer using an isolated interface. 
 */



#include "IC746.h" // Simulate an ICOM 746 radio from the CAT point of view.  https://github.com/kk4das/IC746CAT by Dean Souleles, KK4DAS, <kk4das@gmail.com>

#include "codanNGT.h" // Library for the arduino microcontroller to send command to different PIC microcontroller of Codan Radio.  


codanNGT NGT;
#define DDS_I2C 0x3F //0x3F Frequency control IC405 PIC (RF/IF)
#define FILTER_I2C 0x68 //0x68  Filter control IC PA and filter control
#define TONE_I2C 0x24 //0x24  IC9 PIC control of switches and tone (audio and inteconnect board)

// DEBUG flag, uncomment for testing
#define DEBUG false
//#define DEBUG2   // turning off debug of read requests makes it easier to follow



IC746 radio = IC746();

// variables
unsigned long freq = 7074000l;
unsigned long bfreq = 14125000l;

boolean splitActive = false;

#define VFO_A 0
#define VFO_B 1
byte activeVFO = VFO_A;

// radio modes
#define MODE_LSB 00
#define MODE_USB 01
byte mode = MODE_USB;

#define PTT_RX 0
#define PTT_TX 1
byte ptt = PTT_RX;


// function to run when we must put radio on TX/RX
void catSetPtt(boolean catPTT) {
  // the var ptt follows the value passed, but you can do a few more thing here
  if (catPTT) {
    ptt = PTT_TX;
  } else {
    ptt = PTT_RX;
  }

#if defined (DEBUG)
  String msg = "SetPTT: ";
  if (catPTT) {
    msg += "Tx";
  } else {
    msg += "Rx";
  }
  displayPrintln(msg);
  
#endif
}

boolean catGetPtt() {
#if defined (DEBUG2)
  String msg = "GetPTT: ";
  if (ptt == PTT_TX) {
    msg += "Tx";
  } else {
    msg += "Rx";
  }
  displayPrintln(msg);
#endif
  if (ptt == PTT_TX) {
    return true;
  } else {
    return false;
  }
}

// function to run to toggle Split mode on and off
void catSetSplit(boolean catSplit) {
  // the var ptt follows the value passed, but you can do a few more thing here
  if (catSplit) {
    splitActive = true;
  
  } else {
    splitActive = false;
  }
NGT.update(freq, bfreq, mode, splitActive, activeVFO);
#if defined (DEBUG)
  String msg = "SetSplit: ";
  if (catSplit) {
    msg += "On";
  } else {
    msg += "Off";
  }
  displayPrintln(msg);
#endif
}

// function to run when
void catSwapVfo() {
  // Swap the active VFO
  if (activeVFO == VFO_A) {
    activeVFO = VFO_B;
  } else {
    activeVFO = VFO_A;
  }
NGT.update(freq, bfreq, mode, splitActive, activeVFO);

#if defined (DEBUG)
  // debug
  displayPrintln("swapVfo");
#endif
}

// function to set a freq from CAT
void catSetFreq(long f) {
  // the var freq follows the value passed, but you can do a few more thing here

  
  if (activeVFO == VFO_A) {
    freq = f;
    
  } else {
    bfreq = f;
    
  }

NGT.update(freq, bfreq, mode, splitActive, activeVFO);
  }

// function to set the mode from the cat command
void catSetMode(byte m) {
  if (m == CAT_MODE_LSB) {
    mode = MODE_LSB;
   } else {
    mode = MODE_USB;
    }
NGT.update(freq, bfreq, mode, splitActive, activeVFO);
    

#if defined (DEBUG)
  String msg = "SetMode: ";
  if (mode == MODE_LSB) {
    msg += "LSB";
  } else {
    msg += "USB";
  }
  displayPrintln(msg);
#endif
}

// function to set the active VFO from the cat command
void catSetVFO(byte v) {
  if (v == CAT_VFO_A) {
    activeVFO = CAT_VFO_A;
  } else {
    activeVFO = CAT_VFO_B;
  }

NGT.update(freq, bfreq, mode, splitActive, activeVFO);

#if defined (DEBUG)
  String msg = "SetVFO: ";
  if (v == CAT_VFO_A) {
    msg += "VFO-A";
  } else {
    msg += "VFO-B";
  }
  displayPrintln(msg);
#endif
}

// Function to make VFOS the same
void catVfoAtoB() {
  if (activeVFO == VFO_A) {
    bfreq = freq;
  } else {
    freq = bfreq;
  }

NGT.update(freq, bfreq, mode, splitActive, activeVFO);
  
#if defined (DEBUG)
  String msg = "VfoAtoB";
  displayPrintln(msg);
#endif
}

// function to pass the freq to the cat library
long catGetFreq() {
  // this must return the freq as an unsigned long in Hz, you must prepare it before
  long f;

  if (activeVFO == VFO_A) {
    f = freq;
  } else {
    f = bfreq;
  }
#if defined (DEBUG2)
  // debug
  String msg = "GetFreq: ";
  msg += String(f);
  displayPrintln(msg);
#endif

  // pass it away
  return f;
}

// function to pass the mode to the cat library
byte catGetMode() {
  // this must return the mode in the wat the CAT protocol expect it
  byte catMode;

#if defined (DEBUG2)
  // debug
  String msg = String("GetMode: ");
  if (mode == MODE_LSB) {
    msg += "LSB";
  } else {
    msg += "USB";
  }
  displayPrintln(msg);
#endif

  if (mode == MODE_LSB) {
    catMode = CAT_MODE_LSB;
  } else {
    catMode = CAT_MODE_USB;
  }
  return catMode;
}

// function to pass the smeter reading in RX mode
byte catGetSMeter() {
  static int s = 0;
  static byte counter = 0;

  if (counter == 5) {
    counter = 0;
    if (s == 15) {
      s=0;
    } else {
      s++;
    }
  } else {
    counter++;
  }
  //  Return 0-9, 10=S9+10 - 15=S9+60

#if defined (DEBUG2)
  // debug
  String msg = "GetSMeter: ";
  msg += s;
  displayPrintln(msg);
#endif

  // pass it away (fixed here just for testing)
  return byte(s);
}



void displayPrintln(String s ) {
  //Debug display code goes here
}

void setup() {

  // preload the vars in the cat library
  radio.addCATPtt(catSetPtt);
  radio.addCATGetPtt(catGetPtt);
  radio.addCATAtoB(catVfoAtoB);
  radio.addCATSwapVfo(catSwapVfo);
  radio.addCATsplit(catSetSplit);
  radio.addCATFSet(catSetFreq);
  radio.addCATMSet(catSetMode);
  radio.addCATVSet(catSetVFO);
  radio.addCATGetFreq(catGetFreq);
  radio.addCATGetMode(catGetMode);
  radio.addCATSMeter(catGetSMeter);

  // now we activate the library
  radio.begin(19200, SERIAL_8N1);
  
  NGT.uPaddress(DDS_I2C, FILTER_I2C, TONE_I2C); // setting the i2c addresses of different microcontroller inside Codan RF Unit.

#if defined (DEBUG)
  displayPrintln("CAT Serial Test Ready");
#endif

}

void loop() {
  radio.check();
}
