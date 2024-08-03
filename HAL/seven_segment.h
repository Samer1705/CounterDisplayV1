/******************************************************************************
 * Module: Seven Segment
 * File Name: seven_segment.h
 * Description: Header file for the Seven Segment driver
 * Author: Samer Sameh Lawindy
 *******************************************************************************/

#ifndef HAL_SEVEN_SEGMENT_H_
#define HAL_SEVEN_SEGMENT_H_

#include "../Common/std_types.h"
#include "../MCAL/gpio.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
enum {
	ZERO = 0x3F,
	ONE = 0x06,
	TWO = 0x5B,
	THREE = 0x4F,
	FOUR = 0x66,
	FIVE = 0x6D,
	SIX = 0x7D,
	SEVEN = 0x07,
	EIGHT = 0x7F,
	NINE = 0x6F
};
typedef enum{
	CATHODE, ANODE
}SegmentType;
typedef struct{
	GPIO_PortPinType* segmentPortPinIDs;
	uint8 digits;
	SegmentType segmentType;
	GPIO_PortPinType* enablePortPinIDs;
	uint32 value;
}SevenSegment;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void SEVEN_SEGMENT_Init(SevenSegment* segment);
void SEVEN_SEGMENT_Display(SevenSegment* segment);
void SEVEN_SEGMENT_SetValue(SevenSegment* segment, uint32 number);
uint32 SEVEN_SEGMENT_GetValue(SevenSegment* segment);
void SEVEN_SEGMENT_IncValue(SevenSegment* segment);
void SEVEN_SEGMENT_DecValue(SevenSegment* segment);
void SEVEN_SEGMENT_OFF(SevenSegment* segment);

#endif /* HAL_SEVEN_SEGMENT_H_ */
