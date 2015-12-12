#include "BAWebNode.h"
#include "BAWebPage.h"
#include "WebScene.h"
BAWebNode::BAWebNode(FPOS screenpos, string info):_screenpos(screenpos),_info(info){
	//参数依次：文字内容，图片，单个文字宽，单个文字高，默认显示字符 
	/*
	log("%f %f", screenpos.first, screenpos.second);
	_plabelAtlas = CCLabelAtlas::create(info, BAWEBPAGE->GetNodePath(), 38, 44, '0');
	_plabelAtlas->setPosition(screenpos.first,screenpos.second);
	//设置显示文字 25  
	_plabelAtlas->setString("25");
	Director::getInstance()->getRunningScene()->addChild(_plabelAtlas);
	*/
	_label = Label::createWithTTF(info, "fonts/Marker Felt.ttf", 8);
	_label->setColor(Color3B::WHITE);
	_label->setPosition(screenpos.first, screenpos.second);
	BAWEBPAGE->GetpScene()->addChild(_label, 3);

	_psprite = Sprite::create(BAWEBPAGE->GetNodePath());
	_psprite->setPosition(screenpos.first, screenpos.second);
	BAWEBPAGE->GetpScene()->addChild(_psprite, 2);
}