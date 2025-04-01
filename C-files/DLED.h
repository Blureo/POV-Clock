#ifndef DLED_H
#define DLED_H
#include <stdint.h>



class DLED
{
public:
	DLED();
	//Public API
	void Init();
	void BlinkingLED();
	void BlinkingLEDv3();
	void UpdateLED(uint16_t pattern);
	void LEDBrightness(int input);
	
 
 private:
	uint16_t num = 0x8000; // uint_16 value for the 16 LEds
	int Cmask = 0xFC00; // Mask for C port
  DLED(const DLED&);
  void operator=(const DLED&);
};

extern DLED g_DLED;

#endif  // DLED_H