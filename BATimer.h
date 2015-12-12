#ifndef __BCTIMER_H__
#define __BCTIMER_H__
typedef unsigned long BULONG;
class BCTimer {
private:
	BULONG _start, _end;
	BCTimer(){}
	BCTimer(const BCTimer& x) = delete;
	BCTimer(BCTimer&& x) = delete;
	BCTimer& operator=(const BCTimer& x) = delete;
	BCTimer& operator=(BCTimer&& x) = delete;
public:
	static BCTimer* _pinstance;
	static BCTimer* GetInstance();
	BULONG MGetTickCount();
	void Start();
	void End();
	BULONG Result();
};
#define BCTIMER BCTimer::GetInstance()
#endif