# POV-Clock
Persistence-of-Vision LED Clock

This is an LED clock that makes use of persistence-of-vision to display time and temperature. The time can be set using the rotary encoder and 2 buttons (there are 3, but one was not used in the current iteration).

-------

This project was made in collaboration with 2 others: Joshua Sevick and Kolby Kriegelstein.

I, Jesse Thompson, was responsible for the PCB design, Real-time Clock (RTC), and implementing the rotary encoder's drivers in the RTC. All group members made their own drivers for the UART, ADC (for temperature), and bar of 16 LED's.


The design is based on the NXP MKL16Z MCU and was programmed in C++ using Keil uVision.

-------

## How to Use:
Time is set to zero by default. To set the time, press the edit button and use the rotary encoder to set hour and minute to desired number (switch between hour and minute mode using the hr/min button. If you experience difficulty seeing the time you are setting the clock to, connect a serial cable to the UART pins, and using an application such as PuTTY with a baud of 9600, you can see the time getting updated as you rotate the encoder. If using a terminal, connect the +5V pin. If using a PC, ***do not*** connect the +5V pin.

To see the time, flick your eyes across the LEDs. Moving one's head does not achieve the desired effect well. 

-------

## Current issues and future fixes
1. Edit mode spontaneously turns on despite the button not being pressed. Unsure at this time as to why this occurs. **Fix**: edit mode will only be on if the button is continuously pressed down.
