#ifndef __BAFIRETOCLEAR_H__
#define __BAFIRETOCLEAR_H__
#include "BAUtility.h"
class BAFireToClear {
private:
	static BAFireToClear* _pinstance;
	BAFireToClear(){}
	BAFireToClear(const BAFireToClear& x) = delete;
	BAFireToClear(BAFireToClear&& x) = delete;
	BAFireToClear& operator=(const BAFireToClear& x) = delete;
	BAFireToClear& operator=(BAFireToClear&& x) = delete;
public:
	static BAFireToClear* GetInstance();
	vector<IPOS> _vec;
	vector<IPOS> _dmanvec;
	inline void push(int x, int y){ _vec.push_back(IPOS(x, y)); }
	inline void push_dman(int x, int y){ _dmanvec.push_back(IPOS(x, y)); }
	void clear();
};

#define BAF2C BAFireToClear::GetInstance()
#endif
