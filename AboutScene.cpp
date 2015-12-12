#include "AboutScene.h"
#include "WelcomeScene.h"
USING_NS_CC;
Scene* AboutScene::createScene()
{
	auto scene = Scene::create();
	auto layer = AboutScene::create();
	scene->addChild(layer);
	return scene;
}


bool AboutScene::init() {
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//获取手机可视屏原点的坐标
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
{
	auto bgsprite = Sprite::create("NormalBG.jpg");
	bgsprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(bgsprite, 0);
}
	
	{
		auto sbgsprite = Sprite::create("About_word.png");
		sbgsprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 18));
		this->addChild(sbgsprite, 0);
	}
	

	float tbasey = 20.0f;
	{
		auto returnItem = MenuItemImage::create(
			"ReturnNormal.png",
			"ReturnSelected.png",
			CC_CALLBACK_1(AboutScene::OnReturnPressed, this));

		returnItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + returnItem->getContentSize().height / 2 + tbasey));

		// create menu, it's an autorelease object
		auto menu = Menu::create(returnItem, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);
	}
	return true;
}

void AboutScene::OnReturnPressed(Ref* pSender)
{
	Director::getInstance()->replaceScene(WelcomeScene::createScene());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}