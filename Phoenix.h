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
#ifndef _PHOENIX_CORE_H_
#define _PHOENIX_CORE_H_
#include <stdarg.h>
//#include <EEPROM.h>
#if defined(__SAM3X8E__)
    #define PROGMEM
    #define pgm_read_byte(x)        (*((char *)x))
//  #define pgm_read_word(x)        (*((short *)(x & 0xfffffffe)))
    #define pgm_read_word(x)        ( ((*((unsigned char *)x + 1)) << 8) + (*((unsigned char *)x)))
    #define pgm_read_byte_near(x)   (*((char *)x))
    #define pgm_read_byte_far(x)    (*((char *)x))
//  #define pgm_read_word_near(x)   (*((short *)(x & 0xfffffffe))
//  #define pgm_read_word_far(x)    (*((short *)(x & 0xfffffffe)))
    #define pgm_read_word_near(x)   ( ((*((unsigned char *)x + 1)) << 8) + (*((unsigned char *)x)))
    #define pgm_read_word_far(x)    ( ((*((unsigned char *)x + 1)) << 8) + (*((unsigned char *)x))))
    #define PSTR(x)  x
#endif

#ifdef USEXBEE
#include "diyxbee.h"
#endif

//==============================================================================
//[CONSTANTS]
//==============================================================================
#define BUTTON_DOWN 0
#define BUTTON_UP   1

#define c1DEC   10
#define c2DEC   100
#define c4DEC   10000
#define c6DEC   1000000

#ifdef QUADMODE
enum {
  cRR=0, cRF, cLR, cLF, CNT_LEGS};
#else
enum {
  cRR=0, cRM, cRF, cLR, cLM, cLF, CNT_LEGS};
#endif

#define WTIMERTICSPERMSMUL    64
#define WTIMERTICSPERMSDIV    125
#define USEINT_TIMERAV

extern  byte  NUM_GAITS;
#define SmDiv      4
extern void GaitSelect(void);
extern short SmoothControl (short CtrlMoveInp, short CtrlMoveOut, byte CtrlDivider);

//==============================================================================
// Define Global variables
//==============================================================================
extern boolean          g_fDebugOutput;
extern boolean          g_fEnableServos;
extern boolean          g_fRobotUpsideDown;

extern void MSound(byte cNotes, ...);
extern boolean CheckVoltage(void);

extern word GetLegsXZLength(void);
extern void AdjustLegPositions(word XZLength1);
extern void AdjustLegPositionsToBodyHeight();
extern void ResetLegInitAngles(void);
extern void RotateLegInitAngles (int iDeltaAngle);
extern long GetCmdLineNum(byte **ppszCmdLine);

extern boolean g_fDBGHandleError;

#ifdef c4DOF
extern const byte cTarsLength[] PROGMEM;
#endif

#ifdef OPT_BACKGROUND_PROCESS
#define DoBackgroundProcess()   g_ServoDriver.BackgroundProcess()
#else
#define DoBackgroundProcess()
#endif

#ifdef DEBUG_IOPINS
#define DebugToggle(pin)  {digitalWrite(pin, !digitalRead(pin));}
#define DebugWrite(pin, state) {digitalWrite(pin, state);}
#else
#define DebugToggle(pin)  {;}
#define DebugWrite(pin, state) {;}
#endif

#ifdef __AVR__
#if not defined(UBRR1H)
#if cSSC_IN != 0
extern SoftwareSerial SSCSerial;
#endif
#endif
#endif
#if defined(__PIC32MX__)
  #if defined F
    #undef F
  #endif
  #define F(X) (X)
#endif

//==============================================================================
// Define the class(s) for our Input controllers.
//==============================================================================
class InputController {
public:
    virtual void     Init(void);
    virtual void     ControlInput(void);
    virtual void     AllowControllerInterrupts(boolean fAllow);

#ifdef OPT_TERMINAL_MONITOR_IC
  void            ShowTerminalCommandList(void);
  boolean         ProcessTerminalCommand(byte *psz, byte bLen);
#endif

private:
}
;

extern void  RegisterInputController(InputController *pic);

typedef struct _Coord3D {
  long      x;
  long      y;
  long      z;
}
COORD3D;

//==============================================================================
// Define Gait structure/class - Hopefully allow specific robots to define their
// own gaits and/or define which of the standard ones they want.
//==============================================================================
typedef struct _PhoenixGait {
  short     NomGaitSpeed;
  byte      StepsInGait;
  byte      NrLiftedPos;
  byte      FrontDownPos;
  byte      LiftDivFactor;
  byte      TLDivFactor;
  byte      HalfLiftHeight;

#ifdef QUADMODE
    word      COGAngleStart1;
    word      COGAngleStep1;
    byte      COGRadius;
    boolean   COGCCW;
#endif
  byte        GaitLegNr[CNT_LEGS];
#ifdef DISPLAY_GAIT_NAMES
    PGM_P     pszName;
#endif
}
PHOENIXGAIT;

#ifdef DISPLAY_GAIT_NAMES
#define GAITNAME(name)  ,name
#else
#define GAITNAME(name)
#endif

//==============================================================================
// class ControlState: This is the main structure of data that the Control
//      manipulates and is used by the main Phoenix Code to make it do what is
//      requested.
//==============================================================================
typedef struct _InControlState {
  boolean       fRobotOn;
  boolean       fPrev_RobotOn;
  COORD3D       BodyPos;
  COORD3D       BodyRotOffset;
  COORD3D       BodyRot1;

  byte          GaitType;
  byte          GaitStep;
  PHOENIXGAIT   gaitCur;

  short         LegLiftHeight;
  COORD3D       TravelLength;

#ifdef cTurretRotPin
  int           TurretRotAngle1;
  int           TurretTiltAngle1;
#endif

#ifdef OPT_SINGLELEG
  byte          SelectedLeg;
  COORD3D       SLLeg;
  boolean       fSLHold;
#endif

  boolean       BalanceMode;

  byte          InputTimeDelay;
  word          SpeedControl;
  byte          ForceGaitStepCnt;

#ifdef OPT_DYNAMIC_ADJUST_LEGS
  short         aCoxaInitAngle1[CNT_LEGS];
#endif
}
INCONTROLSTATE;

//==============================================================================
// Define the class(s) for Servo Drivers.
//==============================================================================
class ServoDriver {
public:
  void Init(void);

    uint16_t GetBatteryVoltage(void);

#ifdef OPT_GPPLAYER
  inline boolean  FIsGPEnabled(void) {
    return _fGPEnabled;
  };
  boolean         FIsGPSeqDefined(uint8_t iSeq);
  inline boolean  FIsGPSeqActive(void) {
    return _fGPActive;
  };
  void            GPStartSeq(uint8_t iSeq);
  void            GPPlayer(void);
  uint8_t         GPNumSteps(void);
  uint8_t         GPCurStep(void);
  void            GPSetSpeedMultiplyer(short sm);
#endif
  void        BeginServoUpdate(void);
#ifdef c4DOF
  void            OutputServoInfoForLeg(byte LegIndex, short sCoxaAngle1, short sFemurAngle1, short sTibiaAngle1, short sTarsAngle1);
#else
  void            OutputServoInfoForLeg(byte LegIndex, short sCoxaAngle1, short sFemurAngle1, short sTibiaAngle1);
#endif
#ifdef cTurretRotPin
  void            OutputServoInfoForTurret(short sRotateAngle1, short sTiltAngle1);
#endif
  void            CommitServoDriver(word wMoveTime);
  void            FreeServos(void);

  void            IdleTime(void);

#ifdef OPT_BACKGROUND_PROCESS
  void            BackgroundProcess(void);
#endif

#ifdef OPT_TERMINAL_MONITOR
  void            ShowTerminalCommandList(void);
  boolean         ProcessTerminalCommand(byte *psz, byte bLen);
#endif

private:

#ifdef OPT_GPPLAYER
  boolean _fGPEnabled;
  boolean _fGPActive;
  uint8_t    _iSeq;
    short    _sGPSM;
#endif
}
;

//==============================================================================
// Define global class objects
//==============================================================================
extern ServoDriver      g_ServoDriver;
extern InputController  g_InputController;
extern INCONTROLSTATE   g_InControlState;

#endif
