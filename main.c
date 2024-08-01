/*
 * main.c
 *
 *  Created on: Jul 30, 2024
 *      Author: samer
 */
#include "HAL/seven_segment.h"
#include "MCAL/gpio.h"
#include "MCAL/uart.h"
#include "util/delay.h"

uint8 segmentEnablePins[2] = {PIN0_ID, PIN1_ID};
uint8 data = 0;
SevenSegment segment = {
		PORTC_ID,
		2,
		PORTB_ID,
		&segmentEnablePins,
};

UART_ConfigType uartConfig = {
		UART_9BIT,
		PARITY_DISABLE,
		UART_1_STOP_BIT,
		9600
};

void RXHandler();

int main(){
	UART_init(&uartConfig);
	SEVEN_SEGMENT_Init(&segment);
	for(;;)
	{
		SEVEN_SEGMENT_Display(&segment, 534);
	}
}

void RXHandler()
{
	data = UART_receiveByte();
}
