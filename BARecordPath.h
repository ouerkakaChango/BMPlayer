#ifndef __BARECORDPATH_H__
#define __BARECORDPATH_H__
#include "BAUtility.h"
class BARecordPath {
private:
	string _path="",_json="";
	int _modeid=-1,_page=-1;
	BARecordPath(){}
	BARecordPath(const BARecordPath& x) = delete;
	BARecordPath(BARecordPath&& x) = delete;
	BARecordPath& operator=(const BARecordPath& x) = delete;
	BARecordPath& operator=(BARecordPath&& x) = delete;

	static BARecordPath* _pinstance;
public:
	static BARecordPath* GetInstance();
	inline void SetPath(string path){ _path = path; }
	inline string GetPath(){ return _path; }
	inline void SetModeID(int i){ _modeid = i; }
	inline int GetModeID(){ return _modeid; }
	inline void SetJson(const string& json){ _json = json; }
	inline string GetJson(){ return _json; }
	inline void SetPage(int page){ _page = page; }
	inline int GetPage(){ return _page; }
};
#define BAGAMELOADER BARecordPath::GetInstance()
#endif

