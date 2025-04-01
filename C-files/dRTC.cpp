#include "dRTC.h"
#include "DRot.h"
#include <stdio.h>
#include <MKL16Z4.h>
#include "DLED.h"

dRTC::dRTC() // constructor
{
	edit = false; // edit mode off by default
	timeTypeEdit = true; // hour mode by default
	userHour = 0;
	userMinute = 0;
}

void dRTC::Init()
{
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK; // enable PORTD for buttons
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; // enable PORTE for buttons
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK; // enable PTC1
	SIM->SCGC6 |= SIM_SCGC6_RTC_MASK; // enabling software access and interrupts for RTC
	SIM->SOPT1 = (SIM->SOPT1 & ~SIM_SOPT1_OSC32KSEL_MASK) | SIM_SOPT1_OSC32KSEL(0b10); // selecting RTC_CLKIN
	
	
	PORTC->PCR[1] = (PORTC->PCR[1] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(1); // selecting alt 1
	PORTD->PCR[1] = (PORTD->PCR[1] & ~(PORT_PCR_MUX_MASK | PORT_PCR_PS_MASK| PORT_PCR_IRQC_MASK)) | PORT_PCR_MUX(0b001) | PORT_PCR_IRQC(0b1010)  | PORT_PCR_PS_MASK; // select GPIO functionality on PTD1, enable pull-up/down, select pull-down
	PORTE->PCR[20] = (PORTE->PCR[20] & ~(PORT_PCR_MUX_MASK | PORT_PCR_PS_MASK)) | PORT_PCR_MUX(0b001) | PORT_PCR_PE_MASK |  PORT_PCR_PS_MASK; // select GPIO functionality on PTE20, enable pull-up/down, select pull-down
	PORTE->PCR[21] = (PORTE->PCR[21] & ~(PORT_PCR_MUX_MASK | PORT_PCR_PS_MASK)) | PORT_PCR_MUX(0b001) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; // select GPIO functionality on PTE21, enable pull-up/down, select pull-down
	
	PORTE->PCR[23] = (PORTE->PCR[22] &~PORT_PCR_MUX_MASK)|PORT_PCR_MUX(1);	// LED that flashes every second 
	PORTE->PCR[22] = (PORTE->PCR[23] &~PORT_PCR_MUX_MASK)|PORT_PCR_MUX(1);	// LED that is turned on and off during set time
	PTE->PDDR |= 0xC00000;
	// Default is input, so no need to set PDDR for buttons
	
	RTC->IER |= RTC_IER_TSIE_MASK; // enable time second interrupts
	
	RTC->TSR = 0; // set timer to initial value (zero or other)
	RTC->SR |= RTC_SR_TCE_MASK; // enable timer
	
	NVIC_EnableIRQ(RTC_Seconds_IRQn);
	NVIC_EnableIRQ(PORTC_PORTD_IRQn);
}


// setTime needs to be a loop which is active once edit button is pressed and until its pressed again
// while its high it constantly updates class variable time according to user inputs
void dRTC::editTime()
{
	userHour = RTC->TSR/3600;
	userMinute = (RTC->TSR % 3600) / 60;
	
	PTE->PSOR |= 0x800000;
	g_DLED.LEDBrightness(20);
	while (edit)
	{
		if (timeTypeEdit) // edit hours
		{
			userHour = g_DRot.Get(); // set userHour to encoder value constantly until mode is changed or edit mode is exited
			if (userHour > 23)
				userHour = 23;
		}
		else // edit minute
		{
			userMinute = g_DRot.Get(); // set userMinute to encoder value constantly until mode is changed or edit mode is exited
		}
		
		time = (userHour*100) + userMinute; // set time variable so clockface can read it and send it while editing
	} // while (edit)
	RTC->SR &= ~RTC_SR_TCE_MASK; // disable timer
	RTC->TSR = userHour*3600 + userMinute*60; // update RTC with new time
	RTC->SR |= RTC_SR_TCE_MASK; // enable timer
	PTE->PCOR |= 0x800000;
	g_DLED.LEDBrightness(100);
} // function

int dRTC::getTime()
{
	int temp = RTC->TSR;
	int hour;
	int minute;
	
	if (edit)
		return time;
	else
	{
		hour = temp/3600; // set hours
		minute = (temp % 3600) / 60; // set minutes
		return (100*hour + minute);
	}
}

int dRTC::getRTC()
{
	return RTC->TSR;
}

void dRTC::editToggle()
{
	g_RTC.edit = !g_RTC.edit;
}


void dRTC::timeTypeToggle()
{
	timeTypeEdit = !timeTypeEdit;
}