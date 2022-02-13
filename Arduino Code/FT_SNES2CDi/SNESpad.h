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
#define SNES_PAD_ID_3       0x1000     // 0b0000000000000000 0001000000000000
#define SNES_PAD_ID_2       0x2000     // 0b0000000000000000 0010000000000000
#define SNES_PAD_ID_1       0x4000     // 0b0000000000000000 0100000000000000
#define SNES_PAD_ID_0       0x8000     // 0b0000000000000000 1000000000000000
#define SNES_MOUSE_Y_SIGN   0x10000    // 0b0000000000000001 0000000000000000
#define SNES_MOUSE_Y_6      0x20000    // 0b0000000000000010 0000000000000000
#define SNES_MOUSE_Y_5      0x40000    // 0b0000000000000100 0000000000000000
#define SNES_MOUSE_Y_4      0x80000    // 0b0000000000001000 0000000000000000
#define SNES_MOUSE_Y_3      0x100000   // 0b0000000000010000 0000000000000000
#define SNES_MOUSE_Y_2      0x200000   // 0b0000000000100000 0000000000000000
#define SNES_MOUSE_Y_1      0x400000   // 0b0000000001000000 0000000000000000
#define SNES_MOUSE_Y_0      0x800000   // 0b0000000010000000 0000000000000000
#define SNES_MOUSE_X_SIGN   0x1000000  // 0b0000000100000000 0000000000000000
#define SNES_MOUSE_X_6      0x2000000  // 0b0000001000000000 0000000000000000
#define SNES_MOUSE_X_5      0x4000000  // 0b0000010000000000 0000000000000000
#define SNES_MOUSE_X_4      0x8000000  // 0b0000100000000000 0000000000000000
#define SNES_MOUSE_X_3      0x10000000 // 0b0001000000000000 0000000000000000
#define SNES_MOUSE_X_2      0x20000000 // 0b0010000000000000 0000000000000000
#define SNES_MOUSE_X_1      0x40000000 // 0b0100000000000000 0000000000000000
#define SNES_MOUSE_X_0      0x80000000 // 0b1000000000000000 0000000000000000

class SNESpad {

  public:
    SNESpad();
    SNESpad(int strobe, int clock, int data);
    uint32_t buttons();

  private:
    void strobe();
    uint32_t shiftin();
    int m_strobe, m_clock, m_data;

};

#endif
