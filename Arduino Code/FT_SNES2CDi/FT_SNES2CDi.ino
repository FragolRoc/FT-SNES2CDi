
/*******************************
 *
 *  FT_SNES2CDi_Arduino_v1.ino
 *  Description: software for the SNEStoCDi gamepad converter (allows to use a Nintendo SNES/SFC gamepad on a Philips CDi)
 *  Author: Laurent Berta
 *  Edited by: Fragol Roc for use ONLY with FragolTech SNES2CDi Adapter Board
 *  Date: 01-03-2021
 *  Version: 1.0
 *  Thanks: Rob Duarte (SNESpad library), Paul Hackmann (documentation about the CDi pointing devices)
 *  License: CC-BY 4.0 ( http://creativecommons.org/licenses/by/4.0/legalcode )
 *
 ******************************/


#include "Pad.h"
#include "SNESpad.h"

SNESpad snesPadA = SNESpad(A4, A5, 2); // Create a SNESpad instance, change the pin values to match your wiring (latch, clock, data)
SNESpad snesPadB = SNESpad(A3, A2, 7);
Pad padA = Pad(6, 10, 13); // RTSpin, RXDpin, LEDpin
Pad padB = Pad(7, 9, 14);
bool debugging = false;

// init
void setup()
{
  if (debugging) Serial.begin(38400);
}

// main
void loop()
{
  padA.task();
  padB.task();

	// Get the state of the SNES pad buttons
  uint32_t snesBtnsA = snesPadA.buttons(SNES_MOUSE_FAST);
  uint32_t snesBtnsB = snesPadB.buttons(SNES_MOUSE_FAST);

  if (snesBtnsA && !snesBtnsB) {
    padA.snes2cdi(snesBtnsA);
  } else if (!snesBtnsA && snesBtnsB) {
    padA.snes2cdi(snesBtnsB);
  } else if (snesBtnsA && snesBtnsB) {
    padA.snes2cdi(snesBtnsB);
    padB.snes2cdi(snesBtnsA);
  }

  padA.send();
  padB.send();
}
