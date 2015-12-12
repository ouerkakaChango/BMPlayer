#ifndef __WELCOMESCENE_H__
#define __WELCOMESCENE_H__
#include "cocos2d.h"


class WelcomeScene : public cocos2d::Layer {
private:

public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	//virtual void update(float delta);
	void OnQuitPressed(cocos2d::Ref* pSender);
	void OnAboutPressed(cocos2d::Ref* pSender);
	void OnLocalPressed(cocos2d::Ref* pSender);
	void OnWebPressed(cocos2d::Ref* pSender);


	CREATE_FUNC(WelcomeScene);
};
#endif