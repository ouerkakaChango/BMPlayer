#include "LocalScene.h"
#include "WelcomeScene.h"
#include "BAFileDlg.h"
#include "BARecordPath.h"
#include "HelloWorldScene.h"

USING_NS_CC;


Scene* LocalScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LocalScene::create();
	scene->addChild(layer);
	return scene;
}

bool LocalScene::init()
{

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
		auto titlesprite = Sprite::create("Local_title.png");
		titlesprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2+120));
		this->addChild(titlesprite, 0);
	}
	
	{
		auto sbgsprite = Sprite::create("Local_bgstory.png");
		sbgsprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2+18));
		this->addChild(sbgsprite, 0);
	}
	

	float tbasey = 20.0f, tinterval = 10.0f;
	{
		auto returnItem = MenuItemImage::create(
			"ReturnNormal.png",
			"ReturnSelected.png",
			CC_CALLBACK_1(LocalScene::OnReturnPressed, this));

		returnItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + returnItem->getContentSize().height / 2+tbasey));

		// create menu, it's an autorelease object
		auto menu = Menu::create(returnItem, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);
	}

	{
		auto selectItem = MenuItemImage::create(
			"SelectNormal.png",
			"SelectSelected.png",
			CC_CALLBACK_1(LocalScene::OnSelectPressed, this));

		selectItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + selectItem->getContentSize().height *1.5f+tbasey+tinterval));

		// create menu, it's an autorelease object
		auto menu = Menu::create(selectItem, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);
	}
	return true;
}

void LocalScene::OnReturnPressed(Ref* pSender)
{
	Director::getInstance()->replaceScene(WelcomeScene::createScene());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void LocalScene::OnSelectPressed(Ref* pSender)
{
	if(BAFILEDLG->OpenFile()){
	
		BAGAMELOADER->SetPath(wchar2char((wchar_t*)BAFILEDLG->FilePath()));
		BAGAMELOADER->SetModeID(2);
		Director::getInstance()->replaceScene(HelloWorld::createScene());
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}