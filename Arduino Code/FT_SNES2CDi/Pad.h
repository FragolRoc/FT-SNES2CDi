#ifndef Pad_h
#define Pad_h

#include <inttypes.h>
#include <SoftwareSerial.h>
#include "Arduino.h"


class Pad {

  public:
    Pad(int rts, int rxd, int led);
    void task();
    void send();
    void snes2cdi(uint32_t btns);
    uint32_t snesButtons;

  private:
    SoftwareSerial vSerial;
    bool assertRTS();
    int adjustSpeed(int val);
    void changeSpeed(byte newspeed);
    byte flipByte(byte c);
    int RTSpin, LEDpin;
    int padbyte0, padbyte1, padbyte2, oldpadbyte0, oldpadbyte1, oldpadbyte2, x, y;
    byte spd;
    bool firstId = true;
    bool btnLpressed = false;
    bool btnRpressed = false;
    bool btnSEpressed = false;
    bool standardMapping = true;
    bool connected = false;
};

#endif
