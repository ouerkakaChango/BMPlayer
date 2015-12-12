#include "BAWebNode.h"
#include "BAWebPage.h"
#include "WebScene.h"
BAWebNode::BAWebNode(FPOS screenpos, string info):_screenpos(screenpos),_info(info){
	//�������Σ��������ݣ�ͼƬ���������ֿ��������ָߣ�Ĭ����ʾ�ַ� 
	/*
	log("%f %f", screenpos.first, screenpos.second);
	_plabelAtlas = CCLabelAtlas::create(info, BAWEBPAGE->GetNodePath(), 38, 44, '0');
	_plabelAtlas->setPosition(screenpos.first,screenpos.second);
	//������ʾ���� 25  
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