/*
  SNESpad - Arduino library for interfacing with an SNES joystick

  Version: 1.3 (11/12/2010) - get rid of shortcut constructor - seems to be broken
  Version: 1.2 (05/25/2009) - put pin numbers in constructor (Pascal Hahn)
  Version: 1.1 (09/22/2008) - fixed compilation errors in arduino 0012 (Rob Duarte)
  Version: 1.0 (09/20/2007) - Created (Rob Duarte)

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITSNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SNESpad_h
#define SNESpad_h

#include <inttypes.h>

#define SNES_B              0x01       // 0b0000000000000000 0000000000000001
#define SNES_Y              0x02       // 0b0000000000000000 0000000000000010
#define SNES_SELECT         0x04       // 0b0000000000000000 0000000000000100
#define SNES_START          0x08       // 0b0000000000000000 0000000000001000
#define SNES_UP             0x10       // 0b0000000000000000 0000000000010000
#define SNES_DOWN           0x20       // 0b0000000000000000 0000000000100000
#define SNES_LEFT           0x40       // 0b0000000000000000 0000000001000000
#define SNES_RIGHT          0x80       // 0b0000000000000000 0000000010000000
#define SNES_A              0x100      // 0b0000000000000000 0000000100000000
#define SNES_X              0x200      // 0b0000000000000000 0000001000000000
#define SNES_L              0x400      // 0b0000000000000000 0000010000000000
#define SNES_R              0x800      // 0b0000000000000000 0000100000000000
#define SNES_DEVICE_ID      0xF000     // 0b0000000000000000 1111000000000000
#define SNES_MOUSE_SPEED    0xC00      // 0b0000000000000000 0000110000000000
#define SNES_MOUSE_Y_SIGN   0x10000    // 0b0000000000000001 0000000000000000
#define SNES_MOUSE_Y        0xFE0000   // 0b0000000011111110 0000000000000000
#define SNES_MOUSE_X_SIGN   0x1000000  // 0b0000000100000000 0000000000000000
#define SNES_MOUSE_X        0xFE000000 // 0b1111111000000000 0000000000000000

#define SNES_PAD_ID         0b0000
#define SNES_MOUSE_ID       0b1000

#define SNES_MOUSE_SLOW     0
#define SNES_MOUSE_MEDIUM   1
#define SNES_MOUSE_FAST     2

#define SNES_MOUSE_FAIL_THRESHOLD 3

class SNESpad {

  public:
    SNESpad();
    SNESpad(int strobe, int clock, int data);
    uint32_t buttons(int speed);

  private:
    void strobe(int speed);
    uint32_t shiftin();
    int m_strobe, m_clock, m_data;
    int mouse_speed = 0, mouse_speed_fail = 0;
    bool mouse = false;

};

#endif
