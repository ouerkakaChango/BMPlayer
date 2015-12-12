#ifndef __BAACTION_H__
#define __BAACTION_H__
#include "BAUtility.h"
#include "json.h"
#include <thread>
using std::thread;

class BAMsg{
private:
	string _fname;
	int _x, _y, _ex;
public:
	BAMsg(string fname, int x, int y, int ex) :_fname(fname), _x(x), _y(y), _ex(ex){}
	inline string GetFName(){ return _fname; }
	inline int GetX(){ return _x; }
	inline int GetY(){ return _y; }
	inline int GetEX(){ return _ex; }

};

class BAJsonRound{
public:
	vector<BAMsg> _msgvec;
	BAJsonRound(const Json::Value& root, int round);
};

void BFRealMainFunc(string fname, int x, int y, int ex);

class BAAction {
private:
	vector<thread> _threadvec;
	static BAAction* _pinstance;
	BAAction(){}
	BAAction(const BAAction& x) = delete;
	BAAction(BAAction&& x) = delete;
	BAAction& operator=(const BAAction& x) = delete;
	BAAction& operator=(BAAction&& x) = delete;
public:
	static BAAction* GetInstance();
	void PreRound(const BAJsonRound& bajr);
	void DoAction();
	inline int GetNum(){ return _threadvec.size(); }
};

#define BAACTION BAAction::GetInstance()




#endif
