
#include <MKL16Z4.h>
#include <stdint.h>
#include "DLED.h"
#include "DUART.h"
#include "DADC.h"
#include "POVDisplay.h"
#include "dRTC.h"
#include "DRot.h"
 
DUART g_DUART;
DLED g_DLED;
DADC g_DADC;
POVDisplay g_POVDisplay;
dRTC g_RTC;
DRot g_DRot;
	
int main ()
{ 
	g_DUART.Init();
	g_DLED.Init();
	g_DADC.Init();
	g_DRot.Init();
	g_RTC.Init();
	g_POVDisplay.Init();
	g_POVDisplay.setDisplayTime(g_RTC.getTime());
	
	g_DLED.LEDBrightness(100);

	
	while(1)
	{

		// Final Main
		
		if (g_RTC.edit)
		{
			g_RTC.editTime();
		}
		//end final main

	}

} 

