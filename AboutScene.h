#ifndef __ABOUTSCENE_H__
#define __ABOUTSCENE_H__
#include "cocos2d.h"

class AboutScene : public cocos2d::Layer {
private:

public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	//virtual void update(float delta);
	void OnReturnPressed(cocos2d::Ref* pSender);


	CREATE_FUNC(AboutScene);
};
#endif