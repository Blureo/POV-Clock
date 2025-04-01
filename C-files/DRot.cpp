#include <MKL16Z4.h>
#include "DRot.h"
#include <stdint.h>

DRot::DRot() // constructor
{
	incr = 0;
}
void DRot::Init(){
	SIM->SCGC5 = (SIM->SCGC5 &~SIM_SCGC5_PORTA_MASK)| 1 << SIM_SCGC5_PORTA_SHIFT; // This turns on clock for port A
	
	PORTA->PCR[4] = (PORTA->PCR[4] &~ PORT_PCR_MUX_MASK) | PORT_PCR_MUX(1); // enables pin 4 of port A to be PT
	PORTA->PCR[4] = (PORTA->PCR[4] &~ PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(0b1011); // enables interupt request on either a rising or falling edge of pin 4
	
	PORTA->PCR[5] = (PORTA->PCR[5] &~ PORT_PCR_MUX_MASK) | PORT_PCR_MUX(1); // enables pin 5 of port A to be PT
	PORTA->PCR[5] = (PORTA->PCR[5] &~ PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(0b1011); // enables interupt request on either a rising or falling edge of pin 5
	
	NVIC_EnableIRQ(PORTA_IRQn); // enables processing of interrupts generated from port A
	
	last_state = ((PTA->PDIR & INPUT_MASK) >> 4);
}

void DRot::Turn(){
	//current_state = ((PTA->PDIR & INPUT_MASK) >> 4);
	current_state = ((PTA->PDIR >> 4) & 1) | (((PTA->PDIR >> 5) & 1) << 1);
	
	if((current_state == 00) & (last_state == 01)){
		if (incr == 59)
			incr = 59;
		else
			incr++;
	}
	else if((current_state == 00) & (last_state == 10)){
		if (incr == 0)
			incr = 0;
		else
			incr--;
	}
	else if((current_state == 10) & (last_state == 00)){
		if (incr == 59)
			incr = 59;
		else
			incr++;
	}
	else if((current_state == 10) & (last_state == 11)){
		if (incr == 0)
			incr = 0;
		else
			incr--;
	}
  else if((current_state == 11) & (last_state == 10)){
		if (incr == 59)
			incr = 59;
		else
			incr++;
	}
	else if((current_state == 11) & (last_state == 01)){
		if (incr == 0)
			incr = 0;
		else
			incr--;
	}
	else if((current_state == 01) & (last_state == 11)){
		if (incr == 59)
			incr = 59;
		else
			incr++;
	}
	else if((current_state == 01) & (last_state == 00)){
		if (incr == 0)
			incr = 0;
		else
			incr--;
	}
	last_state = current_state;
}	

void DRot::Zero(){
	incr = 0;
}

int DRot::Get(){
	return incr;
}