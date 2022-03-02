#ifndef Pad_h
#define Pad_h

#include <inttypes.h>
#include <SoftwareSerial.h>
#include "Arduino.h"

#define RELATIVE   0b11001101 // Relative Device (Mouse)
#define MANEUVER   0b11001010 // Maneuvering Device (Controller)

class Pad {

  public:
    Pad(int rts, int rxd);
    void task();
    void send();
    void snesToCDi(uint32_t btns);

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
    byte mode = MANEUVER;
};

#endif
