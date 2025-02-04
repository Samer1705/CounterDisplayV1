 /******************************************************************************
 * Module: ATMEGA32 UART
 * File Name: atmega32_uart.c
 * Description: Source file for the UART AVR driver
 * Author: Samer Sameh Lawindy
 *******************************************************************************/

#include "uart.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "../Common/common_macros.h" /* To use the macros like SET_BIT */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the addresses of the call back functions in the application */
static volatile void (*g_callBackPtrUDR)(void) = NULL_PTR;
static volatile void (*g_callBackPtrTX)(void) = NULL_PTR;
static volatile void (*g_callBackPtrRX)(void) = NULL_PTR;

/*******************************************************************************
 *                          ISR's Definitions                                  *
 *******************************************************************************/
ISR(USART_UDRE_vect)
{
	if(g_callBackPtrUDR != NULL_PTR)
		{
			/* Call the Call Back function in the application after the edge is detected */
			(*g_callBackPtrUDR)(); /* another method to call the function using pointer to function g_callBackPtr(); */
		}
}

ISR(USART_TX_vect)
{
	if(g_callBackPtrTX != NULL_PTR)
		{
			/* Call the Call Back function in the application after the edge is detected */
			(*g_callBackPtrTX)(); /* another method to call the function using pointer to function g_callBackPtr(); */
		}
}

ISR(USART_RX_vect)
{
	if(g_callBackPtrRX != NULL_PTR)
		{
			/* Call the Call Back function in the application after the edge is detected */
			(*g_callBackPtrRX)(); /* another method to call the function using pointer to function g_callBackPtr(); */
		}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *config_Ptr)
{
	uint16 ubrr_value = 0;
	/* U2X = 1 for double transmission speed */
	UCSR0A = (1<<U2X0);
	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * RXB8 & TXB8 not used as we are not using 9-bit mode
	 ***********************************************************************/
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = configure parity as desired
	 * USBS    = configure stop bits as desired
	 * UCSZ1:0 = configure bit data as desired
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
	UCSR0C = (UCSR0C&0xCF)|((config_Ptr->parity)<<UPM00);
	UCSR0C = (UCSR0C&0xF7)|((config_Ptr->stop_bit)<<USBS0);
	UCSR0C = (UCSR0C&0xF9)|((config_Ptr->bit_data)<<UCSZ00);
	/* Calculate the UBRR register value */
	ubrr_value = (uint16)(((F_CPU / (config_Ptr->baud_rate * 8UL))) - 1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRR0H = ubrr_value>>8;
	UBRR0L = ubrr_value;
}
/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while(BIT_IS_CLEAR(UCSR0A,UDRE0));

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR0 = data;

	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)); // Wait until the transmission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	*******************************************************************/
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_receiveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSR0A,RXC0));

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
    return UDR0;
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}
	*******************************************************************/
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_receiveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_receiveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}

void UART_sendData(const uint8 *pData, uint32 uSize)
{
    uint32 uCounter;
    for(uCounter = 0; uCounter < uSize; uCounter++)
    {
        UART_sendByte(pData[uCounter]);
    }
}

void UART_receiveData(uint8 *pData, uint32 uSize)
{
    uint32 uCounter;
    for(uCounter = 0; uCounter < uSize; uCounter++)
    {
        pData[uCounter] = UART_receiveByte();
    }
}

void UART_interruptEnable(IntEn intType)
{
	UCSR0B &= 0x1F;
	UCSR0B |= (intType << UDRIE0);
}

void UART_setCallBackUDR(void(*a_ptr)(void))
{
	g_callBackPtrUDR = a_ptr;
}

void UART_setCallBackTX(void(*a_ptr)(void))
{
	g_callBackPtrTX = a_ptr;
}

void UART_setCallBackRX(void(*a_ptr)(void))
{
	g_callBackPtrRX = a_ptr;
}
