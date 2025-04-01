#ifndef DADC_H
#define DADC_H
#include <stdint.h>



class DADC
{
	public:
		DADC();
	
	//Public API
		void Init();
		int GetTemperature();
		void interruptADC();
	
	private:
		enum
     {
       NUM_THERMISTOR_TABLE_ENTRIES = 120,
       ADC_CHANNEL_THERMISTOR = 12,
       ADC_CHANNEL_LIGHT = 13,
     };
		int result;
		int hightemp;
		int lowtemp;
		static const uint16_t ThermistorValues[NUM_THERMISTOR_TABLE_ENTRIES];
		DADC(const DADC&);
		void operator=(const DADC&);
		
		
		
};
extern DADC g_DADC;
#endif  // DUART_H