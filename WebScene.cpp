#include "WebScene.h"
#include "WelcomeScene.h"
#include "WorldScene.h"
#include "BAUtility.h"
#include "BAWebPage.h"
#include "BAWebNode.h"
#include "BARecordPath.h"
#include <curl/curl.h>

USING_NS_CC;
#define MAX_BUF 4096

char wr_buf[MAX_BUF + 1];
int  wr_index;

TextFieldTTF* p;
Label* la;
size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
	   int segsize = size * nmemb;
	
		   /* Check to see if this data exceeds the size of our buffer. If so,
			   19.     * set the user-defined context value and return 0 to indicate a
			   20.     * problem to curl.
			   21.     */
			      if (wr_index + segsize > MAX_BUF) {
	        *(int *)userp = 1;
		       return 0;
	}
		   /* Copy the data from the curl buffer into our buffer */
				  memcpy((void *)&wr_buf[wr_index], buffer, (size_t)segsize);
		   /* Update the write index */
	    wr_index += segsize;
		    /* Null terminate the buffer */
		   wr_buf[wr_index] = 0;
		    /* Return the number of bytes received, indicating to curl that all is okay */
		    return segsize;
}



Scene* WebScene::createScene()
{
	auto scene = Scene::create();
	auto layer = WebScene::create();
	scene->addChild(layer);
	return scene;
}

bool WebScene::init()
{

	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//获取手机可视屏原点的坐标
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	/*
	auto label = Label::createWithTTF("WebScene", "fonts/Marker Felt.ttf", 24);
	//设置白色
	label->setColor(Color3B::WHITE);
	// 设置label在屏幕中的显示位置
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));
	// 把label添加到画面层
	this->addChild(label, 1);
	*/
	///////////////////////////////////////////////////////////////////////
	float interval = 10.0f;
	float tbasey = 10.0f;
	{
		auto bgsprite = Sprite::create("NormalBG.jpg");
		bgsprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
		this->addChild(bgsprite, 0);
	}


	float tbasex;

	{
		auto returnItem = MenuItemImage::create(
			"ReturnNormal.png",
			"ReturnSelected.png",
			CC_CALLBACK_1(WebScene::OnReturnPressed, this));
		tbasex = returnItem->getContentSize().width / 2+20.0f;
		returnItem->setPosition(Vec2(origin.x + tbasex + returnItem->getContentSize().width * 5 + interval * 5,
			origin.y + returnItem->getContentSize().height / 2+tbasey));

		// create menu, it's an autorelease object
		auto menu = Menu::create(returnItem, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);
	}

		{
			auto selectItem = MenuItemImage::create(
				"ConfirmNormal.png",
				"ConfirmSelected.png",
				CC_CALLBACK_1(WebScene::OnSelectPressed, this));

			selectItem->setPosition(Vec2(origin.x + tbasex+selectItem->getContentSize().width *4+interval*4,
				origin.y + selectItem->getContentSize().height /2+tbasey));

			// create menu, it's an autorelease object
			auto menu = Menu::create(selectItem, NULL);
			menu->setPosition(Vec2::ZERO);
			this->addChild(menu, 1);
		}
	{
		auto preItem = MenuItemImage::create(
			"PreNormal.png",
			"PreSelected.png",
			CC_CALLBACK_1(WebScene::OnPrePressed, this));

		preItem->setPosition(Vec2(origin.x +tbasex,
			origin.y + preItem->getContentSize().height / 2+tbasey));

		// create menu, it's an autorelease object
		auto menu = Menu::create(preItem, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);
	}

	{
		auto nextItem = MenuItemImage::create(
			"NextNormal.png",
			"NextSelected.png",
			CC_CALLBACK_1(WebScene::OnNextPressed, this));

		nextItem->setPosition(Vec2(origin.x +tbasex+ nextItem->getContentSize().width *1+ interval * 1,
			origin.y + nextItem->getContentSize().height / 2+tbasey));

		// create menu, it's an autorelease object
		auto menu = Menu::create(nextItem, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);

	}
	/////////////////////////////////////////////////////////////////////
	{
		auto bgItem = Sprite::create("Web_inputbg.png");
		_posx = origin.x + tbasex+bgItem->getContentSize().width *2 + interval * 2;
		_posy = origin.y + bgItem->getContentSize().height / 2+tbasey;
		bgItem->setPosition(Vec2(_posx,_posy));
		addChild(bgItem, 1);
		_nodex = bgItem->getContentSize().width;
		_nodey = bgItem->getContentSize().height;
		

		p = TextFieldTTF::textFieldWithPlaceHolder("", "fonts/Marker Felt.ttf", 12);
		p->setPosition(Vec2(_posx,_posy));
		addChild(p, 2);
		

		la = Label::createWithTTF("Click2Input", "fonts/Marker Felt.ttf", 12);
		la->setColor(Color3B::BLUE);
		la->setPosition(_posx,_posy);
		addChild(la, 3);
		
	}
	{
		auto goItem = MenuItemImage::create(
			"GoNormal.png",
			"GoSelected.png",
			CC_CALLBACK_1(WebScene::OnGoPressed, this));
		_goposx = origin.x + tbasex+goItem->getContentSize().width *3 + interval * 3;
		_goposy = origin.y + goItem->getContentSize().height / 2+tbasey;
		goItem->setPosition(Vec2(_goposx,_goposy));
		_gonodex = goItem->getContentSize().width;
		_gonodey = goItem->getContentSize().height;
		// create menu, it's an autorelease object
		auto menu2 = Menu::create(goItem, NULL);
		menu2->setPosition(Vec2::ZERO);
		this->addChild(menu2, 1);
	}
	/////////////////////////////////////////////////////////
	_bereturn = false;
	InitInfo();

	float nodex = 960.0f, nodey =40.0f ;
	_edgex = 1; _edgey = 7; _page = 1;
	vector<string> teminfo = GetPageInfo(1);
	BAWEBPAGE->init(_edgex,_edgey,nodex, nodey, "Web_select.png", origin.x, origin.y, visibleSize.height,"Web_node.png",this);
	for (int i = 0; i < _edgex * _edgey; i++){
		if (i + 1>teminfo.size()){ break; }
		float screenx = ((i + 1) % _edgex == 0 ? _edgex : (i + 1) % _edgex)*nodex-nodex/2 + origin.x-240;
		float screeny = visibleSize.height - nodey*((i - i % _edgex) / _edgex + 1) + nodey / 2.0f + origin.y;
		BAWEBPAGE->_vec.push_back(BAWebNode(FPOS(screenx, screeny), teminfo[i]));
	}
	
	if (BAGAMELOADER->GetPage() > 0){
		_page = BAGAMELOADER->GetPage();
		BAWEBPAGE->update(_page, GetPageInfo(_page));
		p->setString(Num2string<int>(BAGAMELOADER->GetPage()));
	}

	///////////////////////////////////////////////////////////
	
	auto _mouseListener = EventListenerMouse::create();
	_mouseListener->onMouseUp = [](Event* event){
		EventMouse* e = (EventMouse*)event;
		if (e->getMouseButton() == 0){
			if (BAWEBPAGE->IsInSameNode(e->getCursorX(), e->getCursorY())){}
		}
		
	};
	_mouseListener->onMouseDown = [=](Event* event){
		EventMouse* e = (EventMouse*)event;
		if (e->getMouseButton() == 0){
			BAWEBPAGE->_lastpos = FPOS(e->getCursorX(), e->getCursorY());
			if (BAWEBPAGE->_selectsprite->isVisible() == true){
				BAWEBPAGE->_selectsprite->setVisible(false);
			}
			if (IsAtInput(e->getCursorX(), e->getCursorY())){
				//开启输入
				p->setString("");
				log("At Input");
				p->attachWithIME();
				la->setString("");
			}
			else if (IsAtGo(e->getCursorX(), e->getCursorY())){
				log("At Go");
				p->detachWithIME();
			}
			else{
				log("other");
				//关闭输入
				p->detachWithIME();
				p->setString("");
				la->setString("Click2Input");
			}	
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
	scheduleUpdate();
	return true;
}


void WebScene::update(float delta){
	if (_bereturn){
		Director::getInstance()->replaceScene(WelcomeScene::createScene());
	}
}

vector<string> WebScene::GetPageInfo(int page){
	int num = _edgex*_edgey;
	int tag = 0;
	tag = num*(page - 1);
	vector<string> re;
	for (int i = 0; i < num && (page - 1)*num + i + 1 <= _infovec.size(); i++){
		re.push_back(_infovec[i+tag]);
	}
	return re;
}

int WebScene::GetPageNum(){
	int pagenum = _edgex*_edgey;
	return (_infovec.size() - _infovec.size() % pagenum) / pagenum + 1;
}

bool WebScene::IsAtInput(float x, float y){
	if (abs(x - _posx) <= _nodex / 2 && abs(y - _posy) <= _nodey / 2){
		return true;
	}
	else{
		return false;
	}
}

bool WebScene::IsAtGo(float x, float y){
	if (abs(x - _goposx) <= _gonodex / 2 && abs(y - _goposy) <= _gonodey / 2){
		return true;
	}
	else{
		return false;
	}
}

void WebScene::InitInfo(){
	/*
	for (int i = 0; i < 23; i++){
		_infovec.push_back("hehe" + Num2string<int>(i));
	}
	*/
	//init _infovec
	//发送请求
	//得到ai录像文件名数据
	//初始化_infovec
	CURL *curl;
	CURLcode ret;
	int  wr_error;wr_error = 0;
	wr_index = 0;
		  curl = curl_easy_init();
	  if (!curl) {
		       // printf("couldn't init curl ");
		  MessageBox("couldn't init curl", "error");
		       return ;
	}
	  ///////////////////////////////////////////////////
	  //???请求的网址
		   curl_easy_setopt(curl, CURLOPT_URL, "bomb.play.cn/Home/Index/getBattles");
	       curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&wr_error);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		    ret = curl_easy_perform(curl);
		    //printf("ret = %d (write_error = %d) ", ret, wr_error);
			if (ret == 0) {
				string tem(wr_buf);
				log("%s", wr_buf);
				_reader.parse(tem,_root);
			}
	///////////////////////////////////////////////////
		    curl_easy_cleanup(curl);

			if (strlen(wr_buf) == 0 || _root["status"].asString() != "success"){
				MessageBox("Internet unavailable or something wrong with server.", "warning");
				_bereturn = true;
				return;
			}


			for (int i = 0; i < _root["msg"]["list"].size(); i++){
				_infovec.push_back("RecordID: " + _root["msg"]["list"][i]["id"].asString() + " Player0: " + _root["msg"]["list"][i]["uname"].asString() + " Player1: " + _root["msg"]["list"][i]["oname"].asString());
			}
			
}


void WebScene::OnReturnPressed(Ref* pSender)
{
	Director::getInstance()->replaceScene(WelcomeScene::createScene());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void WebScene::OnSelectPressed(Ref* pSender)
{
	if(BAWEBPAGE->_selectsprite->isVisible()==true){
		//BAWEBPAGE->GetSelectedInfo()
		//发送请求
		//得到json数据
		/*
		CURL *curl;
		CURLcode ret;
		int  wr_error; wr_error = 0;
		wr_index = 0;
		curl = curl_easy_init();
		if (!curl) {
			// printf("couldn't init curl ");
			MessageBox("couldn't init curl", "error");
			return;
		}
		///////////////////////////////////////////////////
		//???处理url
		curl_easy_setopt(curl, CURLOPT_URL, "www.bomb.play.cn/game.php/Home/Index/getPocess?id=" + BAWEBPAGE->GetSelectedInfo().c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&wr_error);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		ret = curl_easy_perform(curl);
		//printf("ret = %d (write_error = %d) ", ret, wr_error);
		if (ret == 0) {
			BAGAMELOADER->SetJson(string(wr_buf));
		}
		///////////////////////////////////////////////////
		curl_easy_cleanup(curl);
		BAWEBPAGE->Clear();
		
		BAGAMELOADER->SetModeID(1);
		BAGAMELOADER->SetPage(_page);
        CCDirector::getInstance()->replaceScene(HelloWorld::createScene());
		*/
    } else {
		MessageBox("You haven't select anything","warning");
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void WebScene::OnPrePressed(Ref* pSender)
{
	if (_page > 1){ _page--; }
	BAWEBPAGE->update(_page, GetPageInfo(_page));


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void WebScene::OnNextPressed(Ref* pSender)
{
	if (_page >= GetPageNum()){

	}
	else{
		_page++;
		BAWEBPAGE->update(_page, GetPageInfo(_page));
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void WebScene::OnGoPressed(Ref* pSender)
{
	log("%s", p->getString().c_str());
	if (string2Num<int>(p->getString()) < 1 || string2Num<int>(p->getString()) > GetPageNum()){
		p->setString("");
		la->setString("Click2Input");
	}
	else{
		_page = string2Num<int>(p->getString());
		BAWEBPAGE->update(_page, GetPageInfo(_page));
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}