#include "BAMutex.h"

BAMutex* BAMutex::_pinstance=nullptr;
BAMutex* BAMutex::GetInstance(){
	if (_pinstance == nullptr){
		_pinstance = new BAMutex;
	}
	return _pinstance;
}

void BAMutex::SetValue(string key, int value){
	_mutexvec[key] = value;
}

void BAMutex::AddValue(string key){
	_mutexvec[key]++;
}