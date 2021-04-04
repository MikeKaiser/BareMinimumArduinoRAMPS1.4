# BareMinimumArduinoRAMPS1.4
## What is it?
It's a tiny program to initialise an ArduinoMega 2560 connected to a RAMPS1.4 board and test the 12864 graphical LCD panel, SDCard and buttons.
## Why?
The code form Marlin has grown way too complex and I couldn't find a version of GRBL that would talk to the graphical LCDs I had.

I just wanted to see what the bare minimum of code looked like for initialising the panel, reading an ExFat SDCard and detecting button presses.
## Setup
Make sure you've used the Arduino LIbrary manager to install U8G2LIB and SDFAT.

Build, Upload and Run.
## Output
The MCU should initialise the LCD panel and wait for you to insert and SD Card.

It will then list the contents (only enough entries to fill the screen)

It'll wait 5 seconds

Then it'll loop, displaying the state of any buttons.
