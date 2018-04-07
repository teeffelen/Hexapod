//==============================================================================
// V2.3
//==============================================================================
#ifndef HEX_CFG_PHOENIX3_H
#define HEX_CFG_PHOENIX3_H
#define OPT_TERMINAL_MONITOR

#ifdef OPT_TERMINAL_MONITOR
//#define OPT_SSC_FORWARDER
#define OPT_FIND_SERVO_OFFSETS
#endif

#define OPT_GPPLAYER
#define OPT_SINGLELEG

#define DBGSerial         Serial
//#define DEBUG_IOPINS

#if defined(UBRR1H)
#define SSCSerial         Serial1
#else
#endif

#define USEPS2
#define USE_SSC32
//#define  cSSC_BINARYMODE 1
//#undef OPT_FIND_SERVO_OFFSETS
#define cSSC_BAUD        38400   //SSC32 BAUD rate

//==============================================================================
//[Arduino pin configuration]
#define SOUND_PIN    5
#define PS2_DAT      6
#define PS2_CMD      7
#define PS2_SEL      8
#define PS2_CLK      9
//#define cSSC_OUT     0
//#define cSSC_IN      1

//==============================================================================
//[SSC32 pin configuration]
#define cRRCoxaPin      0   //Rear Right leg Hip Horizontal
#define cRRFemurPin     1   //Rear Right leg Hip Vertical
#define cRRTibiaPin     2   //Rear Right leg Knee
#define cRRTarsPin      3   //Tar

#define cRMCoxaPin      4   //Middle Right leg Hip Horizontal
#define cRMFemurPin     5   //Middle Right leg Hip Vertical
#define cRMTibiaPin     6   //Middle Right leg Knee
#define cRMTarsPin      7   //Tar

#define cRFCoxaPin      8   //Front Right leg Hip Horizontal
#define cRFFemurPin     9   //Front Right leg Hip Vertical
#define cRFTibiaPin     10   //Front Right leg Knee
#define cRFTarsPin      11   //Tar

#define cLRCoxaPin      16   //Rear Left leg Hip Horizontal
#define cLRFemurPin     17   //Rear Left leg Hip Vertical
#define cLRTibiaPin     18   //Rear Left leg Knee
#define cLRTarsPin      19   //Tar

#define cLMCoxaPin      20   //Middle Left leg Hip Horizontal
#define cLMFemurPin     21   //Middle Left leg Hip Vertical
#define cLMTibiaPin     22   //Middle Left leg Knee
#define cLMTarsPin      23   //Tar

#define cLFCoxaPin      24   //Front Left leg Hip Horizontal
#define cLFFemurPin     25   //Front Left leg Hip Vertical
#define cLFTibiaPin     26   //Front Left leg Knee
#define cLFTarsPin      27   //Tar


//==============================================================================
//[Min/Max angles]
#define cRRCoxaMin1 -260
#define cRRCoxaMax1 740
#define cRRFemurMin1  -1010
#define cRRFemurMax1  950
#define cRRTibiaMin1  -1060
#define cRRTibiaMax1  770

#define cRMCoxaMin1 -530
#define cRMCoxaMax1 530
#define cRMFemurMin1  -1010
#define cRMFemurMax1  950
#define cRMTibiaMin1  -1060
#define cRMTibiaMax1  770

#define cRFCoxaMin1 -580
#define cRFCoxaMax1 740
#define cRFFemurMin1  -1010
#define cRFFemurMax1  950
#define cRFTibiaMin1  -1060
#define cRFTibiaMax1  770

#define cLRCoxaMin1 -740
#define cLRCoxaMax1 260
#define cLRFemurMin1  -950
#define cLRFemurMax1  1010
#define cLRTibiaMin1  -770
#define cLRTibiaMax1  1060

#define cLMCoxaMin1 -530
#define cLMCoxaMax1 530
#define cLMFemurMin1  -950
#define cLMFemurMax1  1010
#define cLMTibiaMin1  -770
#define cLMTibiaMax1  1060

#define cLFCoxaMin1 -740
#define cLFCoxaMax1 580
#define cLFFemurMin1  -950
#define cLFFemurMax1  1010
#define cLFTibiaMin1  -770
#define cLFTibiaMax1  1060


//==============================================================================
//[Leg dimensions]
//Universal dimensions for each leg in mm
#define cXXCoxaLength     29
#define cXXFemurLength    76
#define cXXTibiaLength    106
#define cXXTarsLength     85

#define cRRCoxaLength     cXXCoxaLength
#define cRRFemurLength    cXXFemurLength
#define cRRTibiaLength    cXXTibiaLength
#define cRRTarsLength     cXXTarsLength

#define cRMCoxaLength     cXXCoxaLength
#define cRMFemurLength    cXXFemurLength
#define cRMTibiaLength    cXXTibiaLength
#define cRMTarsLength     cXXTarsLength

#define cRFCoxaLength     cXXCoxaLength
#define cRFFemurLength    cXXFemurLength
#define cRFTibiaLength    cXXTibiaLength
#define cRFTarsLength     cXXTarsLength

#define cLRCoxaLength     cXXCoxaLength
#define cLRFemurLength    cXXFemurLength
#define cLRTibiaLength    cXXTibiaLength
#define cLRTarsLength     cXXTarsLength

#define cLMCoxaLength     cXXCoxaLength
#define cLMFemurLength    cXXFemurLength
#define cLMTibiaLength    cXXTibiaLength
#define cLMTarsLength     cXXTarsLength

#define cLFCoxaLength     cXXCoxaLength
#define cLFFemurLength    cXXFemurLength
#define cLFTibiaLength    cXXTibiaLength
#define cLFTarsLength     cXXTarsLength


//==============================================================================
//[Body dimensions]
#define cRRCoxaAngle1    -600
#define cRMCoxaAngle1    0
#define cRFCoxaAngle1    600
#define cLRCoxaAngle1    -600
#define cLMCoxaAngle1    0
#define cLFCoxaAngle1    600

#define cRROffsetX       -43
#define cRROffsetZ       82
#define cRMOffsetX       -63
#define cRMOffsetZ       0
#define cRFOffsetX       -43
#define cRFOffsetZ       -82

#define cLROffsetX       43
#define cLROffsetZ       82
#define cLMOffsetX       63
#define cLMOffsetZ       0
#define cLFOffsetX       43
#define cLFOffsetZ       -82

//==============================================================================
//[Start positions legs]
#define cHexInitXZ       105
#define CHexInitXZCos60  53
#define CHexInitXZSin60  91
#define CHexInitY        25

#define cRRInitPosX      CHexInitXZCos60
#define cRRInitPosY      CHexInitY
#define cRRInitPosZ      CHexInitXZSin60

#define cRMInitPosX      cHexInitXZ
#define cRMInitPosY      CHexInitY
#define cRMInitPosZ      0

#define cRFInitPosX      CHexInitXZCos60
#define cRFInitPosY      CHexInitY
#define cRFInitPosZ      -CHexInitXZSin60

#define cLRInitPosX      CHexInitXZCos60
#define cLRInitPosY      CHexInitY
#define cLRInitPosZ      CHexInitXZSin60

#define cLMInitPosX      cHexInitXZ
#define cLMInitPosY      CHexInitY
#define cLMInitPosZ      0

#define cLFInitPosX      CHexInitXZCos60
#define cLFInitPosY      CHexInitY
#define cLFInitPosZ      -CHexInitXZSin60

#endif CFG_HEX_H
