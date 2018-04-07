# Hexapod
This is my personal fork of the original phoenix hexapod code written by [KurtE](https://github.com/KurtE).
It has been changed to work with phoenix clones from china, these are significantly cheaper, but use cheaper servo's with inverted logic. They are also slightly bigger. Therefore, I have made changes to some of the files (mainly `Hex_Cfg.h`) to make the inverse kinematics work better on the phoenix clone.

## hardware
- Phoenix clone frame
- 18x Towerpro MG996R servo
- Arduino Nano
- Lynxmotion SSC32-U
- 6v 3000mah NiMH battery (servo power)
- 9v battery (logic power)

## Installation + Usage
1. [Download](https://github.com/Teeffelen/Hexapod/archive/master.zip) the files as a zip and extract them.
2. Download the [Bill Porter PS2 Library](http://www.billporter.info/2010/06/05/playstation-2-controller-arduino-library-v1-0/) and install it in the Ardiuno IDE.
3. Open the `Phoenix_PS2_SSC32.ino` in the Arduino IDE.
4. Select your board in the menu and hit verify.
5. If the code compiles you are ready to upload it to your Arduino.
6. Run the code by powering the hexapod and logic boards.
7. Press `start` on the PS2 receiver to start the servos, you can find all controls [here](https://github.com/Teeffelen/Hexapod#controls).

## Configuration
The code is setup to work with a phoenix hexapod clone running a Lynxmotion SSC32-U and Arduino Nano (Other Arduino boards should work too, just be sure to check the pinout for the PS2 remote). These should be connected as follows:
```
SSC32:	Arduino:
TX	RX
RX	TX
GND	GND
```
The hexapod itself is controlled with a generic PS2 remote, you'll need to download the [Bill Porter PS2 Library](http://www.billporter.info/2010/06/05/playstation-2-controller-arduino-library-v1-0/) yourself and install it in the Arduino IDE. The PS2 receiver should be wired up to the Arduino as follows:
```
PS2:		Arduino:
Data		D6
Command		D7
Attention	D8
Clock		D9
```
Pinout can be configured in the `Hex_Cfg.h` file.

## Controls
```
[Common controls]
Start		Turn on/off the bot
Circle		Toggle Single leg mode
Triangle	Move body of the ground (walking pos) and back to the ground
Square		Toggle balance mode
L1		Toggle shift mode
L2		Toggle rotate mode
D-up		Move body up by 10mm
D-down		Move body down by 10mm
D-left		Decrease speed
D-right		Increase speed

[Walk mode]
Select		Change the walking gait
Left stick	Walk/Strafe
Right stick	Rotate
R1		Toggle double gait travel speed
R2		Toggle double gait travel length

[Shift Controls]
Left Stick	Shift body X/Z
Right Stick	Shift body Y and rotate body Y

[Rotate Controls]
Left Stick	Rotate body X/Z
Right Stick	Rotate body Y

[Single leg Controls]
Select		Switch legs
Left Stick	Move Leg X/Z (relative)
Right Stick	Move Leg Y (absolute)
R2		Hold/release leg position
```
