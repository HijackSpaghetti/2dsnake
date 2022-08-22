#ifndef _CLOCKER_
#define _CLOCKER_

#include <windows.h>


class clocker {
private:
	ULONGLONG	lastclock;



public:
	clocker() {
		lastclock = GetTickCount64();
	};

	int millis() {
		int elapsed = (GetTickCount64() - lastclock);
		//lastclock = GetTickCount64();
		return elapsed;
	}
	int refresh()
	{
		lastclock = GetTickCount64();
		return 0;
	};



};

#endif