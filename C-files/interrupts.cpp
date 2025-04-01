
#include <MKL16Z4.h>
#include "DUART.h"
#include "DADC.h"
#include "POVDisplay.h"
#include "DRot.h"
#include "dRTC.h"
#include <stdio.h>

volatile int debounceCounter1 = 5000; // for debouncing button press detection in PIT
volatile int debounceCounter2 = 5000; // for debouncing button press for edit mode on PTD1

extern "C" {
	void PIT_IRQHandler ()
	{
		g_POVDisplay.Display();
		if((!(PTE->PDIR & (1<<21))) && (debounceCounter1 >= 5000))
		{
			g_RTC.timeTypeToggle();
			debounceCounter1 = 0;
		}
		debounceCounter1++; // increment for PTE21
		debounceCounter2++; // increment for PTD1
		
		if (debounceCounter2 > 1000000)
			debounceCounter2 = 0;
		
		if (debounceCounter1 > 1000000)
			debounceCounter1 = 0;
		
		//PTE->PTOR |= 0x400000;
		PIT->CHANNEL[0].TFLG = 0b1;// PIT FLag reset if inturput is on
		
	} // PIT_IRQHandler
	void PORTA_IRQHandler()
  {
        if (PORTA->ISFR & (1 << 4))
            g_DRot.Turn();
        else if (PORTA->ISFR & (1 << 5))
            g_DRot.Turn();

				// Sends time when rotary encoder is spun so that it can be read normally from a terminal
        char message[7];
        snprintf(message, 7, "%04d\r\n", g_RTC.getTime());
        g_DUART.Send(message, 7);

        PORTA->ISFR |= (1 << 4); // PTA4
        PORTA->ISFR |= (1 << 5); // PTA5
  }
		
	void RTC_Seconds_IRQHandler()
	{
		if ( ((RTC->TSR/20)%2) ==0)
			g_POVDisplay.setDisplayTime(g_RTC.getTime());
		else
			g_POVDisplay.setDisplayTemp(g_DADC.GetTemperature());
		
    if (RTC->TSR >= 86400) // if time is or is past 24:00
    {
        RTC->SR &= ~RTC_SR_TCE_MASK; // disable timer
        RTC->TSR = 0; // set back to zero
        RTC->SR |= RTC_SR_TCE_MASK; // enable timer
    }
	}// end if RTC seconds
	
	void PORTC_PORTD_IRQHandler()
	{
		const uint32_t PTD1_MASK = (1 << 1);
		if ((PORTD->ISFR & PTD1_MASK) && (debounceCounter2 > 5000)) // if PTD1 is high
		{
			g_RTC.editToggle();
			debounceCounter2 = 0;
		}
		PORTD->ISFR = (PORTD->ISFR & PTD1_MASK);
	}// PORTC_PORTD_IRQHandler

	void UART0_IRQHandler()// Uart0 handler for Serical comunication
	{		
		//PTE->PTOR |= 0x800000; Debug LED
		if(UART0->S1 &(UART0_S1_TDRE_MASK))
		{
			g_DUART.transmit(); // if TX flag is on will go to the transmit handler in the DUART driver
		}
		if(UART0->S1 &(UART0_S1_RDRF_MASK))
		{
			g_DUART.receive();// if RX flag is on will go to the receive handler in the DUART driver
		}
		UART0->S1 = 0xff;// Reset flags
		 
	}//void UART0_IRQHandler()
	
	void ADC0_IRQHandler()
	{
		//PTE->PTOR = 0x400000; //  Debug LED
		if (ADC0->SC1[0] & ADC_SC1_COCO_MASK) 
		{
			g_DADC.interruptADC();
		}// end if
	}//end void ADC0_IRQHandler()
} // extern C
