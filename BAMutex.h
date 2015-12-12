#ifndef __BAMUTEX_H__
#define __BAMUTEX_H__
#include "BAUtility.h"
#include <map>
using std::map;
class BAMutex {
private:
	map<string, int> _mutexvec;
	static BAMutex* _pinstance;
	BAMutex(){}
	BAMutex(const BAMutex& x) = delete;
	BAMutex(BAMutex&& x) = delete;
	BAMutex& operator=(const BAMutex& x) = delete;
	BAMutex& operator=(BAMutex&& x) = delete;
public:
	static BAMutex* GetInstance();
	inline int GetValue(string key){ return _mutexvec[key]; }
	void SetValue(string key, int value);
	void AddValue(string key);
};
#define BAMUTEX BAMutex::GetInstance()
#endif
