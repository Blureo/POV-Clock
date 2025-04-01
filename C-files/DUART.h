#ifndef DUART_H
#define DUART_H
#include <stdint.h>



class DUART
{
	public:
		DUART();
	
	//Public API
		void Init();
		void transmit();
		void receive();
		bool Send(char *buf, unsigned len);
	
	private:
		char g_buffTransmit[100]; // Buffer array for transmit
		int g_buffCountTx; // buffer count for transmit
		char g_buffReciver[100]; // buffer array for recive
		int g_buffCountRx = 0; // buffer count for recive
		bool g_busy;
		DUART(const DUART&);
		void operator=(const DUART&);
		
};
extern DUART g_DUART;
#endif  // DUART_H