#ifndef __BAWEBNODE_H__
#define __BAWEBNODE_H__
#include "BAUtility.h"
#include "cocos2d.h"
USING_NS_CC;

class BAWebNode{
private:
	FPOS _screenpos;
	string _info;
	//CCLabelAtlas* _plabelAtlas;
	
public:
	BAWebNode(FPOS screenpos, string info);
	inline FPOS GetScreenPos(){ return _screenpos; }
	inline string GetInfo(){ return _info; }
	//inline CCLabelAtlas* GetLabel(){ return _plabelAtlas; }
	Label* _label;
	Sprite* _psprite;
};
#endif
