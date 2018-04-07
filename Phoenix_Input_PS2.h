//====================================================================
// V2.5
//====================================================================
// [Include files]
#if ARDUINO>99
#include <Arduino.h> // Arduino 1.0
#else
#include <Wprogram.h> // Arduino 0022
#endif
#include <PS2X_lib.h>

//[CONSTANTS]
#define WALKMODE          0
#define TRANSLATEMODE     1
#define ROTATEMODE        2
#define SINGLELEGMODE     3
#define GPPLAYERMODE      4

#define cTravelDeadZone   4
#define  MAXPS2ERRORCNT   6

#ifndef MAX_BODY_Y
#define MAX_BODY_Y 100
#endif

//=============================================================================
// Global - Local to this file only...
//=============================================================================
PS2X ps2x;

InputController  g_InputController;

static short       g_BodyYOffset;
static short       g_sPS2ErrorCnt;
static short       g_BodyYShift;
static byte        ControlMode;
static bool        DoubleHeightOn;
static bool        DoubleTravelOn;
static bool        WalkMethod;
byte               GPSeq;
short              g_sGPSMController;

extern void PS2TurnRobotOff(void);

//==============================================================================
// This is The function that is called by the Main program to initialize
//the input controller, which in this case is the PS2 controller
//==============================================================================
void InputController::Init(void)
{
  int error;

  //error = ps2x.config_gamepad(57, 55, 56, 54);
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT);

#ifdef DBGSerial
  DBGSerial.print("PS2 Init: ");
  DBGSerial.println(error, DEC);
#endif
  g_BodyYOffset = 0;
  g_BodyYShift = 0;
  g_sPS2ErrorCnt = 0;

  ControlMode = WALKMODE;
  DoubleHeightOn = false;
  DoubleTravelOn = false;
  WalkMethod = false;

  g_InControlState.SpeedControl = 100;
}

void InputController::AllowControllerInterrupts(boolean fAllow)
{
  // We don't need to do anything...
}

//==============================================================================
// This is The main code to input function to read inputs from the PS2 and then
//process any commands.
//==============================================================================
#ifdef OPT_DYNAMIC_ADJUST_LEGS
boolean g_fDynamicLegXZLength = false;
#endif

void InputController::ControlInput(void)
{
  boolean fAdjustLegPositions = false;
  ps2x.read_gamepad();

  if ((ps2x.Analog(1) & 0xf0) == 0x70) {
#ifdef DBGSerial
#ifdef DEBUG_PS2_INPUT
  if (g_fDebugOutput) {
    DBGSerial.print("PS2 Input: ");
    DBGSerial.print(ps2x.ButtonDataByte(), HEX);
    DBGSerial.print(":");
    DBGSerial.print(ps2x.Analog(PSS_LX), DEC);
    DBGSerial.print(" ");
    DBGSerial.print(ps2x.Analog(PSS_LY), DEC);
    DBGSerial.print(" ");
    DBGSerial.print(ps2x.Analog(PSS_RX), DEC);
    DBGSerial.print(" ");
    DBGSerial.println(ps2x.Analog(PSS_RY), DEC);
  }
#endif
#endif

#ifdef OPT_DYNAMIC_ADJUST_LEGS
    boolean fAdjustLegPositions = false;
    short sLegInitXZAdjust = 0;
    short sLegInitAngleAdjust = 0;
#endif
    g_sPS2ErrorCnt = 0;

    if (ps2x.ButtonPressed(PSB_START)) {
      if (g_InControlState.fRobotOn) {
        PS2TurnRobotOff();
      }
      else {
        g_InControlState.fRobotOn = 1;
        fAdjustLegPositions = true;
      }
    }

    if (g_InControlState.fRobotOn) {
      if (ps2x.ButtonPressed(PSB_L1)) {
        MSound( 1, 50, 2000);
        if (ControlMode != TRANSLATEMODE )
          ControlMode = TRANSLATEMODE;
        else {
#ifdef OPT_SINGLELEG
          if (g_InControlState.SelectedLeg==255)
            ControlMode = WALKMODE;
          else
#endif
            ControlMode = SINGLELEGMODE;
        }
      }
      if (ps2x.ButtonPressed(PSB_L2)) {
        MSound( 1, 50, 2000);
        if (ControlMode != ROTATEMODE)
          ControlMode = ROTATEMODE;
        else {
#ifdef OPT_SINGLELEG
          if (g_InControlState.SelectedLeg == 255)
            ControlMode = WALKMODE;
          else
#endif
            ControlMode = SINGLELEGMODE;
        }
      }

#ifdef OPT_SINGLELEG
      if (ps2x.ButtonPressed(PSB_CIRCLE)) {
        if (abs(g_InControlState.TravelLength.x)<cTravelDeadZone && abs(g_InControlState.TravelLength.z)<cTravelDeadZone
          && abs(g_InControlState.TravelLength.y*2)<cTravelDeadZone )   {
          if (ControlMode != SINGLELEGMODE) {
            ControlMode = SINGLELEGMODE;
            if (g_InControlState.SelectedLeg == 255)
              g_InControlState.SelectedLeg=cRF;
          }
          else {
            ControlMode = WALKMODE;
            g_InControlState.SelectedLeg=255;
          }
        }
      }
#endif
#ifdef OPT_GPPLAYER
      if (ps2x.ButtonPressed(PSB_CROSS)) {
        MSound(1, 50, 2000);
        if (ControlMode != GPPLAYERMODE) {
          ControlMode = GPPLAYERMODE;
          GPSeq=0;
        }
        else
          ControlMode = WALKMODE;
      }
#endif
      if (ps2x.ButtonPressed(PSB_SQUARE)) {
        g_InControlState.BalanceMode = !g_InControlState.BalanceMode;
        if (g_InControlState.BalanceMode) {
          MSound(1, 250, 1500);
        }
        else {
          MSound( 2, 100, 2000, 50, 4000);
        }
      }
      if (ps2x.ButtonPressed(PSB_TRIANGLE)) {
        if (g_BodyYOffset>0)
          g_BodyYOffset = 0;
        else
          g_BodyYOffset = 35;
        fAdjustLegPositions = true;
      }

      if (ps2x.ButtonPressed(PSB_PAD_UP)) {
        g_BodyYOffset += 10;
        fAdjustLegPositions = true;
        if (g_BodyYOffset > MAX_BODY_Y)
          g_BodyYOffset = MAX_BODY_Y;
      }

      if (ps2x.ButtonPressed(PSB_PAD_DOWN) && g_BodyYOffset) {
        if (g_BodyYOffset > 10)
          g_BodyYOffset -= 10;
        else
          g_BodyYOffset = 0;

        fAdjustLegPositions = true;
      }

      if (ps2x.ButtonPressed(PSB_PAD_RIGHT)) {
        if (g_InControlState.SpeedControl>0) {
          g_InControlState.SpeedControl = g_InControlState.SpeedControl - 50;
          MSound( 1, 50, 2000);
        }
      }

      if (ps2x.ButtonPressed(PSB_PAD_LEFT)) {
        if (g_InControlState.SpeedControl<2000 ) {
          g_InControlState.SpeedControl = g_InControlState.SpeedControl + 50;
          MSound( 1, 50, 2000);
        }
      }

      byte lx = ps2x.Analog(PSS_LX);
      byte ly = ps2x.Analog(PSS_LY);
#ifdef OPT_DYNAMIC_ADJUST_LEGS
#ifdef OPT_SINGLELEG
      if (ps2x.Button(PSB_L3)) {
#else
      if (ps2x.Button(PSB_CIRCLE)) {
#endif
        sLegInitXZAdjust = ((int)lx-128)/10;
        sLegInitAngleAdjust = ((int)ly-128)/8;
        lx = 0;
        ly = 0;
      }
#endif

//==============================================================================

      if (ControlMode == WALKMODE) {
        if (ps2x.ButtonPressed(PSB_SELECT)
        && abs(g_InControlState.TravelLength.x)<cTravelDeadZone
        && abs(g_InControlState.TravelLength.z)<cTravelDeadZone
          && abs(g_InControlState.TravelLength.y*2)<cTravelDeadZone  ) {
          g_InControlState.GaitType = g_InControlState.GaitType+1;
          if (g_InControlState.GaitType<NUM_GAITS) {
            MSound( 1, 50, 2000);
          }
          else {
            MSound(2, 50, 2000, 50, 2250);
            g_InControlState.GaitType = 0;
          }
          GaitSelect();
        }

        if (ps2x.ButtonPressed(PSB_R1)) {
          MSound( 1, 50, 2000);
          DoubleHeightOn = !DoubleHeightOn;
          if (DoubleHeightOn)
            g_InControlState.LegLiftHeight = 80;
          else
            g_InControlState.LegLiftHeight = 50;
        }

        if (ps2x.ButtonPressed(PSB_R2)) {
          MSound(1, 50, 2000);
          DoubleTravelOn = !DoubleTravelOn;
        }

        if (ps2x.ButtonPressed(PSB_R3)) {
          MSound(1, 50, 2000);
          WalkMethod = !WalkMethod;
        }

        if (WalkMethod)
          g_InControlState.TravelLength.z = (ps2x.Analog(PSS_RY)-128);

        else {
          g_InControlState.TravelLength.x = -(lx - 128);
          g_InControlState.TravelLength.z = (ly - 128);
        }

        if (!DoubleTravelOn) {
          g_InControlState.TravelLength.x = g_InControlState.TravelLength.x/2;
          g_InControlState.TravelLength.z = g_InControlState.TravelLength.z/2;
        }

        g_InControlState.TravelLength.y = -(ps2x.Analog(PSS_RX) - 128)/4;
      }

//==============================================================================

      g_BodyYShift = 0;
      if (ControlMode == TRANSLATEMODE) {
        g_InControlState.BodyPos.x = (lx - 128)/2;
        g_InControlState.BodyPos.z = -(ly - 128)/3;
        g_InControlState.BodyRot1.y = (ps2x.Analog(PSS_RX) - 128)*2;
        g_BodyYShift = (-(ps2x.Analog(PSS_RY) - 128)/2);
      }

//==============================================================================

      if (ControlMode == ROTATEMODE) {
        g_InControlState.BodyRot1.x = (ly - 128);
        g_InControlState.BodyRot1.y = (ps2x.Analog(PSS_RX) - 128)*2;
        g_InControlState.BodyRot1.z = (lx - 128);
        g_BodyYShift = (-(ps2x.Analog(PSS_RY) - 128)/2);
      }

//==============================================================================

#ifdef OPT_SINGLELEG
      if (ControlMode == SINGLELEGMODE) {
        if (ps2x.ButtonPressed(PSB_SELECT)) {
          MSound(1, 50, 2000);
          if (g_InControlState.SelectedLeg<(CNT_LEGS-1))
            g_InControlState.SelectedLeg = g_InControlState.SelectedLeg+1;
          else
            g_InControlState.SelectedLeg=0;
        }

        g_InControlState.SLLeg.x= (lx - 128)/2;
        g_InControlState.SLLeg.y= (ps2x.Analog(PSS_RY) - 128)/10;
        g_InControlState.SLLeg.z = (ly - 128)/2;

        if (ps2x.ButtonPressed(PSB_R2)) {
          MSound(1, 50, 2000);
          g_InControlState.fSLHold = !g_InControlState.fSLHold;
        }
      }
#endif
#ifdef OPT_GPPLAYER
      if (ControlMode == GPPLAYERMODE) {
        if (g_ServoDriver.FIsGPSeqActive() ) {
          if ((g_sGPSMController != 32767)
            || (ps2x.Analog(PSS_RY) > (128+16)) || (ps2x.Analog(PSS_RY) < (128-16)))
          {
            short sNewGPSM = map(ps2x.Analog(PSS_RY), 0, 255, -200, 200);
            if (sNewGPSM != g_sGPSMController) {
              g_sGPSMController = sNewGPSM;
              g_ServoDriver.GPSetSpeedMultiplyer(g_sGPSMController);
            }
          }
        }

        if (ps2x.ButtonPressed(PSB_SELECT)) {
          if (!g_ServoDriver.FIsGPSeqActive() ) {
            if (GPSeq < 5) {
              MSound(1, 50, 1500);
              GPSeq = GPSeq+1;
            }
            else {
              MSound(2, 50, 2000, 50, 2250);
              GPSeq=0;
            }
          }
        }
        if (ps2x.ButtonPressed(PSB_R2))
          if (!g_ServoDriver.FIsGPSeqActive() ) {
          g_ServoDriver.GPStartSeq(GPSeq);
            g_sGPSMController = 32767;
          }
          else {
            g_ServoDriver.GPStartSeq(0xff);
            MSound (2, 50, 2000, 50, 2000);
          }
      }
#endif

      g_InControlState.InputTimeDelay = 128 - max(max(abs(lx - 128), abs(ly - 128)), abs(ps2x.Analog(PSS_RX) - 128));
    }

    g_InControlState.BodyPos.y = min(max(g_BodyYOffset + g_BodyYShift,  0), MAX_BODY_Y);

#ifdef OPT_DYNAMIC_ADJUST_LEGS
    if (sLegInitXZAdjust || sLegInitAngleAdjust) {
      if (!g_InControlState.ForceGaitStepCnt) {
        if (sLegInitXZAdjust)
          g_fDynamicLegXZLength = true;

        sLegInitXZAdjust += GetLegsXZLength();
        if (sLegInitAngleAdjust)
            RotateLegInitAngles(sLegInitAngleAdjust);
        AdjustLegPositions(sLegInitXZAdjust);
      }
    }
#endif

    if (fAdjustLegPositions)
      AdjustLegPositionsToBodyHeight();
  }
  else {
    if (g_sPS2ErrorCnt < MAXPS2ERRORCNT)
      g_sPS2ErrorCnt++;
    else if (g_InControlState.fRobotOn)
      PS2TurnRobotOff();
    ps2x.reconfig_gamepad();
  }
}

//==============================================================================
// PS2TurnRobotOff - code used couple of places so save a little room...
//==============================================================================
void PS2TurnRobotOff(void)
{
  g_InControlState.BodyPos.x = 0;
  g_InControlState.BodyPos.y = 0;
  g_InControlState.BodyPos.z = 0;
  g_InControlState.BodyRot1.x = 0;
  g_InControlState.BodyRot1.y = 0;
  g_InControlState.BodyRot1.z = 0;
  g_InControlState.TravelLength.x = 0;
  g_InControlState.TravelLength.z = 0;
  g_InControlState.TravelLength.y = 0;
  g_BodyYOffset = 0;
  g_BodyYShift = 0;
#ifdef OPT_SINGLELEG
  g_InControlState.SelectedLeg = 255;
#endif
  g_InControlState.fRobotOn = 0;
  AdjustLegPositionsToBodyHeight();
}
