//==============================================================================
//Lynxmotion Phoenix Clone
//
//Version:    3.1
//Date:       09-04-2018
//Programmer: Jeroen Janssen    (Xan)   - Main code
//            Kurt Eckhardt     (KurtE) - Converted to arduino
//            Lex van Teeffelen (Lexons)- Converted to AliExpress Hexapod
//
//This code should only be used on phoenix clones running the 
// Lynxmotion SSC32-U and PS2 remote.
//==============================================================================

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
