# Hexapod
This is my personal fork of the original phoenix hexapod code written by [KurtE](https://github.com/KurtE).
It has been changed to work with phoenix clones from china, these are significantly cheaper, but use cheaper servo's with inverted logic. They are also slightly bigger. Therefore, I have made changes to some of the files (mainly `Hex_Cfg.h`) to make the inverse kinematics work better on the phoenix clone.

## Configuration
The code is setup to work with a phoenix hexapod clone (often found from Chinese sellers) running a Lynxmotion SSC32-U and Arduino Nano. These should be connected as follows:
```
TX	-> RX
RX	-> TX
GND	-> GND
```
