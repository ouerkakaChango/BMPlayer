#ifndef __WEBSCENE_H__
#define __WEBSCENE_H__
#include "BAUtility.h"
#include "cocos2d.h"
#include "json.h"
//#include "cocos-ext.h"
//using namespace cocos2d::extension;
class WebScene : public cocos2d::Layer{
private:

public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	//virtual void update(float delta);
	void OnReturnPressed(cocos2d::Ref* pSender);
	void OnSelectPressed(cocos2d::Ref* pSender);
	void OnPrePressed(cocos2d::Ref* pSender);
	void OnNextPressed(cocos2d::Ref* pSender);
	void OnGoPressed(cocos2d::Ref* pSender);

	vector<string> _infovec;
	vector<string> GetPageInfo(int page);
	int GetPageNum();
	void InitInfo();

	int _edgex,_edgey;
	int _page;

	Json::Reader _reader;
	Json::Value _root;

	float _nodex, _nodey;
	float _posx, _posy;
	bool IsAtInput(float x, float y);

	float _gonodex, _gonodey;
	float _goposx, _goposy;
	bool IsAtGo(float x, float y);
	virtual void update(float delta);
	bool _bereturn = false;

	CREATE_FUNC(WebScene);
};

#endif