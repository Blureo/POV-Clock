
#include "DLED.h"
#include <MKL16Z4.h>
#include <stdint.h>

DLED::DLED()
{
}
void DLED::Init()
{
	// Sets up and turning on D,C, and B port which is located in the SCGC5
	SIM->SCGC5 |= (SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK| SIM_SCGC5_PORTB_MASK);

	// Two for loops so that the MUX Bit PORT D and C PCR to alt 1, which is GPIO
	for (int i=2; i<12; i ++)
	{
		PORTC->PCR[i] = (PORTC->PCR[i] &~PORT_PCR_MUX_MASK)|PORT_PCR_MUX(1); 
  }
	for (int i=2; i<8; i ++)
	{
		PORTD->PCR[i] = (PORTD->PCR[i] &~PORT_PCR_MUX_MASK)|PORT_PCR_MUX(1); 
  }
	
	//Port B0 PWM/ Dimming setup
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	PORTB->PCR[0] = (PORTB->PCR[0] &~PORT_PCR_MUX_MASK)|PORT_PCR_MUX(3); // Setting up Port B0 alt and TPM 
	
	SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK; // Clock setup for TPM1 so we get a 48MHz clock
	SIM->SOPT2 |= (SIM->SOPT2 &~SIM_SOPT2_TPMSRC_MASK) |SIM_SOPT2_TPMSRC(1);
	
	TPM1->SC |= (TPM1->SC &~TPM_SC_CMOD_MASK)| TPM_SC_CMOD(1); // Setting the TPM1 to count up
	TPM1->MOD = 139;// Setting mod so we get a 300KHz dudty cycle
	TPM1->CNT = 0; //Setting hold value to 0
	TPM1->CONTROLS[0].CnV = 5; // Preset value for compare value that can be changed with LEDBrightness
	TPM1->CONTROLS[0].CnSC |= TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK; // Configureing PMW to Edge-Aligned low pulse
	
	//Seting the needed pins in port C and D into output pins
	PTC->PDDR |= 0xffc;
  PTD->PDDR |= 0xfc;
	
}
void DLED::LEDBrightness(int input)
	// Will set brightness of LED by takeing a input of 0 to 100 and then going thorugh a process so that CnV can be changed easliy
	// With 100 being max and 0 being nothing
	// No return values
{
	TPM1->CONTROLS[0].CnV =  (5*input)/3;
}

void DLED::BlinkingLED ()
	// Cyles the leds turn on and off down and then up the POV355 borad. Takes not arguments as it is set pattern.
	// No return values
{
	uint16_t num = 0x8000; // uint_16 value for the 16 LEds
	for (volatile int i=0; i<=14; i ++)//For loop to turn 16 to 0 LEDs on and then off
	{ 
		int Cset = (num & (~Cmask))<< 2;
		int Dset = num >>8;
		PTC->PDOR |= Cset; //Cset on
		PTD->PDOR |= Dset; //Dset on 
		PTC->PDOR &= Cset; //Cset off
		PTD->PDOR &= Dset; //Dset off
		for ( int i = 0; i< 100000; i++) {} // delay to show progress throught leds so it is not a flash
		num = num >>1; // shift the uint16 value down the so the next LED is next
	}
	
	for (volatile int i=0; i<=14; i ++)
	{ // Same For loop but to turn 0 to 16 LEDs on and then off
		int Cset = (num & (~Cmask))<< 2;
		int Dset = num >>8;
		PTC->PDOR |= Cset; //Cset on 
		PTD->PDOR |= Dset; //Dset on 
		PTC->PDOR &= Cset; //Cset off
		PTD->PDOR &= Dset; //Dset off
		for ( int i = 0; i< 100000; i++) {}
		num = num <<1;	
	}
}// End BlinkingLED

void DLED::UpdateLED(uint16_t pattern)
	// To set a pattern with the POV355 board useing a 16 bit number. It takes a 16 bit number and will seperate the bits with 15-10 
	// to the D port and the 9-0 to the C port.  
{
	PTC->PDOR &= 0; //Cset off
	PTD->PDOR &= 0; //Dset ff
	int Cset = (pattern & (~Cmask))<< 2;
	int Dset = pattern >>8;
	PTC->PDOR |= Cset; //Cset on 
	PTD->PDOR |= Dset; //Dset on
	
}
void DLED::BlinkingLEDv3 () // For Fune
	// Cyles the leds turn on and off down and then up the POV355 borad. Takes not arguments as it is set pattern.
	// No return values
{
	uint16_t num = 0xE000; // uint_16 value for the 16 LEds
	for (volatile int i=0; i<=12; i ++)//For loop to turn 16 to 0 LEDs on and then off
	{ 
		int Cset = (num & (~Cmask))<< 2;
		int Dset = num >>8;
		PTC->PSOR |= Cset; //Cset on
		PTD->PSOR |= Dset; //Dset on 
		for ( int i = 0; i< 200000; i++) {} // delay to show progress throught leds so it is not a flash
		PTC->PCOR |= Cset; //Cset off
		PTD->PCOR |= Dset; //Dset off
		
		num = num >>1; // shift the uint16 value down the so the next LED is next
	}
	
	for (volatile int i=0; i<=12; i ++)
	{ // Same For loop but to turn 0 to 16 LEDs on and then off
		int Cset = (num & (~Cmask))<< 2;
		int Dset = num >>8;
		PTC->PSOR |= Cset; //Cset on
		PTD->PSOR |= Dset; //Dset on 
		for ( int i = 0; i< 200000; i++) {}
		PTC->PCOR |= Cset; //Cset off
		PTD->PCOR |= Dset; //Dset off
		
		num = num <<1;	
	}
}