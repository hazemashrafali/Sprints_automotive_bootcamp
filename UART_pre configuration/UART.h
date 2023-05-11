/*
 * UART.h
 *
 *  Created on: May 11, 2023
 *      Author: HAZEM-PC
 */

#ifndef UART_H_
#define UART_H_

#include "avr/io.h"
#include "std_types.h"
#include "common_macros.h"
/*
 * if UART_Async_mode is defined in the code,the UART will use in Asynchronous operation
 * to use in synchronous operation remove UART_Async_mode
 */
#define UART_Async_mode
#ifdef UART_Async_mode
#define UART_mode UART_Async_mode_mask
#else
#define UART_mode UART_Sync_mode_mask
#endif

/*
 * UART parity mode configuration ,its value should be 0(disabled) or 1(even parity) or 2(odd parity)
 */
#define UART_parity_mode 1

#if (UART_parity_mode == 0)
#define UART_parity UART_parity_Disabled_mask
#elif (UART_parity_mode == 1)
#define UART_parity UART_parity_Even_mask
#elif (UART_parity_mode == 2)
#define UART_parity UART_parity_Odd_mask
#else
#error "Number of parity mode  should be equal to 0 or 1 or 2"
#endif

/*
 * if UART_one_stop_mode is defined in the code,the UART will use in one stop bit
 * to use two stop bit remove UART_one_stop_mode
 */

#define UART_one_stop_mode
#ifdef UART_one_stop_mode
#define UART_stop_mode UART_1_stopBit_mask
#else
#define UART_stop_mode UART_2_stopBit_mask
#endif


/*
 * if UART_clk_rising_polarity is defined in the code,the UART will use rising edge
 * to use failing remove UART_clk_rising_polarity
 */

#if(UART_mode == UART_Sync_mode_mask)
#define UART_clk_rising_polarity
#ifdef UART_clk_rising_polarity
#define UART_clk_polarity UART_clk_rising_mask
#else
#define UART_clk_polarity UART_clk_failing_mask
#endif
#endif


//define masking bits
#define UART_Async_mode_mask			0x00
#define UART_Sync_mode_mask				0x40
#define UART_parity_Disabled_mask		0x00
#define UART_parity_Even_mask			0x20
#define UART_parity_Odd_mask			0x30
#define UART_1_stopBit_mask				0x00
#define UART_2_stopBit_mask				0x04
#define UART_clk_rising_mask			0x00
#define UART_clk_failing_mask			0x01
#define UART_Receiver_Enable_mask		0x10
#define UART_Transmitter_Enable_mask	0x08

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(uint32 baud_rate);
void UART_sendByte(const uint8 data);
uint8 UART_recieveByte(void);
void UART_sendString(const uint8 *Str);
void UART_receiveString(uint8 *Str);
void UART_integerTostring(const uint8 data);


#endif /* UART_H_ */
