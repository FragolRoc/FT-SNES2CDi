#include "Pad.h"
#include "SNESpad.h"
#include "Arduino.h"
#include <EEPROM.h>
#include <SoftwareSerial.h>

const int RTSthreshold = 328; // threshold for the CDi RTS analog detection

// constructor
Pad::Pad(int rts, int rxd)
  : RTSpin (rts), // the number of the analog pin used to receive the RTS signal from the CDi
  vSerial(11, rxd, true) // RX, TX, inverse_logic. RX is not used here, as the CDi only communicates on the RTS line
  {
    vSerial.begin(1200); // open serial interface to send data to the CDi

    oldpadbyte0 = 0;
    oldpadbyte1 = 0;
    oldpadbyte2 = 0;
  
    byte eepromData = EEPROM.read(0); // retrieve speed setting from the Arduino's EEPROM
    if(eepromData >= 1 && eepromData <= 5) spd = eepromData;
    else spd = 3;
  }

void Pad::task()
{    
  if (assertRTS()) { // wait for CDi to assert the RTS line
    if (!connected) {
      if(firstId) delay(100);
      else delay(1);
      firstId = false;

      vSerial.write(0b11001101); // send device id ("relative device")
      connected = true;
    }
  } else {
    connected = false;
  }
}

void Pad::send()
{
  if(!connected) return;

  if((padbyte0 != oldpadbyte0) || (padbyte1 != 0b10000000) || (padbyte2 != 0b10000000) || ((padbyte0 & 0b00001111) != 0))  // see if state has changed
  {
    vSerial.write(padbyte0);
    vSerial.write(padbyte1);
    vSerial.write(padbyte2);
  }

  // save state
  oldpadbyte0 = padbyte0; 
  oldpadbyte1 = padbyte1;
  oldpadbyte2 = padbyte2;
}

void Pad::snes2cdi(uint32_t btns)
{
  snesButtons = btns;
  bool isController = ((btns & SNES_DEVICE_ID) >> 12) == SNES_PAD_ID;
  bool isMouse = ((btns & SNES_DEVICE_ID) >> 12) == SNES_MOUSE_ID;

  // manage speed control
  if(btns & SNES_R) {
    if(!btnRpressed) changeSpeed(spd+1); // speed : up
    btnRpressed = true;
  }
  else btnRpressed = false;
  if(btns & SNES_L) {
    if(!btnLpressed) changeSpeed(spd-1); // speed : down
    btnLpressed = true;
  }
  else btnLpressed = false;
  if(btns & SNES_START) changeSpeed(3); // speed : default (3)

  padbyte0 = 0b11000000;  //initialize data bytes
  padbyte1 = 0b10000000;
  padbyte2 = 0b10000000;

  // Dpad or Mouse X axis
  x = 127;
  if (isController) {
    if(btns & SNES_LEFT) x = 254;
    if(btns & SNES_RIGHT) x = 1;
    x = adjustSpeed(x);
  }
  if (isMouse) {
    x = 0;
    if (btns & SNES_MOUSE_X) {
      x = (btns & SNES_MOUSE_X) >> 25;
      x = flipByte(x);
    }
    if (btns & SNES_MOUSE_X_SIGN) x = 127 + x;
    else x = 127 - x;
  }
  
  if(x!=127) {
    x = x ^ 0b01111111;
    padbyte1 = padbyte1 | x;
    padbyte1 = padbyte1 & 0b10111111;
    if((x & 0b01000000) != 0)
      padbyte0 = padbyte0 | 0b00000001;
    if((x & 0b10000000) != 0)
      padbyte0 = padbyte0 | 0b00000010;
  }

  // Dpad or Mouse Y axis
  y = 127;
  if (isController) {
    if(btns & SNES_UP) y = 254;
    if(btns & SNES_DOWN) y = 1;
    y = adjustSpeed(y);
  }
  if (isMouse) {
    y = 0;
    if (btns & SNES_MOUSE_Y) {
      y = (btns & SNES_MOUSE_Y) >> 17;
      y = flipByte(y);
    }
    if (btns & SNES_MOUSE_Y_SIGN) y = 127 + y;
    else y = 127 - y;
  }

  if(y!=127) {
    y = y ^ 0b01111111;
    padbyte2 = padbyte2 | y;
    padbyte2 = padbyte2 & 0b10111111;
    if((y & 0b01000000) != 0)
      padbyte0 = padbyte0 | 0b00000100;
    if((y & 0b10000000) != 0)
      padbyte0 = padbyte0 | 0b00001000;
  }

  // buttons
  if(btns & SNES_SELECT) {
    if(!btnSEpressed) standardMapping = !standardMapping; // mapping change : invert buttons 1 & 2 (Y & B)
    btnSEpressed = true;
  }
  else btnSEpressed = false;

  if (isController) {
    if(standardMapping) {
      if(btns & SNES_Y) padbyte0 = padbyte0 | 0b00100000;  //button 1 (Y)
      if(btns & SNES_B) padbyte0 = padbyte0 | 0b00010000;  //button 2 (B)
    }
    else {
      if(btns & SNES_B) padbyte0 = padbyte0 | 0b00100000;  //button 1 (B)
      if(btns & SNES_Y) padbyte0 = padbyte0 | 0b00010000;  //button 2 (Y)
    }
    if((btns & SNES_X) || (btns & SNES_A)) padbyte0 = padbyte0 | 0b00110000; // button 3 (A or X)
  }

  if (isMouse) {
    if(btns & SNES_X) padbyte0 = padbyte0 | 0b00100000; // button 1 (X)
    if(btns & SNES_A) padbyte0 = padbyte0 | 0b00010000; // button 2 (A)
  }
}

// send back the correct Dpad value depending on the speed setting
int Pad::adjustSpeed(int val)
{
  if(val==127 || spd==5) return val;

  if(val==254) {
    if(spd==4) return 202;
    if(spd==3) return 170;
    if(spd==2) return 149;
    if(spd==1) return 130;
  }
  else {
    if(spd==4) return 53;
    if(spd==3) return 85;
    if(spd==2) return 106;
    if(spd==1) return 125;
  }
}

// change speed setting and save it to the EEPROM
void Pad::changeSpeed(byte newspeed)
{
  if(newspeed<1) newspeed=1;
  else if(newspeed>5) newspeed=5;
  
  spd=newspeed;
  EEPROM.write(0, spd);
}

byte Pad::flipByte(byte c) {
  char r=0;
  for(byte i = 0; i < 8; i++) {
    r <<= 1;
    r |= c & 1;
    c >>= 1;
  }
  return r;
}

// true if RTS asserted
bool Pad::assertRTS() {
  if(analogRead(RTSpin) < RTSthreshold) return false;
  else return true;
}
