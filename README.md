# Hexapod
This is my personal fork of the original phoenix hexapod code written by [KurtE](https://github.com/KurtE).
It has been changed to work with phoenix clones from china, these are significantly cheaper, but use cheaper servo's with inverted logic. They are also slightly bigger. Therefore, I have made changes to some of the files (mainly `Hex_Cfg.h`) to make the inverse kinematics work better on the phoenix clone.

## Configuration
The code is setup to work with a phoenix hexapod clone (often found from Chinese sellers) running a Lynxmotion SSC32-U and Arduino Nano (Other Arduino boards should work too, just be sure to check the pinout for the PS2 remote). These should be connected as follows:
```
TX	-> RX
RX	-> TX
GND	-> GND
```
The hexapod is controlled with a generic PS2 remote, you'll need to download the [Bill Porter PS2 Library](http://www.billporter.info/2010/06/05/playstation-2-controller-arduino-library-v1-0/) yourself and install it in the Arduino IDE. The PS2 receiver should be wired up to the Arduino as follows:
```
PS2_DAT	6	(data)
PS2_CMD	7	(command)
PS2_SEL	8	(attention)
PS2_CLK	9	(clock)
```
