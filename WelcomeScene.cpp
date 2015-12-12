#include "WelcomeScene.h"
#include "LocalScene.h"
#include "WebScene.h"
#include "AboutScene.h"

USING_NS_CC;
Scene* WelcomeScene::createScene()
{
	auto scene = Scene::create();
	auto layer = WelcomeScene::create();
	scene->addChild(layer);
	return scene;
}

bool WelcomeScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//获取手机可视屏原点的坐标
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	///////////////////////////////////////////////////////////////////////
	float tbasey = 20.0f, tinterval = 10.0f;

	{
		auto bgsprite = Sprite::create("Welcome_bg.png");
		bgsprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
		this->addChild(bgsprite, 0);
	}

	{
		auto titlesprite = Sprite::create("Welcome_title.png");
		titlesprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 120));
		this->addChild(titlesprite, 1);
	}

	{
		auto closeItem = MenuItemImage::create(
			"QuitNormal.png",
			"QuitSelected.png",
			CC_CALLBACK_1(WelcomeScene::OnQuitPressed, this));

		closeItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + closeItem->getContentSize().height / 2+tbasey));

		// create menu, it's an autorelease object
		auto menu = Menu::create(closeItem, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);
	}

	{
		auto aboutItem = MenuItemImage::create(
			"AboutNormal.png",
			"AboutSelected.png",
			CC_CALLBACK_1(WelcomeScene::OnAboutPressed, this));

		aboutItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + aboutItem->getContentSize().height *1.5f + tbasey + tinterval));

		// create menu, it's an autorelease object
		auto menu = Menu::create(aboutItem, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);
	}
	{
		auto localItem = MenuItemImage::create(
			"LocalNormal.png",
			"LocalSelected.png",
			CC_CALLBACK_1(WelcomeScene::OnLocalPressed, this));

		localItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + localItem->getContentSize().height *2.5f+tbasey+tinterval*2));

		// create menu, it's an autorelease object
		auto menu = Menu::create(localItem, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);
	}
		{
			auto webItem = MenuItemImage::create(
				"WebNormal.png",
				"WebSelected.png",
				CC_CALLBACK_1(WelcomeScene::OnWebPressed, this));

			webItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
				origin.y + webItem->getContentSize().height *3.5f+tbasey+3*tinterval));

			// create menu, it's an autorelease object
			auto menu = Menu::create(webItem, NULL);
			menu->setPosition(Vec2::ZERO);
			this->addChild(menu, 1);
		}
	return true;
}

void WelcomeScene::OnQuitPressed(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void WelcomeScene::OnLocalPressed(Ref* pSender)
{
	Director::getInstance()->replaceScene(LocalScene::createScene());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void WelcomeScene::OnWebPressed(Ref* pSender)
{
	//Director::getInstance()->replaceScene(WebScene::createScene());
	MessageBox("sorry,this function hasn't been finished", "unfinished");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void WelcomeScene::OnAboutPressed(cocos2d::Ref* pSender){
	Director::getInstance()->replaceScene(AboutScene::createScene());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
