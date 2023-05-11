 /******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.c
 *
 * Description: Source file for the Keypad driver
 *
 * Author: hazem ashraf
 *
 *******************************************************************************/
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "keypad.h"


/*******************************************************************************
 *                      Functions Prototypes(Private)                          *
 *******************************************************************************/

#if (KEYPAD_NUM_COLS == 3)
/*
 * Function responsible for mapping the switch number in the keypad to
 * its corresponding functional number in the Proteus for 4x3 keypad
 */
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number);
#elif (KEYPAD_NUM_COLS == 4)
/*
 * Function responsible for mapping the switch number in the keypad to
 * its corresponding functional number in the Proteus for 4x4 keypad
 */
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number);
#endif

volatile uint8 g_button_value='@';
volatile uint8 g_keypad_value;
volatile uint8 g_flag=0;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void  KEYPAD_getPressedKey(void)
{
	uint8 col,row;
	uint8 keypad_port_value = 0;

		for(col=0;col<KEYPAD_NUM_COLS;col++) /* loop for columns */
		{
			/* 
			 * Each time setup the direction for all keypad port as input pins,
			 * except this column will be output pin
			 */
			GPIO_setupPortDirection(KEYPAD_PORT_ID,PORT_INPUT);
			GPIO_setupPinDirection(KEYPAD_PORT_ID,KEYPAD_FIRST_COLUMN_PIN_ID+col,PIN_OUTPUT);
			
#if(KEYPAD_BUTTON_PRESSED == LOGIC_LOW)
			/* Clear the column output pin and set the rest pins value */
			keypad_port_value = ~(1<<(KEYPAD_FIRST_COLUMN_PIN_ID+col));
#else
			/* Set the column output pin and clear the rest pins value */
			keypad_port_value = (1<<(KEYPAD_FIRST_COLUMN_PIN_ID+col));
#endif
			GPIO_writePort(KEYPAD_PORT_ID,keypad_port_value);

			for(row=0;row<KEYPAD_NUM_ROWS;row++) /* loop for rows */
			{
				/* Check if the switch is pressed in this row */
				if(GPIO_readPin(KEYPAD_PORT_ID,row+KEYPAD_FIRST_ROW_PIN_ID) == KEYPAD_BUTTON_PRESSED)
				{
					//cli();
					g_button_value= KEYPAD_4x4_adjustKeyNumber((row*KEYPAD_NUM_COLS)+col+1);
					Timer_init_Normal_Mode();
					//g_flag=1;
					//sei();
				}
			}
		}
}

void Timer_init_Normal_Mode(void)
{
	TCNT0 = 200;		// Set Timer initial value to 0
	TIMSK = (1<<TOIE0); // Enable Timer0 Overflow Interrupt
	/* configure the timer
	 * 1. Non PWM mode FOC0=1
	 * 2. Normal Mode WGM01=0 & WGM00=0
	 * 3. Normal Mode COM00=0 & COM01=0 
	 * 4. clock = F_CPU/256 CS00=0 CS01=0 CS02=1
	 */
	TCCR0 = (1<<FOC0) | (1<<CS02);
}

uint8 Keypad_read(void)
{
	return g_keypad_value;
}

ISR(TIMER0_OVF_vect)
{
	g_keypad_value=g_button_value;
	TCNT0 = 200;
}

#if (KEYPAD_NUM_COLS == 3)

/*
 * Description :
 * Update the keypad pressed button value with the correct one in keypad 4x3 shape
 */
static uint8 KEYPAD_4x3_adjustKeyNumber(uint8 button_number)
{
	uint8 keypad_button = 0;
	switch(button_number)
	{
		case 10: keypad_button = '*'; // ASCII Code of *
				 break;
		case 11: keypad_button = 0;
				 break;		
		case 12: keypad_button = '#'; // ASCII Code of #
				 break;
		default: keypad_button = button_number;
				break;
	}
	return keypad_button;
} 

#elif (KEYPAD_NUM_COLS == 4)

/*
 * Description :
 * Update the keypad pressed button value with the correct one in keypad 4x4 shape
 */
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number)
{
	uint8 keypad_button = 0;
	switch(button_number)
	{
		case 1: keypad_button = 7;
				break;
		case 2: keypad_button = 8;
				break;
		case 3: keypad_button = 9;
				break;
		case 4: keypad_button = '%'; // ASCII Code of %
				break;
		case 5: keypad_button = 4;
				break;
		case 6: keypad_button = 5;
				break;
		case 7: keypad_button = 6;
				break;
		case 8: keypad_button = '*'; /* ASCII Code of '*' */
				break;		
		case 9: keypad_button = 1;
				break;
		case 10: keypad_button = 2;
				break;
		case 11: keypad_button = 3;
				break;
		case 12: keypad_button = '-'; /* ASCII Code of '-' */
				break;
		case 13: keypad_button = 13;  /* ASCII of Enter */
				break;			
		case 14: keypad_button = 0;
				break;
		case 15: keypad_button = '='; /* ASCII Code of '=' */
				break;
		case 16: keypad_button = '+'; /* ASCII Code of '+' */
				break;
		default: keypad_button = button_number;
				break;
	}
	return keypad_button;
} 

#endif
