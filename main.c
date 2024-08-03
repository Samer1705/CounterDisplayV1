/*
 * main.c
 *
 *  Created on: Jul 30, 2024
 *      Author: samer
 */

#include "Common/std_types.h"
#include "HAL/seven_segment.h"
#include "MCAL/gpio.h"
#include "MCAL/uart.h"
#include <avr/io.h>

GPIO_PortPinType segmentEnablePins[3] = {
		{ PORTB_ID, PIN1_ID },
		{ PORTB_ID, PIN2_ID },
		{ PORTB_ID, PIN3_ID },
};
GPIO_PortPinType segmentPins[7] = {
		{ PORTC_ID, PIN0_ID },
		{ PORTC_ID, PIN1_ID },
		{ PORTC_ID, PIN2_ID },
		{ PORTC_ID, PIN3_ID },
		{ PORTC_ID, PIN4_ID },
		{ PORTC_ID, PIN5_ID },
		{ PORTB_ID, PIN0_ID },
};
SevenSegment segment = {
		segmentPins,
		3,
		ANODE,
		segmentEnablePins,
		000
};

UART_ConfigType uartConfig = {
		UART_8BIT,
		PARITY_DISABLE,
		UART_1_STOP_BIT,
		9600
};

void RXHandler();

int main(){
	/* Enable change of Interrupt Vectors */
	MCUCR = (1<<IVCE);
	/* Move interrupts to Boot Flash section */
	MCUCR = (1<<IVSEL);
	SREG |= (1<<7);
	UART_init(&uartConfig);
	UART_interruptEnable(RX_INT);
	UART_setCallBackRX(RXHandler);
	SEVEN_SEGMENT_Init(&segment);
	for(;;)
	{
		SEVEN_SEGMENT_Display(&segment);
	}
}

void RXHandler()
{
	uint8 data = UART_receiveByte();
	UART_sendByte(data);
	SEVEN_SEGMENT_SetValue(&segment, data);
	UART_sendByte(SEVEN_SEGMENT_GetValue(&segment));
}
