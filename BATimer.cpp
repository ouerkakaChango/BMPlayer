#include "BATimer.h"

BCTimer* BCTimer::_pinstance = nullptr;
BCTimer* BCTimer::GetInstance(){
	if (_pinstance == nullptr){
		_pinstance = new BCTimer;
	}
	return _pinstance;
}

#ifdef WIN32
#define OS_WINDOWS WIN32

#include <windows.h>
#endif
#ifdef LINUX
#include <unistd.h>
#include <sys/time.h>
#include <netinet/in.h>
#endif
#ifdef VXWORKS
#include "vxworks.h"
#include <tickLib.h>
#include <sysLib.h>
#endif

BULONG BCTimer::MGetTickCount()
{
	BULONG currentTime;
#ifdef WIN32
	currentTime = GetTickCount();
#endif
#ifdef LINUX
	struct timeval current;
	gettimeofday(&current, NULL);
	currentTime = current.tv_sec * 1000 + current.tv_usec / 1000;
#endif
#ifdef OS_VXWORKS
	ULONGA timeSecond = tickGet() / sysClkRateGet();
	ULONGA timeMilsec = tickGet() % sysClkRateGet() * 1000 / sysClkRateGet();
	currentTime = timeSecond * 1000 + timeMilsec;
#endif
	return currentTime;
}

void BCTimer::Start() {
	_start = MGetTickCount();
}

void BCTimer::End() {
	_end = MGetTickCount();
}

BULONG BCTimer::Result() {
	return _end - _start;
}