# LED_Panels

Project to use the RGB LED panels donated to Syn Shop

The current code is based on a Teensy 3.2.  The project will need something much more powerful because the Teensy isn't able to update the display fast enough.  LED_Write_Buffer_Teensy demonstrates the problem.  The Teensy can't PWM the display fast enough to avoid flickering.

# Basics

The panels are 64 pixels wide and 16 pixels high.  Each pixel is an RGB LED.  They run on five volts and if they're all on they draw around three amps.  It looks like all of these panels are damaged in some way, either because pixels are stuck on or stuck off.  Most of the panels only have a few stuck pixels.

Data is sent to the LED panels using two SPI-style clocked serial data lines (D2 and D1), one for the top eight rows and one for the bottom eight rows.  The eight rows are divided into four groups of two rows each, interlaced as follows: A, B, C, D, A, B, C, D.  The active lines are selected by bringing the A0 and A1 pins high.  If both A0 and A1 are high group A is active, if only A1 is high group B is active, if only A0 is high group C is active, and if both A0 and A1 are low than group D is active.  If a group is active on the top half of the display it is also active on the bottom half of the display.  Only four rows are lit at any time, and the control hardware has to make sure the active lines are switched often enough that it looks like all the rows are lit up at the same time.

The OE pin is an inverted output enable, meaning the pin should be low when the display is on.  LAT is a latch, just like on a shift register. Pulse LAT to latch the data after loading is complete. Data is shifted in from right to left and top to bottom, meaning the first bit sent starts at the top left and ends up at the bottom right when the entire display is full.  Because all of the data should be shifted in and latched before enabling the display the data is sent starting with the bottom right pixel and moving right to left and bottom to top.  To make things even more confusing, two rows of sixteen pixels each needs to be sent for each of the three colors in turn before the next block of pixels is sent.  Hopefully my sample code makes it clearer.

# Goals

Port code to an FPGA.  Because all three colors have to be sent on the same data line and the pixels have to be both PWMed for relative brightness and updated often enough to avoid flickering there's no way a microcontroller can handle more than one panel. So far I can't even get the Teensy to update one panel fast enough, but I'm not using hardware SPI or DMA.  It may be possible to use hardware SPI since there are two software-selectable SPI pins on the Teensy.  Feel free to give it a shot.
