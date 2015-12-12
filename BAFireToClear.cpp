#include "BAFireToClear.h"
#include "BAMap.h"
BAFireToClear* BAFireToClear::_pinstance = nullptr;
BAFireToClear* BAFireToClear::GetInstance(){
	if (_pinstance == nullptr){
		_pinstance = new BAFireToClear;
	}
	return _pinstance;
}

void BAFireToClear::clear(){
	for (auto iter : _vec){
		BAMAP->RemoveFire(iter.first, iter.second);
	}
	_vec.clear();
	for (auto iter : _dmanvec){
		BAMAP->RemoveDirtMan(iter.first, iter.second);
	}
	_dmanvec.clear();
}