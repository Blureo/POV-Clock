
#include "DADC.h"
#include <MKL16Z4.h>
#include <stdint.h>

DADC::DADC()
{
}
const uint16_t DADC::ThermistorValues[NUM_THERMISTOR_TABLE_ENTRIES] = { // libary for ADC value to F with the frist being 1 degree and the last being 120 degrees
  59525,
  59339,
  59149,
  58952,
  58752,
  58547,
  58337,
  58122,
  57903,
  57679,
  57449,
  57216,
  56977,
  56734,
  56484,
  56230,
  55971,
  55708,
  55440,
  55166,
  54886,
  54602,
  54313,
  54019,
  53720,
  53416,
  53107,
  52794,
  52475,
  52151,
  51822,
  51488,
  51151,
  50808,
  50462,
  50111,
  49755,
  49394,
  49030,
  48662,
  48288,
  47909,
  47526,
  47142,
  46757,
  46365,
  45967,
  45568,
  45168,
  44763,
  44354,
  43941,
  43526,
  43109,
  42691,
  42272,
  41852,
  41429,
  41000,
  40569,
  40138,
  39709,
  39282,
  38851,
  38415,
  37979,
  37544,
  37105,
  36665,
  36228,
  35795,
  35367,
  34938,
  34503,
  34063,
  33622,
  33192,
  32768,
  32339,
  31911,
  31485,
  31061,
  30639,
  30219,
  29801,
  29384,
  28970,
  28560,
  28152,
  27746,
  27343,
  26944,
  26546,
  26151,
  25761,
  25374,
  24988,
  24607,
  24231,
  23858,
  23486,
  23119,
  22757,
  22398,
  22042,
  21690,
  21344,
  21002,
  20663,
  20325,
  19993,
  19665,
  19342,
  19023,
  18708,
  18397,
  18090,
  17787,
  17487,
  17193
};

void DADC::Init()
{
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK | SIM_SCGC6_PIT_MASK; // Enbaleing Clock for ADC and PIT and need ports
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;// turning on B port
	SIM->SOPT7 |= SIM_SOPT7_ADC0TRGSEL(0b1101) | SIM_SOPT7_ADC0ALTTRGEN_MASK ; //SIM_SOPT7_ADC0PRETRGSEL(0);// tigger on PIT 1 Channel and ADC channel A
	
	/*
	PIT->MCR = (PIT->MCR & ~PIT_MCR_MDIS_MASK); // PIT Setup 
	PIT->CHANNEL[1].LDVAL = 1390;// 24MHZ clock frequency
	PIT->CHANNEL[1].TCTRL = (PIT->CHANNEL[1].TCTRL & ~PIT_TCTRL_TEN_MASK) | PIT_TCTRL_TEN_MASK;// turn on counter 
	*/
	PORTB->PCR[2] = (PORTB->PCR[2] &~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(0);//ADC port B change
	
	ADC0->CFG1 |= ADC_CFG1_ADLPC_MASK | ADC_CFG1_ADLSMP_MASK | ADC_CFG1_MODE(3) | ADC_CFG1_ADICLK(3); // set low power mode, no extra clock divider, long sampling time, 16-bit conversions, and select the built-in async clock source
	ADC0->CFG2 &= ~(ADC_CFG2_ADACKEN_MASK | ADC_CFG2_ADHSC_MASK | ADC_CFG2_ADLSTS_MASK); //set the async clock to only be enabled during ADC conversions (not all the time), disable high-speed mode, and choose the longest possible sampling time
	ADC0->SC3 |= ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(11); //enable hardware averaging and set the number of samples averaged to 32
	
	ADC0->SC3 |= ADC_SC3_CAL_MASK; // Calibartion 
	while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK)) // busy while till calibration is done
	{
	}
		
	uint16_t tmp;
	//Calculate and store plus-side calibration
	tmp = ADC0->CLP0;
	tmp += ADC0->CLP1;
	tmp += ADC0->CLP2;
	tmp += ADC0->CLP3;
	tmp += ADC0->CLP4;
	tmp += ADC0->CLPS;
	tmp = (tmp >> 1) | 0x8000;
	ADC0->PG = tmp;
	//Calculate and store minus-side calibration
	tmp = ADC0->CLM0;
	tmp += ADC0->CLM1;
	tmp += ADC0->CLM2;
	tmp += ADC0->CLM3;
	tmp += ADC0->CLM4;
	tmp += ADC0->CLMS;
	tmp = (tmp >> 1) | 0x8000;
	ADC0->MG = tmp;
	
	ADC0->SC1[0] = ADC_SC1_ADCH(ADC_CHANNEL_THERMISTOR) | ADC_SC1_AIEN_MASK; // Setting pin and inturrpet
	ADC0->SC2 = ADC_SC2_ADTRG_MASK; // hardware tigger
	NVIC_EnableIRQ(ADC0_IRQn);// NVIC turn on
	
}// end init

int DADC::GetTemperature() 
	// Get priavte resutls when called and converts into int F
	// Int value of F
{
	for (int i =0; i < NUM_THERMISTOR_TABLE_ENTRIES; i++)
	{
		if( result >= ThermistorValues[i]) // goes thorugh and see if the rounding is closer to a higher of lower value of temp
		{
			hightemp = result - ThermistorValues[i];
			lowtemp  = result -  ThermistorValues[i-1];
			if (hightemp > lowtemp)
			{
				return i;
			}
			else
				return i-1;
		}
	}
	return 888;
};
void DADC::interruptADC()
	//when inturrpet called and changes reultss to most reslet value
{
	result = ADC0->R[0];	
}