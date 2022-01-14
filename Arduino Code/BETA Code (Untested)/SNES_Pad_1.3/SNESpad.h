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

#define SNES_B       0x01
#define SNES_Y       0x02
#define SNES_SELECT  0x04
#define SNES_START   0x08
#define SNES_UP      0x10
#define SNES_DOWN    0x20
#define SNES_LEFT    0x40
#define SNES_RIGHT   0x80
#define SNES_A       0x100
#define SNES_X       0x200
#define SNES_L       0x400
#define SNES_R       0x800

class SNESpad {

  public:
    SNESpad();
    SNESpad(int strobe, int clock, int data);
    int buttons();

  private:
    void strobe();
    int shiftin();
    int m_strobe, m_clock, m_data;

};

#endif

