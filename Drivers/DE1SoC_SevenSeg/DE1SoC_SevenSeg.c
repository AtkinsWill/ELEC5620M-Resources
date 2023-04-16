/*
 * DE1SoC_SevenSeg.c
 *
 *  Created on: 12 Feb 2021
 *  	Author: Harry Clegg
 *  Updated on: 12 Mar 2023
 *      Author: William Atkins
 */

#include "DE1SoC_SevenSeg.h"

// ToDo: Add the base addresses of the seven segment display peripherals.
volatile unsigned char *sevenseg_base_lo_ptr = (unsigned char *)0xFF200020;
volatile unsigned char *sevenseg_base_hi_ptr = (unsigned char *)0xFF200030;

// There are four HEX displays attached to the low (first) address.
#define SEVENSEG_N_DISPLAYS_LO 4

// There are two HEX displays attached to the high (second) address.
#define SEVENSEG_N_DISPLAYS_HI 2

void DE1SoC_SevenSeg_Write(unsigned int display, unsigned char value) {
    // Select between the two addresses so that the higher level functions
    // have a seamless interface.
    if (display < SEVENSEG_N_DISPLAYS_LO) {
        // If we are targeting a low address, use byte addressing to access
        // directly.
        sevenseg_base_lo_ptr[display] = value;
    } else {
        // If we are targeting a high address, shift down so byte addressing
        // works.
        display = display - SEVENSEG_N_DISPLAYS_LO;
        sevenseg_base_hi_ptr[display] = value;
    }
}

void DE1SoC_SevenSeg_SetSingle(unsigned int display, unsigned int value) {
    // ToDo: Write the code for driving a single seven segment display here.
    // Your function should turn a real value 0-F into the correctly encoded
    // bits to enable the correct segments on the seven segment display to
    // illuminate. Use the DE1SoC_SevenSeg_Write function you created earlier
    // to set the bits of the display.

	// Initialise the integer value that will be sent to the 7 segment display address.
	unsigned int seven_seg_bits;

	/** An example:
	     *
	     *    Integer input = 5
	     *    Shape | Lit LEDs
	     *    _________________
	     *      _	| LED 0
	     *     |	| LED 5
	     *      _	| LED 6
	     *       |	| LED 2
	     *      _ 	| LED 3
	     *      	|
	     *    Binary map values =	7,6,5,4,3,2,1,0
	     *    Binary map 		=	0,1,1,0,1,1,0,1
	     *    Integer value 	=	109
	     *    7 Segment Display "display" is assigned value 109 in order to visually produce the digit "5".
	     */

	// This switch statement maps each decimal value 0 - 15 to the 7 segment display value to illuminate the required LEDS
	// to visually display the characters 0x0 - 0xF.
	// An input value exceeding 15 is out of bounds, so a "-" is displayed by illuminating LED 6, in the middle of the display.
	switch(value) {
	  case 0:
		  seven_seg_bits = 63;
		  break;
	  case 1:
		  seven_seg_bits = 6;
	      break;
	  case 2:
		  seven_seg_bits = 91;
		  break;
	  case 3:
		  seven_seg_bits = 79;
		  break;
	  case 4:
		  seven_seg_bits = 102;
		  break;
	  case 5:
		  seven_seg_bits = 109;
		  break;
	  case 6:
		  seven_seg_bits = 125;
		  break;
	  case 7:
		  seven_seg_bits = 7;
		  break;
	  case 8:
		  seven_seg_bits = 127;
		  break;
	  case 9:
		  seven_seg_bits = 103;
		  break;
	  case 10:
		  seven_seg_bits = 119;
		  break;
	  case 11:
		  seven_seg_bits = 124;
		  break;
	  case 12:
		  seven_seg_bits = 57;
		  break;
	  case 13:
		  seven_seg_bits = 94;
		  break;
	  case 14:
		  seven_seg_bits = 121;
		  break;
	  case 15:
		  seven_seg_bits = 113;
		  break;
	  default:
		  seven_seg_bits = 64;
	}

	DE1SoC_SevenSeg_Write(display, seven_seg_bits);
}

void DE1SoC_SevenSeg_SetDoubleHex(unsigned int display, unsigned int value) {
    // ToDo: Write the code for setting a pair of seven segment displays
    // here. Good coding practice suggests your solution should call
    // DE1SoC_SevenSeg_SetSingle() twice.
    // This function should show the first digit of a HEXADECIMAL number on
    // the specified 'display', and the second digit on the display to
    // the left of the specified display.

    /** Some examples:
     *
     *    input | output | HEX(N+1) | HEX(N)
     *    ----- | ------ | -------- | ------
     *        5 |     05 |        0 |      5
     *       30 |     1E |        1 |      E
     *     0x60 |     60 |        6 |      0
     */

	// most_sig_hex is the left-most column of the 2 digit hexadecimal value. It has a value of 16 * digit.
	// least_sig_hex is the right-most column of the 2 digit hexadecimal value. It has a value of 1 * digit.
	// These values are initialised and calculated to convert the input decimal value into 2 digit hexadecimal.
	unsigned int most_sig_hex = value / 16;	   	// Integer division (/) returns the integer output, discarding the remainder.
	unsigned int least_sig_hex = value % 16; 	// Modulo (%) returns the integer remainder.

	// If the input decimal value exceeds 255 then the hexadecimal value would exceed FF, so should display "--".
	// Value 16 is not found in the DE1SoC_SevenSeg_SetSingle switch statement, so results in a "-" being displayed.
	// If not directly assigned in this manner, most_sig_hex would have a value > 15 so would display "-"
	// but least_sig_hex would still have a value between 0 and 15, and so would not display "-".
	if (value > 255) {
		most_sig_hex = 999;
		least_sig_hex = 999;
	}

	// DE1SoC_SevenSeg_SetSingle is called twice to display the 2 digit hexadecimal value.
	DE1SoC_SevenSeg_SetSingle(display+1, most_sig_hex); // Display the most significant hex on the left display, equal to "display+1".
	DE1SoC_SevenSeg_SetSingle(display, least_sig_hex); //Display the least significant hex on the right display, equal to "display".


}

void DE1SoC_SevenSeg_SetDoubleDec(unsigned int display, unsigned int value) {
    // ToDo: Write the code for setting a pair of seven segment displays
    // here. Good coding practice suggests your solution should call
    // DE1SoC_SevenSeg_SetSingle() twice.
    // This function should show the first digit of a DECIMAL number on
    // the specified 'display', and the second digit on the display to
    // the left of the specified display.

    /** Some examples:
     *
     *	  input | output | HEX(N+1) | HEX(N)
     *    ----- | ------ | -------- | ------
     *        5 |     05 |        0 |      5
     *       30 |     30 |        3 |      0
     *     0x90 |     96 |        9 |      6
     */

	// most_sig_dec is the left-most column of the 2 digit decimal value. It has a value of 10 * digit.
	// least_sig_dec is the right-most column of the 2 digit decimal value. It has a value of 1 * digit.
	// These values are initialised and calculated to convert the input decimal value into 2 individual decimal digits.
	unsigned int most_sig_dec = value / 10;	   	// Integer division (/) returns the integer output, discarding the remainder.
	unsigned int least_sig_dec = value % 10; 	// Modulo (%) returns the integer remainder.

	// If value exceeds 99 then it cannot be displayed by 2 decimal digits, so it should display "--" instead.
	// Without explicitly setting these values to exceed 15, the least significant decimal would continue to cycle 0-9 as it is
	// calculated from the modulo.
	if (value > 99) {
		most_sig_dec = 999; 	// Any value > 15 will display "-" when DE1SoC_SevenSeg_SetSingle is called.
		least_sig_dec = 999;
	}

	// DE1SoC_SevenSeg_SetSingle is called twice to display the 2 decimal digits.
	DE1SoC_SevenSeg_SetSingle(display+1, most_sig_dec); // Display the most significant decimal on the left display, equal to "display+1".
	DE1SoC_SevenSeg_SetSingle(display, least_sig_dec); //Display the least significant decimal on the right display, equal to "display".
}
