/******************************************************************************
 * Module: Seven Segment
 * File Name: seven_segment.c
 * Description: Source file for the Seven Segment driver
 * Author: Samer Sameh Lawindy
 *******************************************************************************/
#include "seven_segment.h"
#include "../MCAL/gpio.h"
#include "../Common/common_macros.h"
#include <util/delay.h>

uint8 segmentCathodeNumbers[] = { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE };
uint8 segmentAnodeNumbers[] = { ~ZERO, ~ONE, ~TWO, ~THREE, ~FOUR, ~FIVE, ~SIX, ~SEVEN, ~EIGHT, ~NINE };

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
	uint8 digitsIndex, segmentIndex;
	for(segmentIndex = 0; segmentIndex < 7; segmentIndex++)
	{
		GPIO_setupPinDirection(segment->segmentPortPinIDs[segmentIndex].portID, segment->segmentPortPinIDs[segmentIndex].pinID, PIN_OUTPUT);
	}
	for(digitsIndex = 0; digitsIndex < segment->digits; digitsIndex++)
	{
		GPIO_setupPinDirection(segment->enablePortPinIDs[digitsIndex].portID, segment->enablePortPinIDs[digitsIndex].pinID, PIN_OUTPUT);
	}
}
void SEVEN_SEGMENT_Display(SevenSegment* segment)
{
	uint8 digitsIndex, segmentIndex;
	uint8 digitsArray[8];
	splitNumber(segment->value, segment->digits, digitsArray);
	for(digitsIndex = 0; digitsIndex < segment->digits; digitsIndex++)
	{
		if(segment->segmentType == CATHODE)
		{
			GPIO_writePin(segment->enablePortPinIDs[digitsIndex].portID, segment->enablePortPinIDs[digitsIndex].pinID, LOGIC_LOW);
			for(segmentIndex = 0; segmentIndex < 7; segmentIndex++)
			{
				GPIO_writePin(segment->segmentPortPinIDs[segmentIndex].portID, segment->segmentPortPinIDs[segmentIndex].pinID, GET_BIT(segmentCathodeNumbers[digitsArray[digitsIndex]], segmentIndex));
			}
			_delay_ms(5);
			GPIO_writePin(segment->enablePortPinIDs[digitsIndex].portID, segment->enablePortPinIDs[digitsIndex].pinID, LOGIC_HIGH);
		}
		else
		{
			GPIO_writePin(segment->enablePortPinIDs[digitsIndex].portID, segment->enablePortPinIDs[digitsIndex].pinID, LOGIC_HIGH);
			for(segmentIndex = 0; segmentIndex < 7; segmentIndex++)
			{
				GPIO_writePin(segment->segmentPortPinIDs[segmentIndex].portID, segment->segmentPortPinIDs[segmentIndex].pinID, GET_BIT(segmentAnodeNumbers[digitsArray[digitsIndex]], segmentIndex));
			}
			_delay_ms(5);
			GPIO_writePin(segment->enablePortPinIDs[digitsIndex].portID, segment->enablePortPinIDs[digitsIndex].pinID, LOGIC_LOW);
		}
	}
}
void SEVEN_SEGMENT_SetValue(SevenSegment* segment, uint32 number)
{
	segment->value =  number;
}
uint32 SEVEN_SEGMENT_GetValue(SevenSegment* segment)
{
	return segment->value;
}
void SEVEN_SEGMENT_IncValue(SevenSegment* segment)
{
	segment->value++;
}
void SEVEN_SEGMENT_DecValue(SevenSegment* segment)
{
	segment->value--;
}
void SEVEN_SEGMENT_OFF(SevenSegment* segment)
{
	uint8 digitsIndex;
	for(digitsIndex = 0; digitsIndex < segment->digits; digitsIndex++)
	{
		if(segment->segmentType == CATHODE)
		{
			GPIO_writePin(segment->enablePortPinIDs[digitsIndex].portID, segment->enablePortPinIDs[digitsIndex].pinID, LOGIC_HIGH);
		}
		else
		{
			GPIO_writePin(segment->enablePortPinIDs[digitsIndex].portID, segment->enablePortPinIDs[digitsIndex].pinID, LOGIC_LOW);
		}
	}
}
