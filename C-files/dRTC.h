#ifndef dRTC_H
#define dRTC_H

#include <stdint.h>

class dRTC
{
public:
	
	dRTC();

	// Public API
	void Init();
	void editTime();
	int getTime(); // gets time variable
	int getRTC(); // gets seconds timer from RTC
	void editToggle(); // toggles edit mode on/off
	void timeTypeToggle(); // toggles timeTypeEdit
	int time; // 4 digit number which holds clockface time
	bool edit; // edit mode on/off  (true:false -> on:off)

private:
	bool timeTypeEdit; // change hours or minutes (true:false -> hour:minute)
	uint8_t userHour; // time variables for edit mode
	uint8_t userMinute; // time variables for edit mode
  dRTC(const dRTC&);
  void operator=(const dRTC&);
};

extern dRTC g_RTC;

#endif  // dRTC_H