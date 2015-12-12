#include "BARecordPath.h"

BARecordPath* BARecordPath::_pinstance = nullptr;
BARecordPath* BARecordPath::GetInstance(){
	if (_pinstance == nullptr){
		_pinstance = new BARecordPath;
	}
	return _pinstance;
}

