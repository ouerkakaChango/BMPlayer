#ifndef __BAWEBPAGE_H__
#define __BAWEBPAGE_H__
#include "BAUtility.h"
#include "cocos2d.h"
USING_NS_CC;

class WebScene;
class BAWebNode;
class BAWebPage{
private:
	int _edgex,_edgey;
	float _nodex, _nodey,_orx,_ory;
	float _height;
	string _selectedinfo = "";
	string _nodepath;

	BAWebPage(){}
	BAWebPage(const BAWebPage& x) = delete;
	BAWebPage(BAWebPage&& x) = delete;
	BAWebPage& operator=(const BAWebPage& x) = delete;
	BAWebPage& operator=(BAWebPage&& x) = delete;

	WebScene* _pscene=nullptr;

	static BAWebPage* _pinstance;
public:
	static BAWebPage* GetInstance();

	string GetRecordID(string nodestr);

	bool IsInSameNode(float x,float y);

	void update(int page,const vector<string>& infovec);

	void Clear();

	void init( int edgex,int edgey,float nodex, float nodey, string spritepath, float orx, float ory, float height,string nodepath,WebScene* pscene);

	inline string GetNodePath(){ return _nodepath; }
	inline WebScene* GetpScene(){ return _pscene; }
	inline string GetSelectedInfo(){ return _selectedinfo; }
	vector<BAWebNode> _vec;
	FPOS _lastpos;
	Sprite* _selectsprite;
};
#define BAWEBPAGE BAWebPage::GetInstance()

#endif

