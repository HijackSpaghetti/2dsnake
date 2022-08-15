#include <windows.h>


class clocker {
private:
	DWORD	lastclock;



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