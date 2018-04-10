//==============================================================================
// V2.1
//==============================================================================

#define DEFINE_HEX_GLOBALS
#if ARDUINO>99
#include <Arduino.h>
#else
#endif
//#include <EEPROM.h>
//#include <PS2X_lib.h>
#include <pins_arduino.h>
//#include <SoftwareSerial.h>
#define BalanceDivFactor CNT_LEGS
//#include <Wire.h>
//#include <I2CEEProm.h>

#ifdef DBGSerial
#define DEBUG
//#define DEBUG_X
#endif

//==============================================================================
//[TABLES]

static const byte GetACos[] PROGMEM = {
  255,254,252,251,250,249,247,246,245,243,242,241,240,238,237,236,234,233,232,231,229,228,227,225,
  224,223,221,220,219,217,216,215,214,212,211,210,208,207,206,204,203,201,200,199,197,196,195,193,
  192,190,189,188,186,185,183,182,181,179,178,176,175,173,172,170,169,167,166,164,163,161,160,158,
  157,155,154,152,150,149,147,146,144,142,141,139,137,135,134,132,130,128,127,125,123,121,119,117,
  115,113,111,109,107,105,103,101,98,96,94,92,89,87,84,81,79,76,73,73,73,72,72,72,71,71,71,70,70,
  70,70,69,69,69,68,68,68,67,67,67,66,66,66,65,65,65,64,64,64,63,63,63,62,62,62,61,61,61,60,60,59,
  59,59,58,58,58,57,57,57,56,56,55,55,55,54,54,53,53,53,52,52,51,51,51,50,50,49,49,48,48,47,47,47,
  46,46,45,45,44,44,43,43,42,42,41,41,40,40,39,39,38,37,37,36,36,35,34,34,33,33,32,31,31,30,29,28,
  28,27,26,25,24,23,23,23,23,22,22,22,22,21,21,21,21,20,20,20,19,19,19,19,18,18,18,17,17,17,17,16,
  16,16,15,15,15,14,14,13,13,13,12,12,11,11,10,10,9,9,8,7,6,6,5,3,0 };//

static const word GetSin[] PROGMEM = {
  0, 87, 174, 261, 348, 436, 523, 610, 697, 784, 871, 958, 1045, 1132, 1218, 1305, 1391, 1478, 1564,
  1650, 1736, 1822, 1908, 1993, 2079, 2164, 2249, 2334, 2419, 2503, 2588, 2672, 2756, 2840, 2923, 3007,
  3090, 3173, 3255, 3338, 3420, 3502, 3583, 3665, 3746, 3826, 3907, 3987, 4067, 4146, 4226, 4305, 4383,
  4461, 4539, 4617, 4694, 4771, 4848, 4924, 4999, 5075, 5150, 5224, 5299, 5372, 5446, 5519, 5591, 5664,
  5735, 5807, 5877, 5948, 6018, 6087, 6156, 6225, 6293, 6360, 6427, 6494, 6560, 6626, 6691, 6755, 6819,
  6883, 6946, 7009, 7071, 7132, 7193, 7253, 7313, 7372, 7431, 7489, 7547, 7604, 7660, 7716, 7771, 7826,
  7880, 7933, 7986, 8038, 8090, 8141, 8191, 8241, 8290, 8338, 8386, 8433, 8480, 8526, 8571, 8616, 8660,
  8703, 8746, 8788, 8829, 8870, 8910, 8949, 8987, 9025, 9063, 9099, 9135, 9170, 9205, 9238, 9271, 9304,
  9335, 9366, 9396, 9426, 9455, 9483, 9510, 9537, 9563, 9588, 9612, 9636, 9659, 9681, 9702, 9723, 9743,
  9762, 9781, 9799, 9816, 9832, 9848, 9862, 9876, 9890, 9902, 9914, 9925, 9935, 9945, 9953, 9961, 9969,
  9975, 9981, 9986, 9990, 9993, 9996, 9998, 9999, 10000 };//

//==============================================================================
// Inversed for Towerpro MG996R servo's
// Change values if you are using other servo's and/or experiencing problems...

#ifndef cRRCoxaInv
#define cRRCoxaInv 0
#endif
#ifndef cRMCoxaInv
#define cRMCoxaInv 0
#endif
#ifndef cRFCoxaInv
#define cRFCoxaInv 0
#endif

#ifndef cLRCoxaInv
#define cLRCoxaInv 1
#endif
#ifndef cLMCoxaInv
#define cLMCoxaInv 1
#endif
#ifndef cLFCoxaInv
#define cLFCoxaInv 1
#endif

#ifndef cRRFemurInv
#define cRRFemurInv 0
#endif
#ifndef cRMFemurInv
#define cRMFemurInv 0
#endif
#ifndef cRFFemurInv
#define cRFFemurInv 0
#endif

#ifndef cLRFemurInv
#define cLRFemurInv 1
#endif
#ifndef cLMFemurInv
#define cLMFemurInv 1
#endif
#ifndef cLFFemurInv
#define cLFFemurInv 1
#endif

#ifndef cRRTibiaInv
#define cRRTibiaInv 0
#endif
#ifndef cRMTibiaInv
#define cRMTibiaInv 0
#endif
#ifndef cRFTibiaInv
#define cRFTibiaInv 0
#endif

#ifndef cLRTibiaInv
#define cLRTibiaInv 1
#endif
#ifndef cLMTibiaInv
#define cLMTibiaInv 1
#endif
#ifndef cLFTibiaInv
#define cLFTibiaInv 1
#endif

#ifndef cRRTarsInv
#define cRRTarsInv 0
#endif
#ifndef cRMTarsInv
#define cRMTarsInv 0
#endif
#ifndef cRFTarsInv
#define cRFTarsInv 0
#endif

#ifndef cLRTarsInv
#define cLRTarsInv 0
#endif
#ifndef cLMTarsInv
#define cLMTarsInv 0
#endif
#ifndef cLFTarsInv
#define cLFTarsInv 0
#endif

//==============================================================================

#ifndef BALANCE_DELAY
#define BALANCE_DELAY 100
#endif

#ifndef QUADMODE
#ifdef cRRFemurHornOffset1
static const short cFemurHornOffset1[] PROGMEM = {
  cRRFemurHornOffset1, cRMFemurHornOffset1, cRFFemurHornOffset1, cLRFemurHornOffset1, cLMFemurHornOffset1, cLFFemurHornOffset1};
#define CFEMURHORNOFFSET1(LEGI) ((short)pgm_read_word(&cFemurHornOffset1[LEGI]))
#else
#ifndef cFemurHornOffset1
#define cFemurHornOffset1  0
#endif
#define CFEMURHORNOFFSET1(LEGI)  (cFemurHornOffset1)
#endif

#ifdef cRRTibiaHornOffset1
static const short cTibiaHornOffset1[] PROGMEM = {
  cRRTibiaHornOffset1, cRMTibiaHornOffset1, cRFTibiaHornOffset1, cLRTibiaHornOffset1, cLMTibiaHornOffset1, cLFTibiaHornOffset1};
#define CTIBIAHORNOFFSET1(LEGI) ((short)pgm_read_word(&cTibiaHornOffset1[LEGI]))
#else
#ifndef cTibiaHornOffset1
#define cTibiaHornOffset1  0
#endif
#define CTIBIAHORNOFFSET1(LEGI)  (cTibiaHornOffset1)
#endif

#ifdef c4DOF
#ifdef cRRTarsHornOffset1
static const short cTarsHornOffset1[] PROGMEM = {
  cRRTarsHornOffset1,  cRMTarsHornOffset1,  cRFTarsHornOffset1,  cLRTarsHornOffset1,  cLMTarsHornOffset1,  cLFTarsHornOffset1};
#define CTARSHORNOFFSET1(LEGI) ((short)pgm_read_word(&cTarsHornOffset1[LEGI]))
#else
#ifndef cTarsHornOffset1
#define cTarsHornOffset1  0
#endif
#define CTARSHORNOFFSET1(LEGI)  cTarsHornOffset1
#endif
#endif

#ifndef SERVOS_DO_MINMAX
const short cCoxaMin1[] PROGMEM = {
  cRRCoxaMin1,  cRMCoxaMin1,  cRFCoxaMin1,  cLRCoxaMin1,  cLMCoxaMin1,  cLFCoxaMin1};
const short cCoxaMax1[] PROGMEM = {
  cRRCoxaMax1,  cRMCoxaMax1,  cRFCoxaMax1,  cLRCoxaMax1,  cLMCoxaMax1,  cLFCoxaMax1};
const short cFemurMin1[] PROGMEM ={
  cRRFemurMin1, cRMFemurMin1, cRFFemurMin1, cLRFemurMin1, cLMFemurMin1, cLFFemurMin1};
const short cFemurMax1[] PROGMEM ={
  cRRFemurMax1, cRMFemurMax1, cRFFemurMax1, cLRFemurMax1, cLMFemurMax1, cLFFemurMax1};
const short cTibiaMin1[] PROGMEM ={
  cRRTibiaMin1, cRMTibiaMin1, cRFTibiaMin1, cLRTibiaMin1, cLMTibiaMin1, cLFTibiaMin1};
const short cTibiaMax1[] PROGMEM = {
  cRRTibiaMax1, cRMTibiaMax1, cRFTibiaMax1, cLRTibiaMax1, cLMTibiaMax1, cLFTibiaMax1};

#ifdef c4DOF
const short cTarsMin1[] PROGMEM = {
  cRRTarsMin1, cRMTarsMin1, cRFTarsMin1, cLRTarsMin1, cLMTarsMin1, cLFTarsMin1};
const short cTarsMax1[] PROGMEM = {
  cRRTarsMax1, cRMTarsMax1, cRFTarsMax1, cLRTarsMax1, cLMTarsMax1, cLFTarsMax1};
#endif
#endif

const bool cCoxaInv[] = {cRRCoxaInv, cRMCoxaInv, cRFCoxaInv, cLRCoxaInv, cLMCoxaInv, cLFCoxaInv};
bool cFemurInv[] = {cRRFemurInv, cRMFemurInv, cRFFemurInv, cLRFemurInv, cLMFemurInv, cLFFemurInv};
const bool cTibiaInv[] = {cRRTibiaInv, cRMTibiaInv, cRFTibiaInv, cLRTibiaInv, cLMTibiaInv, cLFTibiaInv};

#ifdef c4DOF
const boolean cTarsInv[] = {cRRTarsInv, cRMTarsInv, cRFTarsInv, cLRTarsInv, cLMTarsInv, cLFTarsInv};
#endif

const byte cCoxaLength[] PROGMEM = {
  cRRCoxaLength,  cRMCoxaLength,  cRFCoxaLength,  cLRCoxaLength,  cLMCoxaLength,  cLFCoxaLength};
const byte cFemurLength[] PROGMEM = {
  cRRFemurLength, cRMFemurLength, cRFFemurLength, cLRFemurLength, cLMFemurLength, cLFFemurLength};
const byte cTibiaLength[] PROGMEM = {
  cRRTibiaLength, cRMTibiaLength, cRFTibiaLength, cLRTibiaLength, cLMTibiaLength, cLFTibiaLength};
#ifdef c4DOF
const byte cTarsLength[] PROGMEM = {
  cRRTarsLength, cRMTarsLength, cRFTarsLength, cLRTarsLength, cLMTarsLength, cLFTarsLength};
#endif

const short cOffsetX[] PROGMEM = {
  cRROffsetX, cRMOffsetX, cRFOffsetX, cLROffsetX, cLMOffsetX, cLFOffsetX};
const short cOffsetZ[] PROGMEM = {
  cRROffsetZ, cRMOffsetZ, cRFOffsetZ, cLROffsetZ, cLMOffsetZ, cLFOffsetZ};

const short cCoxaAngle1[] PROGMEM = {
  cRRCoxaAngle1, cRMCoxaAngle1, cRFCoxaAngle1, cLRCoxaAngle1, cLMCoxaAngle1, cLFCoxaAngle1};

#ifdef cRRInitCoxaAngle1
const short cCoxaInitAngle1[] PROGMEM = {
  cRRInitCoxaAngle1, cRMInitCoxaAngle1, cRFInitCoxaAngle1, cLRInitCoxaAngle1, cLMInitCoxaAngle1, cLFInitCoxaAngle1};
#endif

const short cInitPosX[] PROGMEM = {
  cRRInitPosX, cRMInitPosX, cRFInitPosX, cLRInitPosX, cLMInitPosX, cLFInitPosX};
const short cInitPosY[] PROGMEM = {
  cRRInitPosY, cRMInitPosY, cRFInitPosY, cLRInitPosY, cLMInitPosY, cLFInitPosY};
const short cInitPosZ[] PROGMEM = {
  cRRInitPosZ, cRMInitPosZ, cRFInitPosZ, cLRInitPosZ, cLMInitPosZ, cLFInitPosZ};

//==============================================================================

#else
#ifdef cRRFemurHornOffset1
static const short cFemurHornOffset1[] PROGMEM = {
  cRRFemurHornOffset1, cRFFemurHornOffset1, cLRFemurHornOffset1, cLFFemurHornOffset1};
#define CFEMURHORNOFFSET1(LEGI) ((short)pgm_read_word(&cFemurHornOffset1[LEGI]))
#else
#ifndef cFemurHornOffset1
#define cFemurHornOffset1  0
#endif
#define CFEMURHORNOFFSET1(LEGI)  (cFemurHornOffset1)
#endif

#ifdef cRRTibiaHornOffset1
static const short cTibiaHornOffset1[] PROGMEM = {
  cRRTibiaHornOffset1, cRFTibiaHornOffset1, cLRTibiaHornOffset1, cLFTibiaHornOffset1};
#define CTIBIAHORNOFFSET1(LEGI) ((short)pgm_read_word(&cTibiaHornOffset1[LEGI]))
#else
#ifndef cTibiaHornOffset1
#define cTibiaHornOffset1  0
#endif
#define CTIBIAHORNOFFSET1(LEGI)  (cTibiaHornOffset1)
#endif

#ifdef c4DOF
#ifdef cRRTarsHornOffset1
static const short cTarsHornOffset1[] PROGMEM = {
  cRRTarsHornOffset1, cRFTarsHornOffset1,  cLRTarsHornOffset1, cLFTarsHornOffset1};
#define CTARSHORNOFFSET1(LEGI) ((short)pgm_read_word(&cTarsHornOffset1[LEGI]))
#else
#ifndef cTarsHornOffset1
#define cTarsHornOffset1  0
#endif
#define CTARSHORNOFFSET1(LEGI)  cTarsHornOffset1
#endif
#endif

#ifndef SERVOS_DO_MINMAX
const short cCoxaMin1[] PROGMEM = {
  cRRCoxaMin1,  cRFCoxaMin1,  cLRCoxaMin1,  cLFCoxaMin1};
const short cCoxaMax1[] PROGMEM = {
  cRRCoxaMax1,  cRFCoxaMax1,  cLRCoxaMax1,  cLFCoxaMax1};
const short cFemurMin1[] PROGMEM ={
  cRRFemurMin1, cRFFemurMin1, cLRFemurMin1, cLFFemurMin1};
const short cFemurMax1[] PROGMEM ={
  cRRFemurMax1, cRFFemurMax1, cLRFemurMax1, cLFFemurMax1};
const short cTibiaMin1[] PROGMEM ={
  cRRTibiaMin1, cRFTibiaMin1, cLRTibiaMin1, cLFTibiaMin1};
const short cTibiaMax1[] PROGMEM = {
  cRRTibiaMax1, cRFTibiaMax1, cLRTibiaMax1, cLFTibiaMax1};

#ifdef c4DOF
const short cTarsMin1[] PROGMEM = {
  cRRTarsMin1, cRFTarsMin1, cLRTarsMin1, cLFTarsMin1};
const short cTarsMax1[] PROGMEM = {
  cRRTarsMax1, cRFTarsMax1, cLRTarsMax1, cLFTarsMax1};
#endif
#endif

const bool cCoxaInv[] = {cRRCoxaInv, cRFCoxaInv, cLRCoxaInv, cLFCoxaInv};
bool cFemurInv[] = {cRRFemurInv, cRFFemurInv, cLRFemurInv, cLFFemurInv};
const bool cTibiaInv[] = {cRRTibiaInv, cRFTibiaInv, cLRTibiaInv, cLFTibiaInv};

#ifdef c4DOF
const boolean cTarsInv[] = {
  cRRTarsInv, cRFTarsInv, cLRTarsInv, cLFTarsInv};
#endif

const byte cCoxaLength[] PROGMEM = {
  cRRCoxaLength,  cRFCoxaLength,  cLRCoxaLength,  cLFCoxaLength};
const byte cFemurLength[] PROGMEM = {
  cRRFemurLength, cRFFemurLength, cLRFemurLength, cLFFemurLength};
const byte cTibiaLength[] PROGMEM = {
  cRRTibiaLength, cRFTibiaLength, cLRTibiaLength, cLFTibiaLength};
#ifdef c4DOF
const byte cTarsLength[] PROGMEM = {
  cRRTarsLength, cRFTarsLength, cLRTarsLength, cLFTarsLength};
#endif

const short cOffsetX[] PROGMEM = {
  cRROffsetX, cRFOffsetX, cLROffsetX, cLFOffsetX};
const short cOffsetZ[] PROGMEM = {
  cRROffsetZ, cRFOffsetZ, cLROffsetZ, cLFOffsetZ};

const short cCoxaAngle1[] PROGMEM = {
  cRRCoxaAngle1, cRFCoxaAngle1, cLRCoxaAngle1, cLFCoxaAngle1};

#ifdef cRRInitCoxaAngle1
const short cCoxaInitAngle1[] PROGMEM = {
  cRRInitCoxaAngle1, cRFInitCoxaAngle1, cLRInitCoxaAngle1, cLFInitCoxaAngle1};
#endif

const short cInitPosX[] PROGMEM = {
  cRRInitPosX, cRFInitPosX, cLRInitPosX, cLFInitPosX};
const short cInitPosY[] PROGMEM = {
  cRRInitPosY, cRFInitPosY, cLRInitPosY, cLFInitPosY};
const short cInitPosZ[] PROGMEM = {
  cRRInitPosZ, cRFInitPosZ, cLRInitPosZ, cLFInitPosZ};

#endif

boolean g_fShowDebugPrompt;
boolean g_fDebugOutput;
boolean g_fEnableServos = true;

//==============================================================================
// [PS2 input]
#define cTravelDeadZone         4

//==============================================================================
//[Angles]
short           CoxaAngle1[CNT_LEGS];
short           FemurAngle1[CNT_LEGS];
short           TibiaAngle1[CNT_LEGS];
#ifdef c4DOF
short           TarsAngle1[CNT_LEGS];
#endif

//==============================================================================

short           LegPosX[CNT_LEGS];
short           LegPosY[CNT_LEGS];
short           LegPosZ[CNT_LEGS];

//==============================================================================

boolean         LedA;
boolean         LedB;
boolean         LedC;
boolean         Eyes;

byte            Index;
byte            LegIndex;

short           AngleDeg1;
short           sin4;
short           cos4;
short           AngleRad4;

short           AtanX;
short           AtanY;
short           Atan4;
long            XYhyp2;

short           PosX;
short           PosZ;
short           PosY;
long            BodyFKPosX;
long            BodyFKPosY;
long            BodyFKPosZ;

long            IKFeetPosX;
long            IKFeetPosY;
long            IKFeetPosZ;
boolean         IKSolution;
boolean         IKSolutionWarning;
boolean         IKSolutionError;

//==============================================================================

unsigned long   lTimerStart;
unsigned long   lTimerEnd;
byte            CycleTime;

word            ServoMoveTime;
word            PrevServoMoveTime;

//==============================================================================

INCONTROLSTATE   g_InControlState;

ServoDriver      g_ServoDriver;

boolean          g_fLowVoltageShutdown;
uint16_t         Voltage;

//boolean         g_InControlState.fRobotOn;
//boolean         g_InControlState.fPrev_RobotOn;

//==============================================================================

long            TotalTransX;
long            TotalTransZ;
long            TotalTransY;
long            TotalYBal1;
long            TotalXBal1;
long            TotalZBal1;

byte            PrevSelectedLeg;
boolean         AllDown;

boolean         TravelRequest;

long            GaitPosX[CNT_LEGS];
long            GaitPosY[CNT_LEGS];
long            GaitPosZ[CNT_LEGS];
long            GaitRotY[CNT_LEGS];

//boolean         GaitLegInAir[CNT_LEGS];
//byte            GaitNextLeg;

boolean         fWalking;
byte            bExtraCycle;
#define         cGPlimit 2

boolean        g_fRobotUpsideDown;
boolean        fRobotUpsideDownPrev;
//==============================================================================

#ifndef DEFAULT_GAIT_SPEED
#define DEFAULT_GAIT_SPEED 50
#define DEFAULT_SLOW_GAIT 70
#endif

//cRR=0, cRF, cLR, cLF, CNT_LEGS};

#ifndef OVERWRITE_GAITS
#ifndef QUADMODE
//  Speed, Steps, Lifted, Front Down, Lifted Factor, Half Height, On Ground,
//                      { RR, <RM> RF, LR, <LM>, LF}
#ifdef DISPLAY_GAIT_NAMES
extern "C" {
  const char s_szGN1[] PROGMEM = "Ripple 12";
  const char s_szGN2[] PROGMEM = "Tripod 8";
  const char s_szGN3[] PROGMEM = "Tripple 12";
  const char s_szGN4[] PROGMEM = "Tripple 16";
  const char s_szGN5[] PROGMEM = "Wave 24";
  const char s_szGN6[] PROGMEM = "Tripod 6";
};
#endif

PHOENIXGAIT APG[] = {
    {DEFAULT_SLOW_GAIT, 12, 3, 2, 2, 8, 3, {7, 11, 3, 1, 5, 9} GAITNAME(s_szGN1)},     // Ripple 12
    {DEFAULT_SLOW_GAIT, 8, 3, 2, 2, 4, 3, {1, 5, 1, 5, 1, 5} GAITNAME(s_szGN2)},       //Tripod 8 steps
    {DEFAULT_GAIT_SPEED, 12, 3, 2, 2, 8, 3, {5, 10, 3, 11, 4, 9} GAITNAME(s_szGN3) },  //Triple Tripod 12 step
    {DEFAULT_GAIT_SPEED, 16, 5, 3, 4, 10, 1, {6, 13, 4, 14, 5, 12} GAITNAME(s_szGN4)}, // Triple Tripod 16 steps, use 5 lifted positions
    {DEFAULT_SLOW_GAIT, 24, 3, 2, 2, 20, 3, {13, 17, 21, 1, 5, 9} GAITNAME(s_szGN5)},  //Wave 24 steps
    {DEFAULT_GAIT_SPEED, 6, 2, 1, 2, 4, 1, {1, 4, 1, 4, 1, 4} GAITNAME(s_szGN6)}       //Tripod 6 steps
};

#else
#ifdef DISPLAY_GAIT_NAMES
extern "C" {
  const char s_szGN1[] PROGMEM = "Ripple 12";
  const char s_szGN2[] PROGMEM = "Tripod 8";
}
#endif
PHOENIXGAIT APG[] = {
    {DEFAULT_GAIT_SPEED, 16, 3, 2, 2, 12, 3, 2250, 3600/16, 30, true, {5, 9, 1, 13} GAITNAME(s_szGN1)}, // Wave 16
    {1, 28, 3, 2, 2, 24, 3, 2250, 3600/28, 30, true, {8, 15, 1, 22} GAITNAME(s_szGN2)}                  // Wave 28?
};

#endif
#endif

//==============================================================================

#ifdef ADD_GAITS
byte NUM_GAITS = sizeof(APG)/sizeof(APG[0]) + sizeof(APG_EXTRA)/sizeof(APG_EXTRA[0]);
#else
byte NUM_GAITS = sizeof(APG)/sizeof(APG[0]);
#endif

//==============================================================================
// Function prototypes
//==============================================================================
extern void GaitSelect(void);
extern void  WriteOutputs(void);
extern void SingleLegControl(void);
extern void GaitSeq(void);
extern void BalanceBody(void);
extern void CheckAngles();

extern void PrintSystemStuff(void);

//extern void  GaitGetNextLeg(byte GaitStep);
extern void BalCalcOneLeg (long PosX, long PosZ, long PosY, byte BalLegNr);
extern void BodyFK (short PosX, short PosZ, short PosY, short RotationY, byte BodyIKLeg) ;
extern void LegIK (short IKFeetPosX, short IKFeetPosY, short IKFeetPosZ, byte LegIKLegNr);
extern void Gait (byte GaitCurrentLegNr);
extern void GetSinCos(short AngleDeg1);
extern short GetATan2 (short AtanX, short AtanY);
extern unsigned long isqrt32 (unsigned long n);

extern void StartUpdateServos(void);
extern boolean TerminalMonitor(void);

//==============================================================================
// SETUP: the main arduino setup function.
//==============================================================================
void setup(){
#ifdef OPT_SKETCHSETUP
  SketchSetup();
#endif
  g_fShowDebugPrompt = true;
  g_fDebugOutput = false;
#ifdef DBGSerial
  DBGSerial.begin(38400);
#endif

  g_ServoDriver.Init();

  //Checks to see if our Servo Driver support a GP Player
  //DBGSerial.write("Program Start\n\r");
  //debug stuff
  delay(10);

  //Turning off all the leds
  LedA = 0;
  LedB = 0;
  LedC = 0;
  Eyes = 0;

  for (LegIndex= 0; LegIndex < CNT_LEGS; LegIndex++ )
  {
    LegPosX[LegIndex] = (short)pgm_read_word(&cInitPosX[LegIndex]);
    LegPosY[LegIndex] = (short)pgm_read_word(&cInitPosY[LegIndex]);
    LegPosZ[LegIndex] = (short)pgm_read_word(&cInitPosZ[LegIndex]);
  }

  ResetLegInitAngles();

  #ifdef OPT_SINGLELEG
  g_InControlState.SelectedLeg = 255;
  PrevSelectedLeg = 255;
#endif
  g_InControlState.BodyPos.x = 0;
  g_InControlState.BodyPos.y = 0;
  g_InControlState.BodyPos.z = 0;

  g_InControlState.BodyRot1.x = 0;
  g_InControlState.BodyRot1.y = 0;
  g_InControlState.BodyRot1.z = 0;
  g_InControlState.BodyRotOffset.x = 0;
  g_InControlState.BodyRotOffset.y = 0;
  g_InControlState.BodyRotOffset.z = 0;

  g_InControlState.GaitType = 0;
  g_InControlState.BalanceMode = 0;
  g_InControlState.LegLiftHeight = 50;
  g_InControlState.ForceGaitStepCnt = 0;
  g_InControlState.GaitStep = 1;
  GaitSelect();

#ifdef cTurretRotPin
  g_InControlState.TurretRotAngle1 = cTurretRotInit;
  g_InControlState.TurretTiltAngle1 = cTurretTiltInit;
#endif

  g_InputController.Init();

  ServoMoveTime = 150;
  g_InControlState.fRobotOn = 0;
  g_fLowVoltageShutdown = false;
#ifdef DEBUG_IOPINS
  //pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
#endif
#ifdef OPT_WALK_UPSIDE_DOWN
  g_fRobotUpsideDown = false;
#ifdef DBGSerial
  DBGSerial.println(IsRobotUpsideDown, DEC);
#endif
#endif

}

//==============================================================================
// Loop: the main arduino main Loop function
//==============================================================================
void loop(void)
{
  unsigned long lTimeWaitEnd;
  lTimerStart = millis();
  DoBackgroundProcess();

  CheckVoltage();
  if (!g_fLowVoltageShutdown) {
    //DebugWrite(A0, HIGH);
    g_InputController.ControlInput();
    //DebugWrite(A0, LOW);
  }
  WriteOutputs();

#ifdef IsRobotUpsideDown
    if (!fWalking){
    g_fRobotUpsideDown = IsRobotUpsideDown;
    if (g_fRobotUpsideDown != fRobotUpsideDownPrev) {
      g_fRobotUpsideDown = IsRobotUpsideDown;
      if (g_fRobotUpsideDown != fRobotUpsideDownPrev) {
        fRobotUpsideDownPrev = g_fRobotUpsideDown;
#ifdef DGBSerial
        DBGSerial.println(fRobotUpsideDownPrev, DEC);
#endif
      }
    }
  }
  //DBGSerial.println(analogRead(0), DEC);
#endif
#ifdef OPT_WALK_UPSIDE_DOWN
  if (g_fRobotUpsideDown){
    g_InControlState.TravelLength.x = -g_InControlState.TravelLength.x;
    g_InControlState.BodyPos.x = -g_InControlState.BodyPos.x;
    g_InControlState.SLLeg.x = -g_InControlState.SLLeg.x;
    g_InControlState.BodyRot1.z = -g_InControlState.BodyRot1.z;
  }
#endif

#ifdef OPT_GPPLAYER
  g_ServoDriver.GPPlayer();
  if (g_ServoDriver.FIsGPSeqActive())
    return;
#endif

  SingleLegControl ();
  DoBackgroundProcess();

  GaitSeq();

  DoBackgroundProcess();

  TotalTransX = 0;
  TotalTransZ = 0;
  TotalTransY = 0;
  TotalXBal1 = 0;
  TotalYBal1 = 0;
  TotalZBal1 = 0;

  if (g_InControlState.BalanceMode) {
#ifdef DEBUG
      if (g_fDebugOutput) {
  TravelRequest = (abs(g_InControlState.TravelLength.x)>cTravelDeadZone) || (abs(g_InControlState.TravelLength.z)>cTravelDeadZone)
    || (abs(g_InControlState.TravelLength.y)>cTravelDeadZone) || (g_InControlState.ForceGaitStepCnt != 0) || fWalking;

        DBGSerial.print("T(");
    DBGSerial.print(fWalking, DEC);
    DBGSerial.print(" ");
        DBGSerial.print(g_InControlState.TravelLength.x,DEC);
        DBGSerial.print(",");
        DBGSerial.print(g_InControlState.TravelLength.y,DEC);
        DBGSerial.print(",");
        DBGSerial.print(g_InControlState.TravelLength.z,DEC);
        DBGSerial.print(")");
      }
#endif
    for (LegIndex = 0; LegIndex < (CNT_LEGS/2); LegIndex++) {

      DoBackgroundProcess();
      BalCalcOneLeg (-LegPosX[LegIndex]+GaitPosX[LegIndex], LegPosZ[LegIndex]+GaitPosZ[LegIndex],
          (LegPosY[LegIndex]-(short)pgm_read_word(&cInitPosY[LegIndex]))+GaitPosY[LegIndex], LegIndex);
    }

    for (LegIndex = (CNT_LEGS/2); LegIndex < CNT_LEGS; LegIndex++) {
      DoBackgroundProcess();
      BalCalcOneLeg(LegPosX[LegIndex]+GaitPosX[LegIndex], LegPosZ[LegIndex]+GaitPosZ[LegIndex],
          (LegPosY[LegIndex]-(short)pgm_read_word(&cInitPosY[LegIndex]))+GaitPosY[LegIndex], LegIndex);
    }
    BalanceBody();
  }

  IKSolution = 0 ;
  IKSolutionWarning = 0;
  IKSolutionError = 0 ;

#ifdef DEBUG
    if (g_fDebugOutput && g_InControlState.fRobotOn) {
        DBGSerial.print(g_InControlState.GaitStep,DEC);
        DBGSerial.print(":");
    }
#endif
//==============================================================================

  for (LegIndex = 0; LegIndex < (CNT_LEGS/2); LegIndex++) {
    DoBackgroundProcess();
    BodyFK(-LegPosX[LegIndex]+g_InControlState.BodyPos.x+GaitPosX[LegIndex] - TotalTransX,
    LegPosZ[LegIndex]+g_InControlState.BodyPos.z+GaitPosZ[LegIndex] - TotalTransZ,
    LegPosY[LegIndex]+g_InControlState.BodyPos.y+GaitPosY[LegIndex] - TotalTransY,
    GaitRotY[LegIndex], LegIndex);

    LegIK (LegPosX[LegIndex]-g_InControlState.BodyPos.x+BodyFKPosX-(GaitPosX[LegIndex] - TotalTransX),
    LegPosY[LegIndex]+g_InControlState.BodyPos.y-BodyFKPosY+GaitPosY[LegIndex] - TotalTransY,
    LegPosZ[LegIndex]+g_InControlState.BodyPos.z-BodyFKPosZ+GaitPosZ[LegIndex] - TotalTransZ, LegIndex);
  }

  for (LegIndex = (CNT_LEGS/2); LegIndex < CNT_LEGS; LegIndex++) {
    DoBackgroundProcess();
    BodyFK(LegPosX[LegIndex]-g_InControlState.BodyPos.x+GaitPosX[LegIndex] - TotalTransX,
    LegPosZ[LegIndex]+g_InControlState.BodyPos.z+GaitPosZ[LegIndex] - TotalTransZ,
    LegPosY[LegIndex]+g_InControlState.BodyPos.y+GaitPosY[LegIndex] - TotalTransY,
    GaitRotY[LegIndex], LegIndex);
    LegIK (LegPosX[LegIndex]+g_InControlState.BodyPos.x-BodyFKPosX+GaitPosX[LegIndex] - TotalTransX,
    LegPosY[LegIndex]+g_InControlState.BodyPos.y-BodyFKPosY+GaitPosY[LegIndex] - TotalTransY,
    LegPosZ[LegIndex]+g_InControlState.BodyPos.z-BodyFKPosZ+GaitPosZ[LegIndex] - TotalTransZ, LegIndex);
  }
#ifdef OPT_WALK_UPSIDE_DOWN
  if (g_fRobotUpsideDown){
    g_InControlState.BodyPos.x = -g_InControlState.BodyPos.x;
    g_InControlState.SLLeg.x = -g_InControlState.SLLeg.x;
    g_InControlState.BodyRot1.z = -g_InControlState.BodyRot1.z;
  }
#endif

  CheckAngles();

  LedC = IKSolutionWarning;
  LedA = IKSolutionError;

  if (g_InControlState.fRobotOn) {
    if (g_InControlState.fRobotOn && !g_InControlState.fPrev_RobotOn) {
      MSound(3, 60, 2000, 80, 2250, 100, 2500);
#ifdef USEXBEE
      XBeePlaySounds(3, 60, 2000, 80, 2250, 100, 2500);
#endif

      Eyes = 1;
    }

    if ((abs(g_InControlState.TravelLength.x)>cTravelDeadZone) || (abs(g_InControlState.TravelLength.z)>cTravelDeadZone) ||
      (abs(g_InControlState.TravelLength.y*2)>cTravelDeadZone)) {
      ServoMoveTime = g_InControlState.gaitCur.NomGaitSpeed + (g_InControlState.InputTimeDelay*2) + g_InControlState.SpeedControl;
      if (g_InControlState.BalanceMode)
        ServoMoveTime = ServoMoveTime + BALANCE_DELAY;
    }
    else
    ServoMoveTime = 200 + g_InControlState.SpeedControl;

    DoBackgroundProcess();
    StartUpdateServos();

//==============================================================================

    for (LegIndex = 0; LegIndex < CNT_LEGS; LegIndex++) {
      if ( (GaitPosX[LegIndex] > cGPlimit) || (GaitPosX[LegIndex] < -cGPlimit)
        || (GaitPosZ[LegIndex] > cGPlimit) || (GaitPosZ[LegIndex] < -cGPlimit)
        || (GaitRotY[LegIndex] > cGPlimit) || (GaitRotY[LegIndex] < -cGPlimit))    {

        bExtraCycle = g_InControlState.gaitCur.NrLiftedPos + 1;
        break;
      }
    }
    if (bExtraCycle>0){
      bExtraCycle--;
      fWalking = !(bExtraCycle==0);

      lTimeWaitEnd = lTimerStart + PrevServoMoveTime;

      DebugWrite(A1, HIGH);
      do {
        DoBackgroundProcess();
      }
      while (millis() < lTimeWaitEnd);
      DebugWrite(A1, LOW);
#ifdef DEBUG_X
      if (g_fDebugOutput) {
        DBGSerial.print("BRX:");
        DBGSerial.print(g_InControlState.BodyRot1.x,DEC);
        DBGSerial.print("W?:");
         DBGSerial.print(fWalking,DEC);
         DBGSerial.print(" GS:");
         DBGSerial.print(g_InControlState.GaitStep,DEC);
         DBGSerial.print(" GPZ:");
         DBGSerial.print(GaitPosZ[cLF],DEC);
         DBGSerial.print(" GPY:");
         DBGSerial.println(GaitPosY[cLF],DEC);
      }
#endif
    }
#ifdef DEBUG_X
    if (g_fDebugOutput) {
      DBGSerial.print("TY:");
      DBGSerial.print(TotalYBal1,DEC);
      DBGSerial.print(" LFZ:");
      DBGSerial.println(LegPosZ[cLF],DEC);
      DBGSerial.flush();
    }
#endif
    DebugToggle(A2);
    g_ServoDriver.CommitServoDriver(ServoMoveTime);
  }
  else {
    if (g_InControlState.fPrev_RobotOn || (AllDown= 0)) {
      ServoMoveTime = 600;
      StartUpdateServos();
      g_ServoDriver.CommitServoDriver(ServoMoveTime);
      MSound(3, 100, 2500, 80, 2250, 60, 2000);
#ifdef USEXBEE
      XBeePlaySounds(3, 100, 2500, 80, 2250, 60, 2000);
#endif
      lTimeWaitEnd = millis() + 600;
      do {
        DoBackgroundProcess();
      }
      while (millis() < lTimeWaitEnd);
      //delay(600);
    }
    else {
      g_ServoDriver.FreeServos();
      Eyes = 0;
    }

    g_ServoDriver.IdleTime();

#ifdef OPT_TERMINAL_MONITOR
    if (TerminalMonitor())
      return;
#endif
    delay(20);
  }

  PrevServoMoveTime = ServoMoveTime;

  if (g_InControlState.fRobotOn)
    g_InControlState.fPrev_RobotOn = 1;
  else
    g_InControlState.fPrev_RobotOn = 0;
}


void StartUpdateServos()
{
  byte    LegIndex;

  g_ServoDriver.BeginServoUpdate();

    for (LegIndex = 0; LegIndex < CNT_LEGS; LegIndex++) {
#ifdef c4DOF
    g_ServoDriver.OutputServoInfoForLeg(LegIndex,
        cCoxaInv[LegIndex]? -CoxaAngle1[LegIndex] : CoxaAngle1[LegIndex],
        cFemurInv[LegIndex]? -FemurAngle1[LegIndex] : FemurAngle1[LegIndex],
        cTibiaInv[LegIndex]? -TibiaAngle1[LegIndex] : TibiaAngle1[LegIndex],
        cTarsInv[LegIndex]? -TarsAngle1[LegIndex] : TarsAngle1[LegIndex]);
#else
    g_ServoDriver.OutputServoInfoForLeg(LegIndex,
        cCoxaInv[LegIndex]? -CoxaAngle1[LegIndex] : CoxaAngle1[LegIndex],
        cFemurInv[LegIndex]? -FemurAngle1[LegIndex] : FemurAngle1[LegIndex],
        cTibiaInv[LegIndex]? -TibiaAngle1[LegIndex] : TibiaAngle1[LegIndex]);
#endif
  }
#ifdef cTurretRotPin
  g_ServoDriver.OutputServoInfoForTurret(g_InControlState.TurretRotAngle1, g_InControlState.TurretTiltAngle1);
#endif
}

//==============================================================================
//[WriteOutputs] Updates the state of the leds
//==============================================================================
void WriteOutputs(void)
{
#ifdef cEyesPin
  digitalWrite(cEyesPin, Eyes);
#endif
}

//==============================================================================
//[CHECK VOLTAGE]
byte s_bLVBeepCnt;
boolean CheckVoltage() {
#ifdef cTurnOffVol

  Voltage = g_ServoDriver.GetBatteryVoltage();

  // BUGBUG:: if voltage is 0 it failed to retrieve don't hang program...
  //    if (!Voltage)
  //      return;

  if (!g_fLowVoltageShutdown) {
    if ((Voltage < cTurnOffVol) || (Voltage >= 1999)) {
#ifdef DBGSerial
      DBGSerial.print("Voltage went low, turn off robot ");
      DBGSerial.println(Voltage, DEC);
#endif
      g_InControlState.BodyPos.x = 0;
      g_InControlState.BodyPos.y = 0;
      g_InControlState.BodyPos.z = 0;
      g_InControlState.BodyRot1.x = 0;
      g_InControlState.BodyRot1.y = 0;
      g_InControlState.BodyRot1.z = 0;
      g_InControlState.TravelLength.x = 0;
      g_InControlState.TravelLength.z = 0;

#ifdef OPT_SINGLELEG
      g_InControlState.TravelLength.y = 0;
      g_InControlState.SelectedLeg = 255;
#endif
      g_fLowVoltageShutdown = 1;
      s_bLVBeepCnt = 0;
      g_InControlState.fRobotOn = false;
    }
#ifdef cTurnOnVol
  }
  else if ((Voltage > cTurnOnVol) && (Voltage < 1999)) {
#ifdef DBGSerial
    DBGSerial.print(F("Voltage restored: "));
    DBGSerial.println(Voltage, DEC);
#endif
    g_fLowVoltageShutdown = 0;

#endif
  }
  else {
    if (s_bLVBeepCnt < 5) {
      s_bLVBeepCnt++;
#ifdef DBGSerial
      DBGSerial.println(Voltage, DEC);
#endif
      MSound( 1, 45, 2000);
    }
    delay(2000);
  }
#endif
  return g_fLowVoltageShutdown;
}

//==============================================================================
//[SINGLE LEG CONTROL]
void SingleLegControl(void)
{
#ifdef OPT_SINGLELEG
  AllDown = (LegPosY[cRF]==(short)pgm_read_word(&cInitPosY[cRF])) &&
    (LegPosY[cRR]==(short)pgm_read_word(&cInitPosY[cRR])) &&
    (LegPosY[cLR]==(short)pgm_read_word(&cInitPosY[cLR])) &&
#ifndef QUADMODE
    (LegPosY[cRM]==(short)pgm_read_word(&cInitPosY[cRM])) &&
    (LegPosY[cLM]==(short)pgm_read_word(&cInitPosY[cLM])) &&
#endif
    (LegPosY[cLF]==(short)pgm_read_word(&cInitPosY[cLF]));

  if (g_InControlState.SelectedLeg<= (CNT_LEGS-1)) {
    if (g_InControlState.SelectedLeg!=PrevSelectedLeg) {
      if (AllDown) {
        LegPosY[g_InControlState.SelectedLeg] = (short)pgm_read_word(&cInitPosY[g_InControlState.SelectedLeg])-20;
        PrevSelectedLeg = g_InControlState.SelectedLeg;
      }
      else {
        LegPosX[PrevSelectedLeg] = (short)pgm_read_word(&cInitPosX[PrevSelectedLeg]);
        LegPosY[PrevSelectedLeg] = (short)pgm_read_word(&cInitPosY[PrevSelectedLeg]);
        LegPosZ[PrevSelectedLeg] = (short)pgm_read_word(&cInitPosZ[PrevSelectedLeg]);
      }
    }
    else if (!g_InControlState.fSLHold) {
      LegPosY[g_InControlState.SelectedLeg] = LegPosY[g_InControlState.SelectedLeg]+g_InControlState.SLLeg.y;
      //LegPosY[g_InControlState.SelectedLeg] = (short)pgm_read_word(&cInitPosY[g_InControlState.SelectedLeg])+g_InControlState.SLLeg.y;
      LegPosX[g_InControlState.SelectedLeg] = (short)pgm_read_word(&cInitPosX[g_InControlState.SelectedLeg])+g_InControlState.SLLeg.x;
      LegPosZ[g_InControlState.SelectedLeg] = (short)pgm_read_word(&cInitPosZ[g_InControlState.SelectedLeg])+g_InControlState.SLLeg.z;   
    }
  }
  else {
    if (!AllDown) {
      for(LegIndex = 0; LegIndex <= (CNT_LEGS-1);LegIndex++) {
        LegPosX[LegIndex] = (short)pgm_read_word(&cInitPosX[LegIndex]);
        LegPosY[LegIndex] = (short)pgm_read_word(&cInitPosY[LegIndex]);
        LegPosZ[LegIndex] = (short)pgm_read_word(&cInitPosZ[LegIndex]);
      }
    }
    if (PrevSelectedLeg!=255)
      PrevSelectedLeg = 255;
  }
#endif
}


void GaitSelect(void)
{
  if (g_InControlState.GaitType < NUM_GAITS) {
#ifdef ADD_GAITS
    if (g_InControlState.GaitType < (sizeof(APG_EXTRA)/sizeof(APG_EXTRA[0])))
        g_InControlState.gaitCur = APG_EXTRA[g_InControlState.GaitType];
    else
        g_InControlState.gaitCur = APG[g_InControlState.GaitType - (sizeof(APG_EXTRA)/sizeof(APG_EXTRA[0]))];
#else
    g_InControlState.gaitCur = APG[g_InControlState.GaitType];
#endif
  }

#ifdef DBGSerial
  if (g_fDebugOutput) {
    DBGSerial.print(g_InControlState.GaitType, DEC);
    DBGSerial.print("    {");
    DBGSerial.print(g_InControlState.gaitCur.NomGaitSpeed, DEC);
    DBGSerial.print(", ");
  DBGSerial.print(g_InControlState.gaitCur.StepsInGait, DEC);
    DBGSerial.print(", ");
  DBGSerial.print(g_InControlState.gaitCur.NrLiftedPos, DEC);
    DBGSerial.print(", ");
  DBGSerial.print(g_InControlState.gaitCur.FrontDownPos, DEC);
    DBGSerial.print(", ");
  DBGSerial.print(g_InControlState.gaitCur.LiftDivFactor, DEC);
    DBGSerial.print(", ");
  DBGSerial.print(g_InControlState.gaitCur.TLDivFactor, DEC);
    DBGSerial.print(", ");
  DBGSerial.print(g_InControlState.gaitCur.HalfLiftHeight, DEC);
    DBGSerial.print(", {");
    for (int il = 0; il < CNT_LEGS; il++) {
        DBGSerial.print(g_InControlState.gaitCur.GaitLegNr[il], DEC);
        if (il < (CNT_LEGS-1))
            DBGSerial.print(", ");
    }
    DBGSerial.println("}}");
  }
#endif

}

//==============================================================================
//[GAIT Sequence]
void GaitSeq(void)
{
  if (fWalking || (g_InControlState.ForceGaitStepCnt != 0))
  TravelRequest = true;
  else {
  TravelRequest = (abs(g_InControlState.TravelLength.x)>cTravelDeadZone)
    || (abs(g_InControlState.TravelLength.z)>cTravelDeadZone)
    || (abs(g_InControlState.TravelLength.y)>cTravelDeadZone) ;

    if (TravelRequest) {
#ifdef QUADCODE
    if (g_InControlState.TravelLength.z < 0)
            g_InControlState.GaitStep = ((g_InControlState.TravelLength.X < 0)? g_InControlState.gaitCur.GaitLegNr[cLR] : g_InControlState.gaitCur.GaitLegNr[cRR]);
    else
            g_InControlState.GaitStep = ((g_InControlState.TravelLength.X < 0)? g_InControlState.gaitCur.GaitLegNr[cLF] : g_InControlState.gaitCur.GaitLegNr[cRF]);
        g_InControlState.GaitStep = ((g_InControlState.GaitStep > g_InControlState.gaitCur.FrontDownPos)? (g_InControlState.GaitStep - g_InControlState.gaitCur.FrontDownPos) : (g_InControlState.GaitStep + g_InControlState.gaitCur.StepsInGait - g_InControlState.gaitCur.FrontDownPos);
#endif
    }
    else {
      g_InControlState.TravelLength.x=0;
      g_InControlState.TravelLength.z=0;
      g_InControlState.TravelLength.y=0;
    }
  }

  for (LegIndex = 0; LegIndex < CNT_LEGS; LegIndex++) {
    Gait(LegIndex);
  }

  g_InControlState.GaitStep++;
  if (g_InControlState.GaitStep>g_InControlState.gaitCur.StepsInGait)
    g_InControlState.GaitStep = 1;

  if (g_InControlState.ForceGaitStepCnt)
    g_InControlState.ForceGaitStepCnt--;
}


//==============================================================================
//[GAIT]
void Gait (byte GaitCurrentLegNr)
{
  short int LegStep = g_InControlState.GaitStep - g_InControlState.gaitCur.GaitLegNr[GaitCurrentLegNr];

  if ((TravelRequest && (g_InControlState.gaitCur.NrLiftedPos&1) &&
    LegStep==0) || (!TravelRequest && LegStep==0 && ((abs(GaitPosX[GaitCurrentLegNr])>2) ||
    (abs(GaitPosZ[GaitCurrentLegNr])>2) || (abs(GaitRotY[GaitCurrentLegNr])>2)))) {
    GaitPosX[GaitCurrentLegNr] = 0;
    GaitPosY[GaitCurrentLegNr] = -g_InControlState.LegLiftHeight;
    GaitPosZ[GaitCurrentLegNr] = 0;
    GaitRotY[GaitCurrentLegNr] = 0;
  }

  else if (((g_InControlState.gaitCur.NrLiftedPos==2 && LegStep==0) || (g_InControlState.gaitCur.NrLiftedPos>=3 &&
    (LegStep==-1 || LegStep==(g_InControlState.gaitCur.StepsInGait-1))))
    && TravelRequest) {
    GaitPosX[GaitCurrentLegNr] = -g_InControlState.TravelLength.x/g_InControlState.gaitCur.LiftDivFactor;
    GaitPosY[GaitCurrentLegNr] = -3*g_InControlState.LegLiftHeight/(3+g_InControlState.gaitCur.HalfLiftHeight);
    GaitPosZ[GaitCurrentLegNr] = -g_InControlState.TravelLength.z/g_InControlState.gaitCur.LiftDivFactor;
    GaitRotY[GaitCurrentLegNr] = -g_InControlState.TravelLength.y/g_InControlState.gaitCur.LiftDivFactor;
  }

  else if ((g_InControlState.gaitCur.NrLiftedPos>=2) && (LegStep==1 || LegStep==-(g_InControlState.gaitCur.StepsInGait-1)) && TravelRequest) {
    GaitPosX[GaitCurrentLegNr] = g_InControlState.TravelLength.x/g_InControlState.gaitCur.LiftDivFactor;
    GaitPosY[GaitCurrentLegNr] = -3*g_InControlState.LegLiftHeight/(3+g_InControlState.gaitCur.HalfLiftHeight);
    GaitPosZ[GaitCurrentLegNr] = g_InControlState.TravelLength.z/g_InControlState.gaitCur.LiftDivFactor;
    GaitRotY[GaitCurrentLegNr] = g_InControlState.TravelLength.y/g_InControlState.gaitCur.LiftDivFactor;
  }

  else if (((g_InControlState.gaitCur.NrLiftedPos==5 && (LegStep==-2 ))) && TravelRequest) {
    GaitPosX[GaitCurrentLegNr] = -g_InControlState.TravelLength.x/2;
    GaitPosY[GaitCurrentLegNr] = -g_InControlState.LegLiftHeight/2;
    GaitPosZ[GaitCurrentLegNr] = -g_InControlState.TravelLength.z/2;
    GaitRotY[GaitCurrentLegNr] = -g_InControlState.TravelLength.y/2;
  }

  else if ((g_InControlState.gaitCur.NrLiftedPos==5) && (LegStep==2 || LegStep==-(g_InControlState.gaitCur.StepsInGait-2)) && TravelRequest) {
    GaitPosX[GaitCurrentLegNr] = g_InControlState.TravelLength.x/2;
    GaitPosY[GaitCurrentLegNr] = -g_InControlState.LegLiftHeight/2;
    GaitPosZ[GaitCurrentLegNr] = g_InControlState.TravelLength.z/2;
    GaitRotY[GaitCurrentLegNr] = g_InControlState.TravelLength.y/2;
  }

  else if ((LegStep==g_InControlState.gaitCur.FrontDownPos || LegStep==-(g_InControlState.gaitCur.StepsInGait-g_InControlState.gaitCur.FrontDownPos)) && GaitPosY[GaitCurrentLegNr]<0) {
    GaitPosX[GaitCurrentLegNr] = g_InControlState.TravelLength.x/2;
    GaitPosZ[GaitCurrentLegNr] = g_InControlState.TravelLength.z/2;
    GaitRotY[GaitCurrentLegNr] = g_InControlState.TravelLength.y/2;
    GaitPosY[GaitCurrentLegNr] = 0;
  }

  else {
    GaitPosX[GaitCurrentLegNr] = GaitPosX[GaitCurrentLegNr] - (g_InControlState.TravelLength.x/(short)g_InControlState.gaitCur.TLDivFactor);
    GaitPosY[GaitCurrentLegNr] = 0;
    GaitPosZ[GaitCurrentLegNr] = GaitPosZ[GaitCurrentLegNr] - (g_InControlState.TravelLength.z/(short)g_InControlState.gaitCur.TLDivFactor);
    GaitRotY[GaitCurrentLegNr] = GaitRotY[GaitCurrentLegNr] - (g_InControlState.TravelLength.y/(short)g_InControlState.gaitCur.TLDivFactor);
  }
}

//==============================================================================
//[BalCalcOneLeg]
void BalCalcOneLeg (long PosX, long PosZ, long PosY, byte BalLegNr)
{
  long            CPR_X;
  long            CPR_Y;
  long            CPR_Z;

#ifdef QUADMODE
  if (g_InControlState.gaitCur.COGAngleStep1 == 0) {
#endif
      long             lAtan;
      CPR_Z = (short)pgm_read_word(&cOffsetZ[BalLegNr]) + PosZ;
      CPR_X = (short)pgm_read_word(&cOffsetX[BalLegNr]) + PosX;
      CPR_Y = 150 + PosY;

      TotalTransY += (long)PosY;
      TotalTransZ += (long)CPR_Z;
      TotalTransX += (long)CPR_X;

      lAtan = GetATan2(CPR_X, CPR_Z);
      TotalYBal1 += (lAtan*1800) / 31415;
#ifdef DEBUG
      if (g_fDebugOutput) {
          DBGSerial.print(" ");
          DBGSerial.print(CPR_X, DEC);
          DBGSerial.print(":");
          DBGSerial.print(CPR_Y, DEC);
          DBGSerial.print(":");
          DBGSerial.print(CPR_Z, DEC);
          DBGSerial.print(":");
          DBGSerial.print(TotalYBal1, DEC);
      }
#endif

      lAtan = GetATan2 (CPR_X, CPR_Y);
      TotalZBal1 += ((lAtan*1800) / 31415) -900;

      lAtan = GetATan2 (CPR_Z, CPR_Y);
      TotalXBal1 += ((lAtan*1800) / 31415) - 900;

#ifdef QUADMODE
    }
#endif

}
//==============================================================================
//[BalanceBody]
void BalanceBody(void)
{
#ifdef QUADMODE
  if (g_InControlState.gaitCur.COGAngleStep1 == 0) {
#endif
      TotalTransZ = TotalTransZ/BalanceDivFactor ;
      TotalTransX = TotalTransX/BalanceDivFactor;
      TotalTransY = TotalTransY/BalanceDivFactor;

#ifndef QUADMODE
      if (TotalYBal1 > 0)
        TotalYBal1 -=  1800;
      else
        TotalYBal1 += 1800;
#endif

      if (TotalZBal1 < -1800)
        TotalZBal1 += 3600;

      if (TotalXBal1 < -1800)
        TotalXBal1 += 3600;

      TotalYBal1 = -TotalYBal1/BalanceDivFactor;
      TotalXBal1 = -TotalXBal1/BalanceDivFactor;
      TotalZBal1 = TotalZBal1/BalanceDivFactor;
#ifdef DEBUG
      if (g_fDebugOutput) {
          DBGSerial.print(" L ");
          DBGSerial.print(BalanceDivFactor, DEC);
          DBGSerial.print(" TTrans: ");
          DBGSerial.print(TotalTransX, DEC);
          DBGSerial.print(" ");
          DBGSerial.print(TotalTransY, DEC);
          DBGSerial.print(" ");
          DBGSerial.print(TotalTransZ, DEC);
          DBGSerial.print(" TBal: ");
          DBGSerial.print(TotalXBal1, DEC);
          DBGSerial.print(" ");
          DBGSerial.print(TotalYBal1, DEC);
          DBGSerial.print(" ");
          DBGSerial.println(TotalZBal1, DEC);
      }
#endif
#ifdef QUADMODE
  } else {
      byte COGShiftNeeded;
      byte BalCOGTransX;
      byte BalCOGTransZ;
      word COGAngle1;
      long BalTotTravelLength;

      COGShiftNeeded = TravelRequest;
      for (LegIndex = 0; LegIndex <= CNT_LEGS; LegIndex++)
      {
        COGShiftNeeded = COGShiftNeeded || (abs(GaitPosX[LegIndex])>2) || (abs(GaitPosZ[LegIndex])>2) || (abs(GaitRotY[LegIndex])>2);
      }

      if (COGShiftNeeded) {
        if (g_InControlState.gaitCur.COGCCW) {
          COGAngle1 = g_InControlState.gaitCur.COGAngleStart1 - (g_InControlState.GaitStep-1) * g_InControlState.gaitCur.COGAngleStep1;
        } else {
          COGAngle1 = g_InControlState.gaitCur.COGAngleStart1 + (g_InControlState.GaitStep-1) * g_InControlState.gaitCur.COGAngleStep1;
        }
        GetSinCos(COGAngle1);
        TotalTransX = (long)g_InControlState.gaitCur.COGRadius * (long)sin4 / c4DEC;
        TotalTransZ = (long)g_InControlState.gaitCur.COGRadius * (long)cos4 / c4DEC;

#ifdef DEBUG
        if (g_fDebugOutput) {
          DBGSerial.print(" TotalTransX: ");
          DBGSerial.print(TotalTransX, DEC);
          DBGSerial.print(" TotalTransZ: ");
          DBGSerial.print(TotalTransZ, DEC);
        }
#endif
        if (((abs(g_InControlState.TravelLength.x)>cTravelDeadZone) || (abs(g_InControlState.TravelLength.z)>cTravelDeadZone)) && (abs(g_InControlState.TravelLength.y)<=cTravelDeadZone) ) {
        //if(TravelRotationY = 0) then

          BalTotTravelLength = isqrt32(abs(g_InControlState.TravelLength.x * g_InControlState.TravelLength.x) + abs(g_InControlState.TravelLength.z*g_InControlState.TravelLength.z));
          BalCOGTransX = abs(g_InControlState.TravelLength.z)*c2DEC/BalTotTravelLength;
          BalCOGTransZ = abs(g_InControlState.TravelLength.x)*c2DEC/BalTotTravelLength;
          TotalTransX = TotalTransX*BalCOGTransX/c2DEC;
          TotalTransZ = TotalTransZ*BalCOGTransZ/c2DEC;
        }

#ifdef DEBUG
        if (g_fDebugOutput) {
          DBGSerial.print(" COGRadius: ");
          DBGSerial.print(g_InControlState.gaitCur.COGRadius, DEC);
          DBGSerial.print(" TotalTransX: ");
          DBGSerial.print(TotalTransX, DEC);
          DBGSerial.print(" TotalTransZ: ");
          DBGSerial.println(TotalTransZ, DEC);
        }
#endif
      }
  }
#endif
}

//==============================================================================
//[GETSINCOS] Get the sinus and cosinus from the angle +/- multiple circles
void GetSinCos(short AngleDeg1)
{
  short        ABSAngleDeg1;

  if (AngleDeg1 < 0)
    ABSAngleDeg1 = AngleDeg1 *-1;
  else
    ABSAngleDeg1 = AngleDeg1;

  if (AngleDeg1 < 0)
    AngleDeg1 = 3600-(ABSAngleDeg1-(3600*(ABSAngleDeg1/3600)));
  else
  AngleDeg1 = ABSAngleDeg1-(3600*(ABSAngleDeg1/3600));

  if (AngleDeg1>=0 && AngleDeg1<=900)
  {
    sin4 = pgm_read_word(&GetSin[AngleDeg1/5]);
    cos4 = pgm_read_word(&GetSin[(900-(AngleDeg1))/5]);
  }

  else if (AngleDeg1>900 && AngleDeg1<=1800)
  {
    sin4 = pgm_read_word(&GetSin[(900-(AngleDeg1-900))/5]);
    cos4 = -pgm_read_word(&GetSin[(AngleDeg1-900)/5]);
  }
  else if (AngleDeg1>1800 && AngleDeg1<=2700)
  {
    sin4 = -pgm_read_word(&GetSin[(AngleDeg1-1800)/5]);
    cos4 = -pgm_read_word(&GetSin[(2700-AngleDeg1)/5]);
  }
  else if(AngleDeg1>2700 && AngleDeg1<=3600)
  {
    sin4 = -pgm_read_word(&GetSin[(3600-AngleDeg1)/5]);
    cos4 = pgm_read_word(&GetSin[(AngleDeg1-2700)/5]);
  }
}

//==============================================================================
//(GETARCCOS) Get the sinus and cosinus from the angle +/- multiple circles
long GetArcCos(short cos4)
{
  boolean NegativeValue/*:1*/;
  if (cos4<0)
  {
    cos4 = -cos4;
    NegativeValue = 1;
  }
  else
    NegativeValue = 0;

  cos4 = min(cos4,c4DEC);

  if ((cos4>=0) && (cos4<9000))
  {
    AngleRad4 = (byte)pgm_read_byte(&GetACos[cos4/79]);
    AngleRad4 = ((long)AngleRad4*616)/c1DEC;
  }
  else if ((cos4>=9000) && (cos4<9900))
  {
    AngleRad4 = (byte)pgm_read_byte(&GetACos[(cos4-9000)/8+114]);
    AngleRad4 = (long)((long)AngleRad4*616)/c1DEC;
  }
  else if ((cos4>=9900) && (cos4<=10000))
  {
    AngleRad4 = (byte)pgm_read_byte(&GetACos[(cos4-9900)/2+227]);
    AngleRad4 = (long)((long)AngleRad4*616)/c1DEC;
  }

  if (NegativeValue)
    AngleRad4 = 31416 - AngleRad4;

  return AngleRad4;
}

unsigned long isqrt32 (unsigned long n)
{
  unsigned long root;
  unsigned long remainder;
  unsigned long  place;

  root = 0;
  remainder = n;
  place = 0x40000000;

  while (place > remainder)
    place = place >> 2;
  while (place)
  {
    if (remainder >= root + place)
    {
      remainder = remainder - root - place;
      root = root + (place << 1);
    }
    root = root >> 1;
    place = place >> 2;
  }
  return root;
}

//==============================================================================
//(GETATAN2) Simplyfied ArcTan2 function based on fixed point ArcCos
short GetATan2 (short AtanX, short AtanY)
{
  XYhyp2 = isqrt32(((long)AtanX*AtanX*c4DEC) + ((long)AtanY*AtanY*c4DEC));
  GetArcCos (((long)AtanX*(long)c6DEC) /(long) XYhyp2);

  if (AtanY < 0)
    Atan4 = -AngleRad4;
  else
    Atan4 = AngleRad4;
  return Atan4;
}

//==============================================================================
//(BODY INVERSE KINEMATICS)
//==============================================================================
void BodyFK (short PosX, short PosZ, short PosY, short RotationY, byte BodyIKLeg)
{
  short            SinA4;
  short            CosA4;
  short            SinB4;
  short            CosB4;
  short            SinG4;
  short            CosG4;
  short            CPR_X;
  short            CPR_Y;
  short            CPR_Z;

  CPR_X = (short)pgm_read_word(&cOffsetX[BodyIKLeg])+PosX + g_InControlState.BodyRotOffset.x;
  CPR_Y = PosY + g_InControlState.BodyRotOffset.y;
  CPR_Z = (short)pgm_read_word(&cOffsetZ[BodyIKLeg]) + PosZ + g_InControlState.BodyRotOffset.z;

  GetSinCos (g_InControlState.BodyRot1.x+TotalXBal1);
  SinG4 = sin4;
  CosG4 = cos4;

  GetSinCos (g_InControlState.BodyRot1.z+TotalZBal1);
  SinB4 = sin4;
  CosB4 = cos4;

#ifdef OPT_WALK_UPSIDE_DOWN
  if (g_fRobotUpsideDown)
    GetSinCos (-g_InControlState.BodyRot1.y+(-RotationY*c1DEC)+TotalYBal1) ;
  else
    GetSinCos (g_InControlState.BodyRot1.y+(RotationY*c1DEC)+TotalYBal1) ;
#else
  GetSinCos (g_InControlState.BodyRot1.y+(RotationY*c1DEC)+TotalYBal1) ;
#endif
  SinA4 = sin4;
  CosA4 = cos4;

  BodyFKPosX = ((long)CPR_X*c2DEC - ((long)CPR_X*c2DEC*CosA4/c4DEC*CosB4/c4DEC - (long)CPR_Z*c2DEC*CosB4/c4DEC*SinA4/c4DEC
    + (long)CPR_Y*c2DEC*SinB4/c4DEC ))/c2DEC;
  BodyFKPosZ = ((long)CPR_Z*c2DEC - ( (long)CPR_X*c2DEC*CosG4/c4DEC*SinA4/c4DEC + (long)CPR_X*c2DEC*CosA4/c4DEC*SinB4/c4DEC*SinG4/c4DEC
    + (long)CPR_Z*c2DEC*CosA4/c4DEC*CosG4/c4DEC - (long)CPR_Z*c2DEC*SinA4/c4DEC*SinB4/c4DEC*SinG4/c4DEC
    - (long)CPR_Y*c2DEC*CosB4/c4DEC*SinG4/c4DEC ))/c2DEC;
  BodyFKPosY = ((long)CPR_Y  *c2DEC - ( (long)CPR_X*c2DEC*SinA4/c4DEC*SinG4/c4DEC - (long)CPR_X*c2DEC*CosA4/c4DEC*CosG4/c4DEC*SinB4/c4DEC
    + (long)CPR_Z*c2DEC*CosA4/c4DEC*SinG4/c4DEC + (long)CPR_Z*c2DEC*CosG4/c4DEC*SinA4/c4DEC*SinB4/c4DEC
    + (long)CPR_Y*c2DEC*CosB4/c4DEC*CosG4/c4DEC ))/c2DEC;
}

//==============================================================================
//[LEG INVERSE KINEMATICS] Calculates the angles of the coxa, femur and tibia for the given position of the feet
//==============================================================================
void LegIK (short IKFeetPosX, short IKFeetPosY, short IKFeetPosZ, byte LegIKLegNr)
{
  unsigned long    IKSW2;
  unsigned long    IKA14;
  unsigned long    IKA24;
  short            IKFeetPosXZ;
#ifdef c4DOF
  long            TarsOffsetXZ;
  long            TarsOffsetY;
  long            TarsToGroundAngle1;
  long            TGA_A_H4;
  long            TGA_B_H3;
#else
#define TarsOffsetXZ 0
#define TarsOffsetY  0
#endif

  long            Temp1;
  long            Temp2;
  long            T3;

  GetATan2 (IKFeetPosX, IKFeetPosZ);
  CoxaAngle1[LegIKLegNr] = (((long)Atan4*180) / 3141) + (short)pgm_read_word(&cCoxaAngle1[LegIKLegNr]);

  IKFeetPosXZ = XYhyp2/c2DEC;
#ifdef c4DOF
  if ((byte)pgm_read_byte(&cTarsLength[LegIKLegNr])) {
    TarsToGroundAngle1 = -cTarsConst + cTarsMulti*IKFeetPosY + ((long)(IKFeetPosXZ*cTarsFactorA))/c1DEC - ((long)(IKFeetPosXZ*IKFeetPosY)/(cTarsFactorB));
    if (IKFeetPosY < 0)
      TarsToGroundAngle1 = TarsToGroundAngle1 - ((long)(IKFeetPosY*cTarsFactorC)/c1DEC);
    if (TarsToGroundAngle1 > 400)
      TGA_B_H3 = 200 + (TarsToGroundAngle1/2);
    else
      TGA_B_H3 = TarsToGroundAngle1;

    if (TarsToGroundAngle1 > 300)
      TGA_A_H4 = 240 + (TarsToGroundAngle1/5);
    else
      TGA_A_H4 = TarsToGroundAngle1;

    if (IKFeetPosY > 0)
      TarsToGroundAngle1 = TGA_A_H4;
    else if (((IKFeetPosY <= 0) & (IKFeetPosY > -10)))
      TarsToGroundAngle1 = (TGA_A_H4 -(((long)IKFeetPosY*(TGA_B_H3-TGA_A_H4))/c1DEC));
    else
    TarsToGroundAngle1 = TGA_B_H3;

    GetSinCos(TarsToGroundAngle1);
    TarsOffsetXZ = ((long)sin4*(byte)pgm_read_byte(&cTarsLength[LegIKLegNr]))/c4DEC;
    TarsOffsetY = ((long)cos4*(byte)pgm_read_byte(&cTarsLength[LegIKLegNr]))/c4DEC;
  }
  else {
    TarsOffsetXZ = 0;
    TarsOffsetY = 0;
  }
#endif

  IKA14 = GetATan2 (IKFeetPosY-TarsOffsetY, IKFeetPosXZ-(byte)pgm_read_byte(&cCoxaLength[LegIKLegNr])-TarsOffsetXZ);

  IKSW2 = XYhyp2;

  Temp1 = ((((long)(byte)pgm_read_byte(&cFemurLength[LegIKLegNr])*(byte)pgm_read_byte(&cFemurLength[LegIKLegNr])) - ((long)(byte)pgm_read_byte(&cTibiaLength[LegIKLegNr])*(byte)pgm_read_byte(&cTibiaLength[LegIKLegNr])))*c4DEC + ((long)IKSW2*IKSW2));
  Temp2 = (long)(2*(byte)pgm_read_byte(&cFemurLength[LegIKLegNr]))*c2DEC * (unsigned long)IKSW2;
  T3 = Temp1 / (Temp2/c4DEC);
  IKA24 = GetArcCos (T3 );
#ifdef DEBUG_IK
    if (g_fDebugOutput && g_InControlState.fRobotOn) {
        DBGSerial.print(" ");
        DBGSerial.print(Temp1, DEC);
        DBGSerial.print(" ");
        DBGSerial.print(Temp2, DEC);
        DBGSerial.print(" ");
        DBGSerial.print(T3, DEC);
        DBGSerial.print(" ");
        DBGSerial.print(IKSW2, DEC);
        DBGSerial.print(" ");
        DBGSerial.print(IKA14, DEC);
        DBGSerial.print(" ");
        DBGSerial.print(IKA24, DEC);
    }
#endif

#ifdef OPT_WALK_UPSIDE_DOWN
  if (g_fRobotUpsideDown)
    FemurAngle1[LegIKLegNr] = (long)(IKA14 + IKA24) * 180 / 3141 - 900 + CFEMURHORNOFFSET1(LegIKLegNr);
  else
    FemurAngle1[LegIKLegNr] = -(long)(IKA14 + IKA24) * 180 / 3141 + 900 + CFEMURHORNOFFSET1(LegIKLegNr);
#else
  FemurAngle1[LegIKLegNr] = -(long)(IKA14 + IKA24) * 180 / 3141 + 900 + CFEMURHORNOFFSET1(LegIKLegNr);
#endif

  Temp1 = ((((long)(byte)pgm_read_byte(&cFemurLength[LegIKLegNr])*(byte)pgm_read_byte(&cFemurLength[LegIKLegNr])) + ((long)(byte)pgm_read_byte(&cTibiaLength[LegIKLegNr])*(byte)pgm_read_byte(&cTibiaLength[LegIKLegNr])))*c4DEC - ((long)IKSW2*IKSW2));
  Temp2 = 2 * ((long)((byte)pgm_read_byte(&cFemurLength[LegIKLegNr]))) * (long)((byte)pgm_read_byte(&cTibiaLength[LegIKLegNr]));
  GetArcCos (Temp1 / Temp2);
#ifdef DEBUG_IK
    if (g_fDebugOutput && g_InControlState.fRobotOn) {
        DBGSerial.print("=");
        DBGSerial.print(Temp1, DEC);
        DBGSerial.print(" ");
        DBGSerial.print(Temp2, DEC);
        DBGSerial.print(" ");
        DBGSerial.print(AngleRad4, DEC);
    }
#endif

#ifdef OPT_WALK_UPSIDE_DOWN
  if (g_fRobotUpsideDown)
    TibiaAngle1[LegIKLegNr] = (1800-(long)AngleRad4*180/3141 + CTIBIAHORNOFFSET1(LegIKLegNr));
  else
    TibiaAngle1[LegIKLegNr] = -(1800-(long)AngleRad4*180/3141 + CTIBIAHORNOFFSET1(LegIKLegNr));
#else
#ifdef PHANTOMX_V2
    TibiaAngle1[LegIKLegNr] = -(1450-(long)AngleRad4*180/3141 + CTIBIAHORNOFFSET1(LegIKLegNr));
#else
    TibiaAngle1[LegIKLegNr] = -(900-(long)AngleRad4*180/3141 + CTIBIAHORNOFFSET1(LegIKLegNr));
#endif
#endif

#ifdef c4DOF
  if ((byte)pgm_read_byte(&cTarsLength[LegIKLegNr])) {
    TarsAngle1[LegIKLegNr] = (TarsToGroundAngle1 + FemurAngle1[LegIKLegNr] - TibiaAngle1[LegIKLegNr])
      + CTARSHORNOFFSET1(LegIKLegNr);
  }
#endif

  if(IKSW2 < ((word)((byte)pgm_read_byte(&cFemurLength[LegIKLegNr])+(byte)pgm_read_byte(&cTibiaLength[LegIKLegNr])-30)*c2DEC))
    IKSolution = 1;
  else
  {
    if(IKSW2 < ((word)((byte)pgm_read_byte(&cFemurLength[LegIKLegNr])+(byte)pgm_read_byte(&cTibiaLength[LegIKLegNr]))*c2DEC))
      IKSolutionWarning = 1;
    else
      IKSolutionError = 1    ;
  }
#ifdef DEBUG
    if (g_fDebugOutput && g_InControlState.fRobotOn) {
        DBGSerial.print("(");
        DBGSerial.print(IKFeetPosX, DEC);
        DBGSerial.print(",");
        DBGSerial.print(IKFeetPosY, DEC);
        DBGSerial.print(",");
        DBGSerial.print(IKFeetPosZ, DEC);
        DBGSerial.print(")=<");
        DBGSerial.print(CoxaAngle1[LegIKLegNr], DEC);
        DBGSerial.print(",");
        DBGSerial.print(FemurAngle1[LegIKLegNr], DEC);
        DBGSerial.print(",");
        DBGSerial.print(TibiaAngle1[LegIKLegNr], DEC);
        DBGSerial.print(">");
        DBGSerial.print((IKSolutionError<<2)+(IKSolutionWarning<<1)+IKSolution, DEC);
        if (LegIKLegNr == (CNT_LEGS-1))
            DBGSerial.println();
    }
#endif
}

//==============================================================================
//[CHECK ANGLES] Checks the mechanical limits of the servos
//==============================================================================
short CheckServoAngleBounds(short sID,  short sVal, const short *sMin PROGMEM, const short *sMax PROGMEM) {
    short s = (short)pgm_read_word(sMin);
    if (sVal < s) {
#ifdef DEBUG_BOUNDS
      if (g_fDebugOutput) {
        DBGSerial.print(sID, DEC);
        DBGSerial.print(" ");
        DBGSerial.print(sVal, DEC);
        DBGSerial.print("<");
        DBGSerial.println(s, DEC);
      }
#endif
        return s;
    }

    s = (short)pgm_read_word(sMax);
    if (sVal > s) {
#ifdef DEBUG_BOUNDS
      if (g_fDebugOutput) {
        DBGSerial.print(sID, DEC);
        DBGSerial.print(" ");
        DBGSerial.print(sVal, DEC);
        DBGSerial.print(">");
        DBGSerial.println(s, DEC);
      }
#endif
        return s;
    }
    return sVal;
}

//==============================================================================
//[CHECK ANGLES] Checks the mechanical limits of the servos
//==============================================================================
void CheckAngles(void)
{
#ifndef SERVOS_DO_MINMAX
  short s = 0;
  for (LegIndex = 0; LegIndex < CNT_LEGS; LegIndex++)
  {
    CoxaAngle1[LegIndex]  = CheckServoAngleBounds(s++, CoxaAngle1[LegIndex], &cCoxaMin1[LegIndex], &cCoxaMax1[LegIndex]);
    FemurAngle1[LegIndex] = CheckServoAngleBounds(s++, FemurAngle1[LegIndex], &cFemurMin1[LegIndex], &cFemurMax1[LegIndex]);
    TibiaAngle1[LegIndex] = CheckServoAngleBounds(s++, TibiaAngle1[LegIndex], &cTibiaMin1[LegIndex], &cTibiaMax1[LegIndex]);
#ifdef c4DOF
    if ((byte)pgm_read_byte(&cTarsLength[LegIndex])) {
      TarsAngle1[LegIndex] = CheckServoAngleBounds(s++, TarsAngle1[LegIndex], &cTarsMin1[LegIndex], &cTarsMax1[LegIndex]);
    }
#endif
  }
#endif
}

//==============================================================================
// SmoothControl (From Zenta) -  This function makes the body
//            rotation and translation much smoother
//==============================================================================
short SmoothControl (short CtrlMoveInp, short CtrlMoveOut, byte CtrlDivider)
{
  if (CtrlMoveOut < (CtrlMoveInp - 4))
    return CtrlMoveOut + abs((CtrlMoveOut - CtrlMoveInp)/CtrlDivider);
  else if (CtrlMoveOut > (CtrlMoveInp + 4))
    return CtrlMoveOut - abs((CtrlMoveOut - CtrlMoveInp)/CtrlDivider);

  return CtrlMoveInp;
}

//==============================================================================
// GetLegsXZLength -
//==============================================================================
word g_wLegsXZLength = 0xffff;
word GetLegsXZLength(void)
{
    // Could save away or could do a little math on one leg...
    if (g_wLegsXZLength != 0xffff)
        return g_wLegsXZLength;

    return isqrt32((LegPosX[0] * LegPosX[0]) + (LegPosZ[0] * LegPosZ[0]));
}

//==============================================================================
// AdjustLegPositions() - Will adjust the init leg positions to the width passed in.
//==============================================================================
#ifndef MIN_XZ_LEG_ADJUST
#define MIN_XZ_LEG_ADJUST (cCoxaLength[0])
#endif

#ifndef MAX_XZ_LEG_ADJUST
#define MAX_XZ_LEG_ADJUST   (cCoxaLength[0]+cTibiaLength[0] + cFemurLength[0]/4)
#endif

void AdjustLegPositions(word XZLength1)
{
    if (XZLength1 > MAX_XZ_LEG_ADJUST)
        XZLength1 = MAX_XZ_LEG_ADJUST;
    if (XZLength1 < MIN_XZ_LEG_ADJUST)
        XZLength1 = MIN_XZ_LEG_ADJUST;

    if (XZLength1 == g_wLegsXZLength)
        return;

    g_wLegsXZLength = XZLength1;


    for (uint8_t LegIndex = 0; LegIndex < CNT_LEGS; LegIndex++) {
#ifdef DEBUG
      if (g_fDebugOutput) {
        DBGSerial.print("(");
        DBGSerial.print(LegPosX[LegIndex], DEC);
        DBGSerial.print(",");
        DBGSerial.print(LegPosZ[LegIndex], DEC);
        DBGSerial.print(")->");
      }
#endif
#ifdef OPT_DYNAMIC_ADJUST_LEGS
      GetSinCos(g_InControlState.aCoxaInitAngle1[LegIndex]);
#else
#ifdef cRRInitCoxaAngle1
      GetSinCos((short)pgm_read_word(&cCoxaInitAngle1[LegIndex]));
#else
      GetSinCos((short)pgm_read_word(&cCoxaAngle1[LegIndex]));
#endif
#endif
      LegPosX[LegIndex] = ((long)((long)cos4 * XZLength1))/c4DEC;
      LegPosZ[LegIndex] = -((long)((long)sin4 * XZLength1))/c4DEC;
#ifdef DEBUG
      if (g_fDebugOutput) {
        DBGSerial.print("(");
        DBGSerial.print(LegPosX[LegIndex], DEC);
        DBGSerial.print(",");
        DBGSerial.print(LegPosZ[LegIndex], DEC);
        DBGSerial.print(") ");
      }
#endif
    }
#ifdef DEBUG
    if (g_fDebugOutput) {
      DBGSerial.println("");
    }
#endif
    g_InControlState.ForceGaitStepCnt = g_InControlState.gaitCur.StepsInGait;
}

//==============================================================================
// ResetLegInitAngles - This is used when we allow the user to
// adjust the leg position angles.  This resets to what it was when the
// the program was started.
//==============================================================================
void ResetLegInitAngles(void)
{
#ifdef OPT_DYNAMIC_ADJUST_LEGS
    for (int LegIndex=0; LegIndex < CNT_LEGS; LegIndex++) {
#ifdef cRRInitCoxaAngle1
            g_InControlState.aCoxaInitAngle1[LegIndex] = (short)pgm_read_word(&cCoxaInitAngle1[LegIndex]);
#else
            g_InControlState.aCoxaInitAngle1[LegIndex] = (short)pgm_read_word(&cCoxaAngle1[LegIndex]);
#endif
    }
    g_wLegsXZLength = 0xffff;
#endif
}

//==============================================================================
// ResetLegInitAngles - This is used when we allow the user to
//==============================================================================
void RotateLegInitAngles (int iDeltaAngle)
{
#ifdef OPT_DYNAMIC_ADJUST_LEGS
    for (int LegIndex=0; LegIndex < CNT_LEGS; LegIndex++) {
        if ((short)pgm_read_word(&cCoxaAngle1[LegIndex]) > 0)
            g_InControlState.aCoxaInitAngle1[LegIndex] += iDeltaAngle;
         else if ((short)pgm_read_word(&cCoxaAngle1[LegIndex]) < 0)
            g_InControlState.aCoxaInitAngle1[LegIndex] -= iDeltaAngle;
        if (g_InControlState.aCoxaInitAngle1[LegIndex] > 700)
            g_InControlState.aCoxaInitAngle1[LegIndex] = 700;
        else if (g_InControlState.aCoxaInitAngle1[LegIndex] < -700)
            g_InControlState.aCoxaInitAngle1[LegIndex] = -700;
    }
    g_wLegsXZLength = 0xffff;
#endif
}

//==============================================================================
// AdjustLegPositionsToBodyHeight() - Will try to adjust the position of the legs
// to be appropriate for the current y location of the body...
//==============================================================================
uint8_t g_iLegInitIndex = 0x00;

void AdjustLegPositionsToBodyHeight()
{
#ifdef CNT_HEX_INITS
  if (g_InControlState.BodyPos.y > (short)pgm_read_byte(&g_abHexMaxBodyY[CNT_HEX_INITS-1]))
    g_InControlState.BodyPos.y =  (short)pgm_read_byte(&g_abHexMaxBodyY[CNT_HEX_INITS-1]);

  uint8_t i;
  word XZLength1 = pgm_read_byte(&g_abHexIntXZ[CNT_HEX_INITS-1]);
  for(i = 0; i < (CNT_HEX_INITS-1); i++) {
    if (g_InControlState.BodyPos.y <= (short)pgm_read_byte(&g_abHexMaxBodyY[i])) {
      XZLength1 = pgm_read_byte(&g_abHexIntXZ[i]);
      break;
    }
  }
  if (i != g_iLegInitIndex) {
    g_iLegInitIndex = i;

#ifdef DEBUG
    if (g_fDebugOutput) {
        DBGSerial.print("ALPTBH: ");
        DBGSerial.print(g_InControlState.BodyPos.y, DEC);
        DBGSerial.print(" ");
        DBGSerial.print(XZLength1, DEC);
    }
#endif
    AdjustLegPositions(XZLength1);
  }
#endif
}

//==============================================================================
// SoundNoTimer - Quick and dirty tone function to try to output a frequency
// to a speaker for some simple sounds.
//==============================================================================
#ifdef SOUND_PIN
void SoundNoTimer(unsigned long duration,  unsigned int frequency)
{
#ifndef __MK20DX256__
#ifdef __AVR__
  volatile uint8_t *pin_port;
  volatile uint8_t pin_mask;
#else
  volatile uint32_t *pin_port;
  volatile uint16_t pin_mask;
#endif
  long toggle_count = 0;
  long lusDelayPerHalfCycle;

  pinMode(SOUND_PIN, OUTPUT);

  pin_port = portOutputRegister(digitalPinToPort(SOUND_PIN));
  pin_mask = digitalPinToBitMask(SOUND_PIN);

  toggle_count = 2 * frequency * duration / 1000;
  lusDelayPerHalfCycle = 1000000L/(frequency * 2);

  while (toggle_count--) {
    *pin_port ^= pin_mask;

    delayMicroseconds(lusDelayPerHalfCycle);
  }
  *pin_port &= ~(pin_mask);
#else
  long toggle_count = 0;
  long lusDelayPerHalfCycle;
  boolean fHigh = false;
  pinMode(SOUND_PIN, OUTPUT);
  digitalWrite(SOUND_PIN, LOW);
  toggle_count = 2 * frequency * duration / 1000;
  lusDelayPerHalfCycle = 1000000L/(frequency * 2);
  while (toggle_count--) {
    fHigh  = !fHigh;
    digitalWrite(SOUND_PIN, fHigh? LOW : HIGH);
    delayMicroseconds(lusDelayPerHalfCycle);
  }
  digitalWrite(SOUND_PIN, LOW);
#endif
}

void MSound(byte cNotes, ...)
{
  va_list ap;
  unsigned int uDur;
  unsigned int uFreq;
  va_start(ap, cNotes);

  while (cNotes > 0) {
    uDur = va_arg(ap, unsigned int);
    uFreq = va_arg(ap, unsigned int);
    SoundNoTimer(uDur, uFreq);
    cNotes--;
  }
  va_end(ap);
}
#else
void MSound(byte cNotes, ...)
{
};
#endif

#ifdef OPT_TERMINAL_MONITOR
#ifdef OPT_DUMP_EEPROM
extern void DumpEEPROMCmd(byte *pszCmdLine);
#endif
#ifdef QUADMODE
extern void UpdateGaitCmd(byte *pszCmdLine);
#endif
#ifdef OPT_DYNAMIC_ADJUST_LEGS
extern void UpdateInitialPosAndAngCmd(byte *pszCmdLine);
#endif

//==============================================================================
// TerminalMonitor - Simple background task checks to see if the user is asking
// us to do anything, like update debug levels ore the like.
//==============================================================================
boolean TerminalMonitor(void)
{
  byte szCmdLine[20];
  byte ich;
  int ch;
  if (g_fShowDebugPrompt) {
    DBGSerial.println(F("Arduino Phoenix Monitor"));
    DBGSerial.println(F("D - Toggle debug on or off"));
#ifdef OPT_DUMP_EEPROM
    DBGSerial.println(F("E - Dump EEPROM"));
#endif
#ifdef QUADMODE
//DBGSerial.println(F("B <percent>"));
  DBGSerial.println(F("G ST NL RR RF LR LF"));
#endif
#ifdef OPT_DYNAMIC_ADJUST_LEGS
    DBGSerial.println(F("I pos ang"));
#endif
#ifdef OPT_TERMINAL_MONITOR_IC
    g_InputController.ShowTerminalCommandList();
#endif

    g_ServoDriver.ShowTerminalCommandList();
    g_fShowDebugPrompt = false;
  }

  if ((ich = DBGSerial.available())) {
    ich = 0;
    for (ich=0; ich < sizeof(szCmdLine); ich++) {
      ch = DBGSerial.read();
      if ((ch == -1) || ((ch >= 10) && (ch <= 15)))
        break;
      szCmdLine[ich] = ch;
    }
    szCmdLine[ich] = '\0';

    for (;;) {
      ch = DBGSerial.peek();
      if ((ch >= 10) && (ch <= 15))
        DBGSerial.read();
      else
        break;
    }
    if (ich) {
    DBGSerial.print(F("Serial Cmd Line:"));
    DBGSerial.write(szCmdLine, ich);
    DBGSerial.println(F("<eol>"));
    }

    if (!ich)  {
      g_fShowDebugPrompt = true;
    }
    else if ((ich == 1) && ((szCmdLine[0] == 'd') || (szCmdLine[0] == 'D'))) {
      g_fDebugOutput = !g_fDebugOutput;
      if (g_fDebugOutput)
        DBGSerial.println(F("Debug is on"));
      else
        DBGSerial.println(F("Debug is off"));
    }
#ifdef OPT_DUMP_EEPROM
    else if (((szCmdLine[0] == 'e') || (szCmdLine[0] == 'E'))) {
      DumpEEPROMCmd(szCmdLine);
    }
#endif
#ifdef QUADMODE
    else if (((szCmdLine[0] == 'g') || (szCmdLine[0] == 'G'))) {
      UpdateGaitCmd(szCmdLine);
    }
#endif
#ifdef OPT_DYNAMIC_ADJUST_LEGS
    else if (((szCmdLine[0] == 'i') || (szCmdLine[0] == 'I'))) {
      UpdateInitialPosAndAngCmd(szCmdLine);
    }
#endif
#ifdef OPT_TERMINAL_MONITOR_IC
    else if (g_InputController.ProcessTerminalCommand(szCmdLine, ich));
#endif
    else
    {
      g_ServoDriver.ProcessTerminalCommand(szCmdLine, ich);
    }
    return true;
  }
  return false;
}

//==============================================================================
// DumpEEPROM
//==============================================================================
#ifdef OPT_DUMP_EEPROM
byte g_bEEPromDumpMode = 0;
word g_wEEPromDumpStart = 0;
byte g_bEEPromDumpCnt = 16;

void DumpEEPROM() {
  byte i;
  word wDumpCnt = g_bEEPromDumpCnt;

  while (wDumpCnt) {
    DBGSerial.print(g_wEEPromDumpStart, HEX);
    DBGSerial.print(" - ");

    for (i = 0; (i < 16) && (i < wDumpCnt); i ++) {
      byte b;
      b = EEPROM.read(g_wEEPromDumpStart+i);
      DBGSerial.print(b, HEX);
      DBGSerial.print(" ");
    }

    DBGSerial.print(" : ");
    for (i = 0; (i < 16) && (i < wDumpCnt); i ++) {
      byte b;
      b = EEPROM.read(g_wEEPromDumpStart+i);
      if ((b > 0x1f) && (b < 0x7f))
        DBGSerial.write(b);
      else
        DBGSerial.print(".");
    }
    DBGSerial.println("");
    g_wEEPromDumpStart += i;
    wDumpCnt -= i;
  }
}
#endif

//==============================================================================
// GetCmdLineNum - passed pointer to pointer so we can update...
//==============================================================================
long GetCmdLineNum(byte **ppszCmdLine) {
  byte *psz = *ppszCmdLine;
  long iVal = 0;
  int iSign = 1;

  while (*psz == ' ')
    psz++;

  if ((*psz == '0') && ((*(psz+1) == 'x') || (*(psz+1) == 'X'))) {
    psz += 2;
    for (;;) {
      if ((*psz >= '0') && (*psz <= '9'))
        iVal = iVal * 16 + *psz++ - '0';
      else if ((*psz >= 'a') && (*psz <= 'f'))
        iVal = iVal * 16 + *psz++ - 'a' + 10;
      else if ((*psz >= 'A') && (*psz <= 'F'))
        iVal = iVal * 16 + *psz++ - 'A' + 10;
      else
        break;
    }

  }
  else {
    if (*psz == '-') {
        iSign = -1;
        psz++;
    }

    while ((*psz >= '0') && (*psz <= '9'))
      iVal = iVal * 10 + *psz++ - '0';
  }
  *ppszCmdLine = psz;
  return iSign * iVal;
}

#ifdef OPT_DUMP_EEPROM
//==============================================================================
// DumpEEPROMCmd
//==============================================================================
void DumpEEPROMCmd(byte *pszCmdLine) {
  if (!*++pszCmdLine)
    DumpEEPROM();
  else if ((*pszCmdLine == 'h') || (*pszCmdLine == 'H'))
    g_bEEPromDumpMode = 0;
  else if ((*pszCmdLine == 'w') || (*pszCmdLine == 'W'))
    g_bEEPromDumpMode = 0;

  else {
    g_wEEPromDumpStart = GetCmdLineNum(&pszCmdLine);

    if (*pszCmdLine == '=') {
      byte *psz = pszCmdLine;
      word w;
      while (*psz) {
        w = GetCmdLineNum(&psz);
        if (psz == pszCmdLine)
          break;
        pszCmdLine = psz;

        EEPROM.write(g_wEEPromDumpStart++, w & 0xff);
      }
    }
    else {
      if (*pszCmdLine == ' ') {
        g_bEEPromDumpCnt = GetCmdLineNum(&pszCmdLine);
      }
    }
    DumpEEPROM();
  }
}
#endif

#ifdef QUADMODE
//==============================================================================
// UpdateGaitCmd
//==============================================================================
void UpdateGaitCmd(byte *pszCmdLine) {
  if (!*++pszCmdLine) {
  DBGSerial.print("St: ");
  DBGSerial.print(g_InControlState.gaitCur.StepsInGait, DEC);
  DBGSerial.print(" ");
  DBGSerial.print(g_InControlState.gaitCur.NrLiftedPos, DEC);
  DBGSerial.print(" ");
  DBGSerial.print(g_InControlState.gaitCur.GaitLegNr[cRR], DEC);
  DBGSerial.print(" ");
  DBGSerial.print(g_InControlState.gaitCur.GaitLegNr[cRF], DEC);
  DBGSerial.print(" ");
  DBGSerial.print(g_InControlState.gaitCur.GaitLegNr[cLR], DEC);
  DBGSerial.print(" ");
  DBGSerial.println(g_InControlState.gaitCur.GaitLegNr[cLF], DEC);
  }
  else {
    word wStepsInGait = GetCmdLineNum(&pszCmdLine);
  word wLifted = GetCmdLineNum(&pszCmdLine);

  if (wStepsInGait) {
    if (wLifted) {
      g_InControlState.gaitCur.NrLiftedPos = wLifted;
      g_InControlState.gaitCur.FrontDownPos = (wLifted+1)/2;
      g_InControlState.gaitCur.LiftDivFactor = (wLifted > 4)? 4 : 2;
    }

    g_InControlState.gaitCur.StepsInGait = wStepsInGait;
    g_InControlState.gaitCur.TLDivFactor = g_InControlState.gaitCur.StepsInGait-g_InControlState.gaitCur.NrLiftedPos;

    g_InControlState.gaitCur.GaitLegNr[cRR] = GetCmdLineNum(&pszCmdLine);
    if (g_InControlState.gaitCur.GaitLegNr[cRR]) {
      g_InControlState.gaitCur.GaitLegNr[cRF] = GetCmdLineNum(&pszCmdLine);
      g_InControlState.gaitCur.GaitLegNr[cLR] = GetCmdLineNum(&pszCmdLine);
      g_InControlState.gaitCur.GaitLegNr[cLF] = GetCmdLineNum(&pszCmdLine);
    }
    else {
      wStepsInGait /= 4;
      g_InControlState.gaitCur.GaitLegNr[cRR] = wStepsInGait / 2;
      g_InControlState.gaitCur.GaitLegNr[cRF] = g_InControlState.gaitCur.GaitLegNr[cRR] + wStepsInGait;
      g_InControlState.gaitCur.GaitLegNr[cLR] = g_InControlState.gaitCur.GaitLegNr[cRF] + wStepsInGait;
      g_InControlState.gaitCur.GaitLegNr[cLF] = g_InControlState.gaitCur.GaitLegNr[cLR] + wStepsInGait;
    }
  }
  }
}
#endif

//==============================================================================
// UpdateGaitCmd
//==============================================================================
#ifdef OPT_DYNAMIC_ADJUST_LEGS
void UpdateInitialPosAndAngCmd(byte *pszCmdLine) {
  if (!*++pszCmdLine) {
  DBGSerial.print("Len: ");
  DBGSerial.print(GetLegsXZLength() , DEC);
  DBGSerial.print(" Angs: ");
    for(int LegIndex=0; LegIndex < CNT_LEGS; LegIndex++) {
        DBGSerial.print(g_InControlState.aCoxaInitAngle1[LegIndex], DEC);
        DBGSerial.print(" ");
    }
    DBGSerial.println();
  }
  else {
    word wNewLegsXZPos = GetCmdLineNum(&pszCmdLine);
    if (*pszCmdLine) {
      int  iDeltaAngle = GetCmdLineNum(&pszCmdLine);
      RotateLegInitAngles(iDeltaAngle);
    }
    AdjustLegPositions(wNewLegsXZPos);

  }
}
#endif

#endif
