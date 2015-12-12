#ifndef __LOCALSCENE_H__
#define __LOCALSCENE_H__
#include "cocos2d.h"


class LocalScene : public cocos2d::Layer {
private:

public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	//virtual void update(float delta);
	void OnReturnPressed(cocos2d::Ref* pSender);
	void OnSelectPressed(cocos2d::Ref* pSender);


	CREATE_FUNC(LocalScene);
};
#endif