/******************************************************************************
 * Module: Seven Segment
 * File Name: seven_segment.c
 * Description: Source file for the Seven Segment driver
 * Author: Samer Sameh Lawindy
 *******************************************************************************/
#include "seven_segment.h"
#include "../MCAL/gpio.h"
#include <util/delay.h>

uint8 segmentNumbers[] = { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE };

static void splitNumber(uint32 number, uint8 digits, uint8 *digitsArray)
{
	uint8 digitsIndex;
	for(digitsIndex = 0; digitsIndex < digits; digitsIndex++)
	{
		digitsArray[digitsIndex] = number % 10;  //split last digit from number
		number = number / 10;    //divide num by 10. num /= 10 also a valid one
	}
}

void SEVEN_SEGMENT_Init(SevenSegment* segment)
{
	uint8 digitsIndex;
	GPIO_setupPortDirection(segment->segmentPortID, PORT_OUTPUT);
	for(digitsIndex = 0; digitsIndex < segment->digits; digitsIndex++)
	{
		GPIO_setupPinDirection(segment->enablePortID, segment->enablePinID[digitsIndex], PIN_OUTPUT);
		GPIO_writePin(segment->enablePortID, segment->enablePinID[digitsIndex], LOGIC_HIGH);
	}

}
void SEVEN_SEGMENT_Display(SevenSegment* segment, uint32 number)
{
	uint8 digitsIndex;
	uint8 digitsArray[8];
	splitNumber(number, segment->digits, digitsArray);
	for(digitsIndex = 0; digitsIndex < segment->digits; digitsIndex++)
	{
		GPIO_writePin(segment->enablePortID, segment->enablePinID[digitsIndex], LOGIC_LOW);
		GPIO_writePort(segment->segmentPortID, segmentNumbers[digitsArray[digitsIndex]]);
		_delay_ms(1);
		GPIO_writePin(segment->enablePortID, segment->enablePinID[digitsIndex], LOGIC_HIGH);
	}
}
void SEVEN_SEGMENT_OFF(SevenSegment* segment)
{
	GPIO_writePort(segment->segmentPortID, 0);
}
