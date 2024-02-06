# Four-mode Traffic Controller

DESCRIPTION:
Program that offers 4 modes of traffic control options via the DE2-115 board. These options are: basic traffic controller (Mode 1), traffic controller with pedestrian support (Mode 2), traffic controller configurable via UART (Mode 3), and traffic controller with camera (Mode 4).


PERIPHERALS USED:
This example exercises the following peripherals:
- UART
- GREEN_LEDS
- SWITCHES
- BUTTONS
- LCD

SOFTWARE SOURCE FILES:
This example includes the following software source files:
- hello_world.c
- mode2.h
- mode2.c
- mode3.h
- mode3.c
- mode4.h
- mode4.c

BOARD/HOST REQUIREMENTS:
- built for a DE2-115 development board with NIOSII programmed
- This program requires a USB to RS-232 connection for Mode 3 & 4.
