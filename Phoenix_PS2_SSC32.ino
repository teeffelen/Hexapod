//=============================================================================
// Header Files
//=============================================================================

#define DEFINE_HEX_GLOBALS
#if ARDUINO>99
#include <Arduino.h>
#else
#endif
#include <EEPROM.h>
#include <PS2X_lib.h>
#include <SoftwareSerial.h>

#include "Hex_CFG.h"
#include "Phoenix.h"
#include "Phoenix_Input_PS2.h"
#include "Phoenix_Driver_SSC32.h"
#include "Phoenix_Code.h"

