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
#ifdef QUADMODE
const byte cCoxaPin[] PROGMEM = {
  cRRCoxaPin,  cRFCoxaPin,  cLRCoxaPin,  cLFCoxaPin};
const byte cFemurPin[] PROGMEM = {
  cRRFemurPin, cRFFemurPin, cLRFemurPin, cLFFemurPin};
const byte cTibiaPin[] PROGMEM = {
  cRRTibiaPin, cRFTibiaPin, cLRTibiaPin, cLFTibiaPin};
#ifdef c4DOF
const byte cTarsPin[] PROGMEM = {
  cRRTarsPin, cRFTarsPin, cLRTarsPin, cLFTarsPin};
#endif
#else
const byte cCoxaPin[] PROGMEM = {
  cRRCoxaPin,  cRMCoxaPin,  cRFCoxaPin,  cLRCoxaPin,  cLMCoxaPin,  cLFCoxaPin};
const byte cFemurPin[] PROGMEM = {
  cRRFemurPin, cRMFemurPin, cRFFemurPin, cLRFemurPin, cLMFemurPin, cLFFemurPin};
const byte cTibiaPin[] PROGMEM = {
  cRRTibiaPin, cRMTibiaPin, cRFTibiaPin, cLRTibiaPin, cLMTibiaPin, cLFTibiaPin};
#ifdef c4DOF
const byte cTarsPin[] PROGMEM = {
  cRRTarsPin, cRMTarsPin, cRFTarsPin, cLRTarsPin, cLMTarsPin, cLFTarsPin};
#endif
#endif

#ifdef __AVR__
#if not defined(UBRR1H)
#if cSSC_IN == 0
#define SSCSerial Serial
#else
SoftwareSerial SSCSerial(cSSC_IN, cSSC_OUT);
#endif    
#endif
#endif

//==============================================================================
// Global - Local to this file only...
//==============================================================================
extern int SSCRead (byte* pb, int cb, word wTimeout, word wEOL);

//Init
void ServoDriver::Init(void) {
  SSCSerial.begin(cSSC_BAUD);

#ifdef OPT_GPPLAYER
  char abT[4];
  byte cbRead;

  _fGPEnabled = false;
  _fGPActive = false;

#ifdef __AVR__
#if not defined(UBRR1H)
#if cSSC_IN != 0
  SSCSerial.listen();
#endif    
#endif    
#endif
  SSCSerial.println(F("QPL0"));
  cbRead = SSCRead((byte*)abT, 4, 25000, (word)-1);

#ifdef DBGSerial
  DBGSerial.print(F("Check GP Enable: "));
  DBGSerial.println(cbRead, DEC);
#endif        
  if (cbRead == 4)
    _fGPEnabled = true;
  else
    MSound (2, 40, 2500, 40, 2500);
#endif
#ifdef cVoltagePin
  for (byte i=0; i < 8; i++)
  GetBatteryVoltage();  
#endif
}

//==============================================================================
//GetBatteryVoltage - Maybe should try to minimize when this is called
// as it uses the serial port... Maybe only when we are not interpolating 
// or if maybe some minimum time has elapsed...
//==============================================================================
#ifdef cVoltagePin  
#ifndef CVADR1
#define CVADR1      30
#define CVADR2      10
#endif

uint16_t  g_awVoltages[8]={
  0,0,0,0,0,0,0,0};
uint16_t  g_wVoltageSum = 0;
byte  g_iVoltages = 0;

uint16_t ServoDriver::GetBatteryVoltage(void) {
  g_iVoltages = (++g_iVoltages)&0x7;
  g_wVoltageSum -= g_awVoltages[g_iVoltages];
  g_awVoltages[g_iVoltages] = analogRead(cVoltagePin);
  g_wVoltageSum += g_awVoltages[g_iVoltages];

#ifdef CVREF
  return ((long)((long)g_wVoltageSum*CVREF*(CVADR1+CVADR2))/(long)(8192*(long)CVADR2));  
#else
  return ((long)((long)g_wVoltageSum*125*(CVADR1+CVADR2))/(long)(2048*(long)CVADR2));  
#endif
}
#endif


//==============================================================================
// Quick and dirty helper function to read so many bytes in from the SSC 
// with a timeout and an end of character marker...
//==============================================================================
int SSCRead (byte* pb, int cb, word wTimeout, word wEOL)
{
  int ich;
  byte* pbIn = pb;
  unsigned long ulTimeLastChar = micros();
  while (cb) {
    while (!SSCSerial.available()) {
      // check for timeout
      if ((word)(micros()-ulTimeLastChar) > wTimeout) {
        return (int)(pb-pbIn);
      }    
    }
    ich = SSCSerial.read();
    *pb++ = (byte)ich;
    cb--;

    if ((word)ich == wEOL)
      break;
    ulTimeLastChar = micros();
  }
  return (int)(pb-pbIn);
}

//==============================================================================
//[GP PLAYER]
//==============================================================================
#ifdef OPT_GPPLAYER
uint8_t g_bGPCntSteps;
uint8_t g_bGPCurStep;
boolean g_fGPSMChanged;

//==============================================================================
//[FIsGPSeqDefined]
//==============================================================================
boolean ServoDriver::FIsGPSeqDefined(uint8_t iSeq)
{
  word wGPSeqPtr;
  
  SSCSerial.print(F("EER -"));
  SSCSerial.print(iSeq*2, DEC);
  SSCSerial.println(F(";2"));
  if ((SSCRead((byte*)&wGPSeqPtr, sizeof(wGPSeqPtr), 1000, 0xffff) == sizeof(wGPSeqPtr)) && (wGPSeqPtr != 0)  && (wGPSeqPtr != 0xffff)) {
    return true;
  }
  return false;
}

//==============================================================================
// Setup to start sequence number...
//==============================================================================
void ServoDriver::GPStartSeq(uint8_t iSeq)
{
  if (!_fGPActive && (iSeq != 0xff)) {
    _fGPActive = true;
    _iSeq = iSeq;
    g_bGPCntSteps = 0xff;
    g_fGPSMChanged = false;
  }
  else {
    _iSeq = iSeq;  // Signal for abort
  }

}

//==============================================================================
//[GP PLAYER]
//==============================================================================
void ServoDriver::GPPlayer(void)
{
  byte abStat[4];
  byte cbRead;

  if (_fGPActive) {
    if (g_bGPCntSteps == 0xff) {
      g_bGPCntSteps = GPNumSteps();
      if (g_bGPCntSteps == 0xff) {
        _fGPActive = false;
      }
      else  {    
        g_InputController.AllowControllerInterrupts(false);   

        // Since we are monitoring GP Sequence we no longer tell it to only run once...
        SSCSerial.print(F("PL0SQ"));
        SSCSerial.println(_iSeq, DEC);
        delay(20);
        while (SSCSerial.read() != -1)
          ;
        g_InputController.AllowControllerInterrupts(true);    
      }
    }
    else {
      g_InputController.AllowControllerInterrupts(false);   
      if (_iSeq == 0xff) {
        SSCSerial.println(F("PL0"));
        _fGPActive=false;
      }
      else {
        SSCSerial.print(F("QPL0\r"));
        cbRead = SSCRead((byte*)abStat, sizeof(abStat), 10000, (word)-1);

        g_bGPCurStep = abStat[1];
        if ((g_bGPCurStep == (g_bGPCntSteps-1)) && (abStat[3] == 0)) {
          // We are done 
          SSCSerial.println(F("PL0"));
          _fGPActive=false;
        }
        else if (g_fGPSMChanged) {
          g_fGPSMChanged = false;
          SSCSerial.print(F("PL0SM"));
          SSCSerial.println(_sGPSM, DEC);
        }
      }
      g_InputController.AllowControllerInterrupts(true);
    }
  }  
}

//==============================================================================
uint8_t ServoDriver::GPNumSteps(void)
{
  word wSeqStart;
  uint8_t bGPCntSteps = 0xff;
  byte cbRead;
  g_InputController.AllowControllerInterrupts(false);

  SSCSerial.print(F("EER -"));
  SSCSerial.print(_iSeq, DEC);
  SSCSerial.println(F(";2"));
  cbRead = SSCRead((byte*)&wSeqStart, sizeof(wSeqStart), 10000, (word)-1);
  
  if ((cbRead == sizeof(wSeqStart)) && (wSeqStart != 0) && (wSeqStart != 0xffff)) {
    SSCSerial.print(F("EER -"));
    SSCSerial.print(wSeqStart, DEC);
    SSCSerial.println(F(";1"));
    cbRead = SSCRead((byte*)bGPCntSteps, sizeof(bGPCntSteps), 10000, (word)-1);
  }
  return bGPCntSteps;
}

//==============================================================================
uint8_t ServoDriver::GPCurStep(void)
{
  return _fGPActive ? g_bGPCurStep : 0xff;
}

//==============================================================================
void ServoDriver::GPSetSpeedMultiplyer(short sm)
{
  if (_sGPSM != sm) {
    _sGPSM = sm;
    g_fGPSMChanged = true;
  }
}

#endif // OPT_GPPLAYER

//==============================================================================
//[BeginServoUpdate] Does whatever preperation that is needed 
// to starrt a move of our servos
//==============================================================================
void ServoDriver::BeginServoUpdate(void)
{
}

//==============================================================================
//[OutputServoInfoForLeg] Do the output to the SSC-32 for the servos
// associated with the Leg number passed in.
//==============================================================================
#define cPwmDiv       991  //old 1059;
#define cPFConst      592  //old 650 ; 900*(1000/cPwmDiv)+cPFConst must always be 1500

#ifdef c4DOF
void ServoDriver::OutputServoInfoForLeg(byte LegIndex, short sCoxaAngle1, short sFemurAngle1, short sTibiaAngle1, short sTarsAngle1)
#else
void ServoDriver::OutputServoInfoForLeg(byte LegIndex, short sCoxaAngle1, short sFemurAngle1, short sTibiaAngle1)
#endif    
{        
  word    wCoxaSSCV;
  word    wFemurSSCV;
  word    wTibiaSSCV;
#ifdef c4DOF
  word    wTarsSSCV;
#endif

  wCoxaSSCV = ((long)(sCoxaAngle1 +900))*1000/cPwmDiv+cPFConst;
  wFemurSSCV = ((long)((long)(sFemurAngle1+900))*1000/cPwmDiv+cPFConst);
  wTibiaSSCV = ((long)(sTibiaAngle1+900))*1000/cPwmDiv+cPFConst;
#ifdef c4DOF
  wTarsSSCV = ((long)(sTarsAngle1+900))*1000/cPwmDiv+cPFConst;
#endif

#ifdef cSSC_BINARYMODE
  SSCSerial.write(pgm_read_byte(&cCoxaPin[LegIndex])  + 0x80);
  SSCSerial.write(wCoxaSSCV >> 8);
  SSCSerial.write(wCoxaSSCV & 0xff);
  SSCSerial.write(pgm_read_byte(&cFemurPin[LegIndex]) + 0x80);
  SSCSerial.write(wFemurSSCV >> 8);
  SSCSerial.write(wFemurSSCV & 0xff);
  SSCSerial.write(pgm_read_byte(&cTibiaPin[LegIndex]) + 0x80);
  SSCSerial.write(wTibiaSSCV >> 8);
  SSCSerial.write(wTibiaSSCV & 0xff);
#ifdef c4DOF
  if ((byte)pgm_read_byte(&cTarsLength[LegIndex])) {
    SSCSerial.write(pgm_read_byte(&cTarsPin[LegIndex]) + 0x80);
    SSCSerial.write(wTarsSSCV >> 8);
    SSCSerial.write(wTarsSSCV & 0xff);
  }
#endif
#else
  SSCSerial.print("#");
  SSCSerial.print(pgm_read_byte(&cCoxaPin[LegIndex]), DEC);
  SSCSerial.print("P");
  SSCSerial.print(wCoxaSSCV, DEC);
  SSCSerial.print("#");
  SSCSerial.print(pgm_read_byte(&cFemurPin[LegIndex]), DEC);
  SSCSerial.print("P");
  SSCSerial.print(wFemurSSCV, DEC);
  SSCSerial.print("#");
  SSCSerial.print(pgm_read_byte(&cTibiaPin[LegIndex]), DEC);
  SSCSerial.print("P");
  SSCSerial.print(wTibiaSSCV, DEC);
#ifdef c4DOF
  if ((byte)pgm_read_byte(&cTarsLength[LegIndex])) {
    SSCSerial.print("#");
    SSCSerial.print(pgm_read_byte(&cTarsPin[LegIndex]), DEC);
    SSCSerial.print("P");
    SSCSerial.print(wTarsSSCV, DEC);
  }
#endif
#endif        
  g_InputController.AllowControllerInterrupts(true);    // Ok for hserial again...
}

//==============================================================================
//[CommitServoDriver Updates the positions of the servos - This outputs
//         as much of the command as we can without committing it.  This
//         allows us to once the previous update was completed to quickly 
//        get the next command to start
//==============================================================================
void ServoDriver::CommitServoDriver(word wMoveTime)
{
#ifdef cSSC_BINARYMODE
  byte    abOut[3];
#endif

  g_InputController.AllowControllerInterrupts(false);

#ifdef cSSC_BINARYMODE
  abOut[0] = 0xA1;
  abOut[1] = wMoveTime >> 8;
  abOut[2] = wMoveTime & 0xff;
  SSCSerial.write(abOut, 3);
#else
  //Send <CR>
  SSCSerial.print("T");
  SSCSerial.println(wMoveTime, DEC);
#endif

  g_InputController.AllowControllerInterrupts(true);
}

//==============================================================================
//[FREE SERVOS] Frees all the servos
//==============================================================================
void ServoDriver::FreeServos(void)
{
  g_InputController.AllowControllerInterrupts(false);
  for (byte LegIndex = 0; LegIndex < 32; LegIndex++) {
    SSCSerial.print("#");
    SSCSerial.print(LegIndex, DEC);
    SSCSerial.print("P0");
  }
  SSCSerial.print("T200\r");
  g_InputController.AllowControllerInterrupts(true);    
}

//==============================================================================
//Function that gets called from the main loop if the robot is not logically
//     on.  Gives us a chance to play some...
//==============================================================================
void ServoDriver::IdleTime(void)
{
}

#ifdef OPT_TERMINAL_MONITOR  
extern void FindServoOffsets(void);
extern void SSCForwarder(void);

//==============================================================================
// ShowTerminalCommandList: Allow the Terminal monitor to call the servo driver
//      to allow it to display any additional commands it may have.
//==============================================================================
void ServoDriver::ShowTerminalCommandList(void) 
{
#ifdef OPT_FIND_SERVO_OFFSETS
  DBGSerial.println(F("O - Enter Servo offset mode"));
#endif        
#ifdef OPT_SSC_FORWARDER
  DBGSerial.println(F("S - SSC Forwarder"));
#endif        
}

//==============================================================================
// ProcessTerminalCommand: The terminal monitor will call this to see if the
//     command the user entered was one added by the servo driver.
//==============================================================================
boolean ServoDriver::ProcessTerminalCommand(byte *psz, byte bLen)
{
#ifdef OPT_FIND_SERVO_OFFSETS
  if ((bLen == 1) && ((*psz == 'o') || (*psz == 'O'))) {
    FindServoOffsets();
  }
#endif
#ifdef OPT_SSC_FORWARDER
  if ((bLen == 1) && ((*psz == 's') || (*psz == 'S'))) {
    SSCForwarder();
  }
#endif
  return true;
}

//==============================================================================
// SSC Forwarder - used to allow things like Lynxterm to talk to the SSC-32 
// through the Arduino...  Will see if it is fast enough...
//==============================================================================
#ifdef OPT_SSC_FORWARDER
void  SSCForwarder(void) 
{
  int sChar;
  int sPrevChar; 
  DBGSerial.println("SSC Forwarder mode - Enter $<cr> to exit");

  for (;;) {
    if ((sChar = DBGSerial.read()) != -1) {
      SSCSerial.write(sChar & 0xff);
      if (((sChar == '\n') || (sChar == '\r')) && (sPrevChar == '$'))
        break;
      sPrevChar = sChar;
    }

    if ((sChar = SSCSerial.read()) != -1) {
      DBGSerial.write(sChar & 0xff);
    }
  }
  DBGSerial.println("Exited SSC Forwarder mode");
}
#endif // OPT_SSC_FORWARDER

//==============================================================================
//  FindServoOffsets - Find the zero points for each of our servos... 
//    Will use the new servo function to set the actual pwm rate and see
//    how well that works...
//==============================================================================
#ifdef OPT_FIND_SERVO_OFFSETS
#ifndef NUMSERVOSPERLEG
#define NUMSERVOSPERLEG 3
#endif

void FindServoOffsets()
{
  byte abSSCServoNum[NUMSERVOSPERLEG*CNT_LEGS];
  signed short asOffsets[NUMSERVOSPERLEG*CNT_LEGS];
  signed char asOffsetsRead[NUMSERVOSPERLEG*CNT_LEGS];

  static char *apszLegs[] = {
#ifdef QUADMODE
    "RR","RF", "LR", "LF"              };
#else
    "RR","RM","RF", "LR", "LM", "LF"              };
#endif
  static char *apszLJoints[] = {
    " Coxa", " Femur", " Tibia", " tArs"              };

  byte szTemp[5];
  byte cbRead;

  int data;
  short sSN;
  boolean fNew = true;
  boolean fExit = false;
  
  if (CheckVoltage()) { 
    Serial.println("Low Voltage: fix or hit $ to abort");
    while (CheckVoltage()) {
      if (Serial.read() == '$')  return;
    }
  }
  
  for (sSN=0; sSN < CNT_LEGS; sSN++) {
    abSSCServoNum[sSN*NUMSERVOSPERLEG + 0] = pgm_read_byte(&cCoxaPin[sSN]);
    abSSCServoNum[sSN*NUMSERVOSPERLEG + 1] = pgm_read_byte(&cFemurPin[sSN]);
    abSSCServoNum[sSN*NUMSERVOSPERLEG + 2] = pgm_read_byte(&cTibiaPin[sSN]);
#ifdef c4DOF
    abSSCServoNum[sSN*NUMSERVOSPERLEG + 3] = pgm_read_byte(&cTarsPin[sSN]);
#endif
  }
  for (sSN=0; sSN < CNT_LEGS*NUMSERVOSPERLEG; sSN++ ) {
    asOffsets[sSN] = 0;       
    asOffsetsRead[sSN] = 0; 

    SSCSerial.print("R");
    SSCSerial.println(32+abSSCServoNum[sSN], DEC);

    cbRead = SSCRead((byte*)szTemp, sizeof(szTemp), 10000, 13);
    if (cbRead > 0)
      asOffsetsRead[sSN] = atoi((const char *)szTemp);

    SSCSerial.print("#");
    SSCSerial.print(abSSCServoNum[sSN], DEC);
    SSCSerial.println("P1500");
  }

  Serial.println("Find Servo Zeros.\n$-Exit, +- changes, *-change servo");
  Serial.println("    0-n Chooses a leg, C-Coxa, F-Femur, T-Tibia");

  sSN = true;
  while(!fExit) {
    if (fNew) {
      Serial.print("Servo: ");
      Serial.print(apszLegs[sSN/NUMSERVOSPERLEG]);
      Serial.print(apszLJoints[sSN%NUMSERVOSPERLEG]);
      Serial.print("(");
      Serial.print(asOffsetsRead[sSN]+asOffsets[sSN], DEC);
      Serial.println(")");

      SSCSerial.print("#");
      SSCSerial.print(abSSCServoNum[sSN], DEC);
      SSCSerial.print("P");
      SSCSerial.print(1500+asOffsets[sSN]+250, DEC);
      SSCSerial.println("T250");
      delay(250);

      SSCSerial.print("#");
      SSCSerial.print(abSSCServoNum[sSN], DEC);
      SSCSerial.print("P");
      SSCSerial.print(1500+asOffsets[sSN]-250, DEC);
      SSCSerial.println("T500");
      delay(500);

      SSCSerial.print("#");
      SSCSerial.print(abSSCServoNum[sSN], DEC);
      SSCSerial.print("P");
      SSCSerial.print(1500+asOffsets[sSN], DEC);
      SSCSerial.println("T250");
      delay(250);

      fNew = false;
    }

    data = Serial.read();
    //if data received
    if (data !=-1)  {
      if (data == '$')
        fExit = true;
        
      else if ((data == '+') || (data == '-')) {
        if (data == '+')
          asOffsets[sSN] += 5;
        else
          asOffsets[sSN] -= 5;

        Serial.print("    ");
        Serial.println(asOffsetsRead[sSN]+asOffsets[sSN], DEC);

        SSCSerial.print("#");
        SSCSerial.print(abSSCServoNum[sSN], DEC);
        SSCSerial.print("P");
        SSCSerial.print(1500+asOffsets[sSN], DEC);
        SSCSerial.println("T100");
      } 
      else if ((data >= '0') && (data <= '5')) {
        fNew = true;
        sSN = (sSN % NUMSERVOSPERLEG) + (data - '0')*NUMSERVOSPERLEG;
      } 
      else if ((data == 'c') || (data == 'C')) {
        fNew = true;
        sSN = (sSN / NUMSERVOSPERLEG) * NUMSERVOSPERLEG + 0;
      } 
      else if ((data == 'f') || (data == 'F')) {
        fNew = true;
        sSN = (sSN / NUMSERVOSPERLEG) * NUMSERVOSPERLEG + 1;
      } 
      else if ((data == 't') || (data == 'T')) {
        // direct enter of which servo to change
        fNew = true;
        sSN = (sSN / NUMSERVOSPERLEG) * NUMSERVOSPERLEG + 2;
      } 
      else if (data == '*') {
        fNew = true;
        sSN++;
        if (sSN == CNT_LEGS*NUMSERVOSPERLEG) 
          sSN = 0;  
      }
    }
  }
  Serial.print("Find Servo exit ");
  for (sSN=0; sSN < CNT_LEGS*NUMSERVOSPERLEG; sSN++){
    Serial.print("Servo: ");
    Serial.print(apszLegs[sSN/NUMSERVOSPERLEG]);
    Serial.print(apszLJoints[sSN%NUMSERVOSPERLEG]);
    Serial.print("(");
    Serial.print(asOffsetsRead[sSN]+asOffsets[sSN], DEC);
    Serial.println(")");
  }

  Serial.print("\nSave Changes? Y/N: ");

  while (((data = Serial.read()) == -1) || ((data >= 10) && (data <= 15)))
    ; 

  if ((data == 'Y') || (data == 'y')) {

    for (sSN=0; sSN < CNT_LEGS*NUMSERVOSPERLEG; sSN++ ) {
      SSCSerial.print("R");
      SSCSerial.print(32+abSSCServoNum[sSN], DEC);
      SSCSerial.print("=");
      SSCSerial.println(asOffsetsRead[sSN]+asOffsets[sSN], DEC);
      delay(10);
    }

    delay(10);
    SSCSerial.println("GOBOOT");
    delay(5);
    SSCSerial.println("g0000");
    delay(500);
  } 
  else {
    void LoadServosConfig();
  }

  g_ServoDriver.FreeServos();

}
#endif  // OPT_FIND_SERVO_OFFSETS
#endif  // 
