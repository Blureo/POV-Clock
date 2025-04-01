#ifndef POVDisplay_H
#define POVDisplay_H
#include <stdint.h>



class POVDisplay
{
	public:
		POVDisplay();
	
	//Public API
		void Init();
		void setDisplayTemp(int Temperature);
		void setDisplayTime(int Time);
		void Display();

	
	private:
		enum
     {
       NUM_FONT_INDEX_TABLE_ENTRIES = 16,
       NUM_FONT_TABLE_ENTRIES = 1485,
     };
		static const int16_t fontIndexPOV[NUM_FONT_INDEX_TABLE_ENTRIES];
		POVDisplay(const POVDisplay&);
		void operator=(const POVDisplay&);
		int g_displaybuff[200];
		static const uint16_t font[NUM_FONT_TABLE_ENTRIES];
		int bookmark = 0 ;
		int displaycounter = 0;
};
extern POVDisplay g_POVDisplay;
#endif  // DUART_H