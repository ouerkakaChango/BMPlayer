#include "WorldScene.h"
#include "BAUtility.h"
#include "BAFireToClear.h"
#include "BAAction.h"
#include "BAMap.h"
#include "BAMutex.h"
#include "BARecordPath.h"
#include "BARecordBoard.h"
#include "PicPathManager.h"
#include "WelcomeScene.h"
#include "WebScene.h"
#include "LocalScene.h"
#include "GifBase.h"
#include "InstantGif.h"
#include <fstream>
USING_NS_CC;
Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	////////////////////////////////////////////////////////////////////
	/*
	char buf[256] = "lauo pipe test...\0\0";
	sprintf(buf, "%s   %d", buf, rand() % 1000);
	DWORD wlen = 0;
	Sleep(1000);//等待pipe的创建成功！
	while (WaitNamedPipe(TEXT("\\\\.\\Pipe\\bapipe"), NMPWAIT_WAIT_FOREVER) == FALSE)
	{
	//cerr << "connect the namedPipe failed!" << endl;
	//return false;
	//循环等待命名管道的连接
	}
	HANDLE hPipe = CreateFile(TEXT("\\\\.\\Pipe\\bapipe"), GENERIC_READ | GENERIC_WRITE, 0,
	NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ((long)hPipe == -1)
	{
	//MessageBox("open the exit pipe failed!");
	}
	char rbuf[256];
	//从MFCPlayer传来的一行关于游戏模式的数据

	while (1)
	{
	if (WriteFile(hPipe, buf, sizeof(buf), &wlen, 0) == FALSE)
	{
	//cerr << "write to pipe failed!" << endl;
	}
	else
	{
	//cout << "write size=" << wlen << endl;

	DWORD rlen = 0;
	ReadFile(hPipe, rbuf, sizeof(rbuf), &rlen, 0);//这里可以测试，一发必先有一收（另一进程中），否则将无法继续（阻塞式）
	//cout << "thread2 read from pipe data=" << rbuf << " size=" << rlen << endl << endl;
	break;
	}
	_sleep(500);
	}



	_modeid = FModeID(rbuf);
	*/
	scheduleUpdate();
	_modeid = BAGAMELOADER->GetModeID();
	std::ifstream ifs;
	switch (_modeid){
	case 0:break;
	case 1:
		_reader.parse(BAGAMELOADER->GetJson(), _root);
		break;
	case 2:
		//_interval = FGetInterVal(rbuf);
		_interval = 0.50f;//???
		_timetochange = _interval;
		//_recordpath = FGetRecordPath(rbuf);
		_recordpath = BAGAMELOADER->GetPath();
		ifs.open(_recordpath);
		assert(ifs.is_open());
		if (!_reader.parse(ifs, _root, false)) {
			/*
			abort();
			MessageBox("Not a record file", "warning");
			_iscontinue = false;
			return true;
			*/
		}
		break;
	default: throw BAError("Mode Error"); return false; break;
	}
	if (_root["name"].asString() != "BomberMan"){
		MessageBox("Not a record file", "warning");
		_iscontinue = false;
		return true;
	}


	this->scheduleUpdate();
	_nowround = 0;

	//BAMAP的初始化
	//首先，分配好vector<vector<BANode>>每一个节点的screenpos (15*15)
	//然后，根据地图信息，要先把地图画出来。
	//（一张整的背景，先整的贴上去）
	//然后取地图字符串，一张张sprite贴上去。（分配节点的id和sprite）

	//PicPathManager的初始化
	PicPathMgr->init(45, 28, 40
		, 40, 20, 20
		, 7, 9
		, "man11", "man12", "man13", "man14"
		, "man21", "man22", "man23", "man24"
		, "man31", "man32", "man33", "man34"
		, "man41", "man42", "man43", "man44"
		, "man51", "man52", "man53", "man54"
		, "man61", "man62", "man63", "man64"
		, "prop1", "prop2", "prop3"
		, "bomb"
		, "fire"
		, "background.jpg", "recordbackground.jpg"
		, "home1.png", "home2.png", "stone.png", "boss.png", "tree.png", "wood.png"
		, "manbebombed1.png", "manbebombed2.png", "manbebombed3.png", "manbebombed4.png", "manbebombed5.png", "manbebombed6.png"
		, 0.05f
		, 20.0f, 20.0f
		, _root["group"][0]["groupName"].asString(), _root["group"][1]["groupName"].asString()
		);

	{
		auto bgsprite = Sprite::create("NormalBG.jpg");
		bgsprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
		this->addChild(bgsprite, 0);
	}
	//贴整的背景图
	{
		auto bgsprite = Sprite::create(PicPathMgr->GetBackground());
		bgsprite->setPosition(Vec2(origin.x + visibleSize.width / 2 - 80, origin.y + visibleSize.height / 2));
		this->addChild(bgsprite, 0);
	}

	//贴记分板的背景图
	float boardx = origin.x + visibleSize.width / 2 + 155, boardy = origin.y + visibleSize.height / 2;
	{
		auto bgsprite = Sprite::create(PicPathMgr->GetRecordBackground());
		bgsprite->setPosition(Vec2(boardx, boardy));
		this->addChild(bgsprite, 0);
	}
	//退出按钮
	{
		auto returnItem = MenuItemImage::create(
			"ReturnNormal.png",
			"ReturnSelected.png",
			CC_CALLBACK_1(HelloWorld::OnReturnPressed, this));

		returnItem->setPosition(Vec2(390, 25));

		// create menu, it's an autorelease object
		auto menu = Menu::create(returnItem, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);
	}

	BARECORDBOARD->init(_root["winner"].asInt() - 1, boardx, boardy, PicPathMgr->GetGroupName(0), PicPathMgr->GetGroupName(1), this);


	float nodex = PicPathMgr->GetBaseX(), nodey = PicPathMgr->GetBaseY();
	string initmap = _root["initMap"].asString();
	for (int i = 0; i < 15 * 15; i++){
		float screenx = ((i + 1) % 15 == 0 ? 15 : (i + 1) % 15)*nodex - nodex / 2.0f + origin.x + 10;
		float screeny = visibleSize.height / 2 + 150 - nodey*((i - i % 15) / 15 + 1) + nodey / 2.0f + origin.y;

		char temid = initmap[i];

		MMSprite mm;
		if (temid == '#'){
			mm.init(temid, PicPathMgr->GetHome(0));
		}
		else if (temid == '&'){
			mm.init(temid, PicPathMgr->GetHome(1));
		}
		else if (temid == '+'){
			mm.init(temid, PicPathMgr->GetWood());
		}
		else if (temid == '$'){
			mm.init(temid, PicPathMgr->GetTree());
		}
		else if (temid == '%'){
			mm.init(temid, PicPathMgr->GetStone());
		}
		else if (temid == 'A'){
			mm.init(temid, PicPathMgr->GetManPlist(0, 4), 0);
		}
		else if (temid == 'B'){
			mm.init(temid, PicPathMgr->GetManPlist(1, 4), 1);
		}
		else if (temid == 'C'){
			mm.init(temid, PicPathMgr->GetManPlist(2, 4), 2);
		}
		else if (temid == 'X'){
			mm.init(temid, PicPathMgr->GetManPlist(3, 4), 3);
		}
		else if (temid == 'Y'){
			mm.init(temid, PicPathMgr->GetManPlist(4, 4), 4);
		}
		else if (temid == 'Z'){
			mm.init(temid, PicPathMgr->GetManPlist(5, 4), 5);
		}
		else if (temid == '0'){
			mm.init(temid, PicPathMgr->GetPropPlist(0), 6);
		}
		else if (temid == '1'){
			mm.init(temid, PicPathMgr->GetPropPlist(1), 7);
		}
		else if (temid == '2'){
			mm.init(temid, PicPathMgr->GetPropPlist(2), 8);
		}
		else if (temid == '*'){
			mm.init(temid, PicPathMgr->GetBoss());
		}
		else if (temid == '@'){
			mm.init(temid, PicPathMgr->GetBombPlist(), 100);
		}
		else{

		}
		BAMAP->BindScene(this);
		BAMAP->_mapvec[(i - i % 15) / 15].push_back(BANode(FPOS(screenx, screeny), mm));
	}


	//schedule(schedule_selector(HelloWorld::myupdate), _interval);
	_timetochange = _interval;

	auto keyboardListener = EventListenerKeyboard::create();
	float mini = 0.05f;
	float miniinter = 0.05f;
	float maxinter = 100.0f;
	keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode key, Event *event){
		if (key == EventKeyboard::KeyCode::KEY_UP_ARROW){
			if (_interval - mini <= miniinter){ _interval = miniinter; }
			else{ _interval -= mini; }
		}
		else if (key == EventKeyboard::KeyCode::KEY_DOWN_ARROW){
			if (_interval + mini >= maxinter){ _interval = maxinter; }
			else{ _interval += mini; }
		}
		else if (key == EventKeyboard::KeyCode::KEY_SPACE){
			//pause
			if (_ispause == false){
				_oriinterval = _interval;
				_interval = 1000000.0f;
				_ispause = true;
			}
			else{
				_interval = _oriinterval;
				_timetochange = _interval;
				_ispause = false;
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	_allmsgnum = 0;
	return true;
}

void HelloWorld::update(float delta){
	if (_iscontinue == false){
		Director::getInstance()->replaceScene(LocalScene::createScene());
	}
	if (_timetochange >= 0.0f){
		_timetochange -= delta;
	}
	else{
		_timetochange = _interval;
		if (!(_nowround >= _root["replay"].size())){
			BAF2C->clear();
			BAJsonRound temround(_root, _nowround); _allmsgnum += temround._msgvec.size();
			BAACTION->PreRound(temround);
			BAACTION->DoAction();
			BARECORDBOARD->update(_root["replay"][_nowround]["score"][0].asInt(), _root["replay"][_nowround]["score"][1].asInt());
		}

		//结束
		if (_nowround >= _root["replay"].size()) {
			int re = _root["winner"].asInt() - 1;
			if (re != 2){
				BARECORDBOARD->SetWinner(_root["group"][_root["winner"].asInt() - 1]["groupName"].asString());
			}
			BARECORDBOARD->SetResultText();
			unscheduleUpdate();
		}
		else {
			if (_nowround >= _root["replay"].size()){
				//MessageBox((Num2string<int>(BAMUTEX->GetValue("ActionDone")) + " " + Num2string<int>(_allmsgnum)).c_str(), "haha");
			}
			_nowround++;
		}
	}
}

void HelloWorld::OnReturnPressed(cocos2d::Ref* pSender){
	BAMAP->Clear();
	BARECORDBOARD->Clear();
	if (_modeid == 2){
		Director::getInstance()->replaceScene(WelcomeScene::createScene());
	}
	else if (_modeid == 1){
		Director::getInstance()->replaceScene(WebScene::createScene());
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
