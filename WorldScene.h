#ifndef __WORLD_SCENE_H__
#define __WORLD_SCENE_H__

#include "cocos2d.h"
#include "json.h"
#include <string>
using std::string;


class HelloWorld : public cocos2d::Layer {
private:
	string _recordpath = "null";
	int _modeid = -1;
	Json::Reader _reader;
	Json::Value _root;
	float _interval = -1.0f;
	float _oriinterval = -1.0f;
	float _timetochange = -1.0f;
	int _nowround = -1;
	bool _iscontinue = true;
	bool _ispause = false;
	int _allmsgnum = 0;
public:
	static cocos2d::Scene* createScene();
	virtual void update(float delta);
	virtual bool init();
	void myupdate(float delta);
	//inline float GetInterval(){ return _interval; }

	void OnReturnPressed(cocos2d::Ref* pSender);

	CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
