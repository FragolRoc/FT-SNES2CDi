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

#include "SNESpad.h"
#include "Arduino.h"

// change the next three lines if necessary
// arduino pins and their connection to the nes joystick
// default: digital pin 2=strobe, 3=clock, 4=data
// see readme.txt for game pad pinout

// -- actually, let's just eliminate this since people seem to be having
// -- trouble with it.. stick with specifying the pin numbers as 
// -- arguments to the constructor

// SNESpad::SNESpad() {
//     SNESpad::SNESpad(2,3,4);
// }

// constructor
SNESpad::SNESpad(int strobe, int clock, int data)
  : m_strobe (strobe),
  m_clock (clock),
  m_data (data)
  {
    pinMode(strobe, OUTPUT);
    pinMode(clock,  OUTPUT);
    pinMode(data, INPUT);
  }

uint32_t SNESpad::buttons(int m_speed)
{
  uint32_t ret = 0;
  byte i;

  strobe(m_speed);
  for (i = 0; i < 32; i++) {
    uint32_t shiftBit = shiftin();
    ret |= shiftBit << i;

    // verify if device is a mouse or not
    if (i == 15) {
      mouse = !shiftBit;
      if (!mouse) break;
    }
  }

  // verify controller or mouse is connected
  if (!ret) {
    mouse_speed_fail = 0;
    mouse_speed = 0;
    return 0;
  }

  // verify mouse speed
  if (mouse) {
    int last_mouse_speed = mouse_speed;

    // parse mouse speed bits
    mouse_speed = ((~ret & SNES_MOUSE_SPEED) >> 10);
    if (mouse_speed > 2) mouse_speed = 0;

    // detect hyperkin mouse failure to change speed to halt further attempts
    if (
      mouse_speed != m_speed
      && last_mouse_speed == mouse_speed
      && mouse_speed_fail < SNES_MOUSE_FAIL_THRESHOLD
    ) {
        mouse_speed_fail++;
    }
  }

  return ~ret;
}

void SNESpad::strobe(int m_speed)
{
  digitalWrite(m_strobe,HIGH);
  delayMicroseconds(12);

  // signal mouse to go to next speed if not desired speed
  if (
    mouse == true
    && mouse_speed != m_speed
    && mouse_speed_fail < SNES_MOUSE_FAIL_THRESHOLD
  ) {
    digitalWrite(m_clock,LOW);
    delayMicroseconds(6);
    digitalWrite(m_clock,HIGH);
    delayMicroseconds(12);
  }

  digitalWrite(m_strobe,LOW);
  delayMicroseconds(6);
}

uint32_t SNESpad::shiftin(void)
{
  digitalWrite(m_clock,LOW);
  delayMicroseconds(6);
  uint32_t ret = digitalRead(m_data);
  digitalWrite(m_clock,HIGH);
  delayMicroseconds(6);
  return ret;
}
