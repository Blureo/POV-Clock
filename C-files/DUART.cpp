
#include "DUART.h"
#include <MKL16Z4.h>
#include <stdint.h>

DUART::DUART()
{
}

void DUART::Init()
	// Initationzation of the DUART driver set up of corret registiors 
 // not reutnr vlaues
{
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;// turning on the clock of PortA and Uart0 so they cna be used
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	
	SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK; // Clock set up so that you get a Clock of 46MHz
	SIM->SOPT2 |= (SIM->SOPT2 &~SIM_SOPT2_UART0SRC_MASK) |SIM_SOPT2_UART0SRC(1);
	
	UART0->BDH |= 0x1; // Set the Baud Rate of 9600 
	UART0->BDL |= 0x11;
	
	UART0->C4  |= 0xF; // Seting Over Sampling Rate which is use to set Baud Rate  as Br = Bc/((OSR+1)*BRm)
	UART0->C2  |=  (UART_C2_RIE_MASK | UART_C2_RE_MASK | UART_C2_TE_MASK); // Enalbes the RX and TX and the RX intrreput
	
	PORTA->PCR[1] = (PORTA->PCR[1] &~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(2); // Seting the PortA for Rx and Tx
	PORTA->PCR[2] = (PORTA->PCR[2] &~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(2);
	NVIC_EnableIRQ(UART0_IRQn); // Enables the NVIC Intrrupt the Uart0
	
}//void DUART::Init()

void DUART::transmit()
	// Transmite handler for UART to send a char over serial Port
// no return values 
{
	  //GPIOE->PTOR |= 0x400000; //  Debug LED
	
	if (g_buffCountTx > 0) // check to see if we are at the end of the message being transmited
	{
		UART0->D = g_buffTransmit[g_buffCountTx]; // Send the message in the TX buffer to the Uart0 D for transmit
		g_buffTransmit[g_buffCountTx] = 0; // set the current value to 0
		g_buffCountTx --; // go to the reduce the current coutn
	}
	else
	{
		UART0->D = g_buffTransmit[g_buffCountTx]; // Send the last char of the message
		g_buffTransmit[g_buffCountTx] = 0; // set that value to 0
		UART0->C2 &= ~UART_C2_TIE_MASK; // Set inturrpet flag to 0
	}

}//void DUART::transmit()

void DUART::receive()
	// RX handler for Uart to recive message to send
// no return values
{
	if (g_buffCountRx <= 100) // check to see if count is under the char limit
	{
		uint8_t income = UART0->D; // Temp value to use in handler
		if ( income == 0x0d) // check for end charater for windows enter of 13
		{
			g_buffReciver[g_buffCountRx] = income; // copys the income char into the reciver bugg into the count of Rx
			g_buffCountRx += 1; // incresse the book mark
			for(int i = 0; i < g_buffCountRx; i ++) // for loop to enter the next char in message
			{
				if ((g_buffReciver[i] > 96) && (g_buffReciver[i] < 123))// if char is a English alphebet
				{
					g_buffReciver[i] = g_buffReciver[i]-32;// Convertin to lower case to unper case
				}
			}
			Send(g_buffReciver,g_buffCountRx); // Send message 
			g_buffCountRx = 0;// rset buff count for next message
		}
		else
		{
			g_buffReciver[g_buffCountRx] = income;  // Enter the end chater
			g_buffCountRx += 1; // add to count
		}
	}
}

bool DUART::Send(char *buf, unsigned len)
	//Fuction to send the message and message lenght and tigger transmit interurrpt
// Returns a bool if message was reveiced or not
{
	//All this function does is put our message in the buffer and return true
	if ((len <= 100)| (g_busy == false) ) // Check to see if busy or if the lenght to big
	{
		g_buffCountTx = len;// set the buff len to of the message
		for(unsigned i=0; i <len; i++)// for loop to put message in the the transmite 
		{
			g_buffTransmit[i] = buf[len-i-1];
		}
		g_busy = true;// setting busy flag
		
		UART0->C2 |= UART_C2_TIE_MASK;// Set up the transmit inturrput
		g_busy = false;// busy flag about
		return true; // message was able to transmit
	}
	else
	{
		return false; // mesage could not be transmitted
	}
	
}//bool DUART::Send(char *buf, unsigned len)