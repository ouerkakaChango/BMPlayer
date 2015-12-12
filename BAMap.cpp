#include "BAMap.h"
#include "HelloWorldScene.h"
#include "PicPathManager.h"
#include "BAFireToClear.h"
#include "BAMutex.h"
#include "GifBase.h"
#include "InstantGif.h"
#include "BATimer.h"
#include <regex>
#include <thread>
using std::regex;
using std::regex_match;

std::mutex mtx;

void MMSprite::init(char id,string path,int exid){
	_id = id;
	if (regex_match(path,regex("\.*\\.png")) ){
		_modeid = 0;
		_path = path;
	}
	else if (regex_match(path, regex("\.*\\.gif"))){
		_modeid = 1;
		_path = path;
	}
	else if (regex_match(path, regex("\.*\\.plist"))){
		_modeid = 2;
		_path = path;
		_exid = exid;
	}
	else{
		throw BAError("MMSPRITE INIT ERROR");
	}
}

BANode::BANode(FPOS screenpos, MMSprite mmsprite) :_screenpos(screenpos), _mmsprite(mmsprite){
	if (mmsprite._modeid == 0){
		_mmsprite._spritevec.push_back ( make_pair(Sprite::create(_mmsprite._path),mmsprite._id));
		_mmsprite._spritevec[0].first->setPosition(screenpos.first, screenpos.second);
		BAMAP->GetScene()->addChild(_mmsprite._spritevec[0].first, 2);
	}
	else if (mmsprite._modeid == 1){
		string name = FileUtils::getInstance()->fullPathForFilename(_mmsprite._path.c_str());
		_mmsprite._pgif = InstantGif::create(name.c_str());
		_mmsprite._pgif->setPosition(screenpos.first, screenpos.second);
		BAMAP->GetScene()->addChild(_mmsprite._pgif, 2);
	}
	else if (mmsprite._modeid == 2){
		int anilength = -1;
		// ���������ɵ�plist�ļ�
		CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		if (mmsprite._exid < 6){
			CCTexture2D* temtexture = CCTextureCache::getInstance()->textureForKey(PicPathMgr->GetManPng(mmsprite._exid, 4)); //����������ȡ��Texure2D�������䵱��������addSpriteFramesWithFile������    
			cache->addSpriteFramesWithFile(PicPathMgr->GetManPlist(mmsprite._exid, 4), temtexture);
			anilength = PicPathMgr->GetManLength(mmsprite._exid);
		}
		else if (mmsprite._exid==100){
			CCTexture2D* temtexture = CCTextureCache::getInstance()->textureForKey(PicPathMgr->GetBombPng()); //����������ȡ��Texure2D�������䵱��������addSpriteFramesWithFile������    
			cache->addSpriteFramesWithFile(PicPathMgr->GetBombPlist(), temtexture);
			anilength = PicPathMgr->GetBombLength();
		}
		else{
			CCTexture2D* temtexture = CCTextureCache::getInstance()->textureForKey(PicPathMgr->GetPropPng(mmsprite._exid - 6)); //����������ȡ��Texure2D�������䵱��������addSpriteFramesWithFile������    
			cache->addSpriteFramesWithFile(PicPathMgr->GetPropPlist(mmsprite._exid - 6), temtexture);
			anilength = PicPathMgr->GetPropLength(mmsprite._exid - 6);
		}
	
		Vector<SpriteFrame* > fcache;
		char *frameNameChar = new char[20];
		for (int i = 0; i < anilength; i++)
		{
			//���������Ϊ���ǰ��ÿ��ͼƬ�����֣�����plist�ļ��ܷ����һ����ǿ��Ը��ݴ��ǰ��ͼƬ����ֱ�ӷ��ʵ���ͼƬ
			if (mmsprite._exid < 6){
				sprintf(frameNameChar, "man%d4_%d.png", mmsprite._exid+1, i+1);
			}
			else if (mmsprite._exid == 100){
				sprintf(frameNameChar, "bomb_%d.png", i + 1);
			}
			else{
				sprintf(frameNameChar, "prop%d_%d.png", mmsprite._exid-5, i+1);
			}
			fcache.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameNameChar));
		}
		//����֡���д����������ڶ�������Ϊÿ֮֡��ļ��ʱ����
		CCAnimation *splitAnimation = CCAnimation::createWithSpriteFrames(fcache, PicPathMgr->GetPNGInterval());
		//���ݶ���������������
		CCAnimate *splitAnimate = CCAnimate::create(splitAnimation);
		_mmsprite._spritevec.push_back ( make_pair(CCSprite::create(),mmsprite._id));

		//�þ���ִ�иö�������������������ѭ��
		_mmsprite._spritevec[0].first->runAction(CCRepeatForever::create(splitAnimate));
		_mmsprite._spritevec[0].first->setPosition(screenpos.first, screenpos.second);
		BAMAP->GetScene()->addChild(_mmsprite._spritevec[0].first, 2);
		
	}
	else{
		//throw BAError("BANODE INIT ERROR");
	}
}

BAMap* BAMap::_pinstance=nullptr;
BAMap* BAMap::GetInstance(){
	if (_pinstance == nullptr){
		_pinstance = new BAMap;
	}
	return _pinstance;
}

BAMap::BAMap() {
	//��ʼ��_mapvec
	for (int i = 0; i < 15; i++){
		_mapvec.push_back(vector<BANode>());
	}
}

bool BAMap::BindScene(HelloWorld* p){
	if (p == nullptr) { 
		throw BAError("BINDSCENE ERROR");
		return false; 
	}
	_pscene = p;
	return true;
}

void BAMap::safecheck(int x, int y){
	
}

void BAMap::MakeBomb(int x, int y){
	safecheck(x, y);

	int anilength = -1;
	CCAnimation *splitAnimation = nullptr;
	// ���������ɵ�plist�ļ�

	{
		std::lock_guard<std::mutex> lck(mtx);
		// ���������ɵ�plist�ļ�
		CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		CCTexture2D* temtexture = CCTextureCache::getInstance()->textureForKey(PicPathMgr->GetBombPng()); //����������ȡ��Texure2D�������䵱��������addSpriteFramesWithFile������    
		cache->addSpriteFramesWithFile(PicPathMgr->GetBombPlist(), temtexture);

		anilength = PicPathMgr->GetBombLength();


		Vector<SpriteFrame* > fcache;
		char *frameNameChar = new char[20];
		for (int i = 0; i < anilength; i++)
		{
			//���������Ϊ���ǰ��ÿ��ͼƬ�����֣�����plist�ļ��ܷ����һ����ǿ��Ը��ݴ��ǰ��ͼƬ����ֱ�ӷ��ʵ���ͼƬ
			sprintf(frameNameChar, "bomb_%d.png", i + 1);
			fcache.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameNameChar));
		}
		//����֡���д����������ڶ�������Ϊÿ֮֡��ļ��ʱ����
		splitAnimation = CCAnimation::createWithSpriteFrames(fcache, PicPathMgr->GetPNGInterval());

	}
	//���ݶ���������������
	CCAnimate *splitAnimate = CCAnimate::create(splitAnimation);

	_mapvec[x][y]._mmsprite._spritevec.push_back( make_pair(CCSprite::create(),'@'));

	//�þ���ִ�иö�������������������ѭ��
	_mapvec[x][y]._mmsprite._spritevec.back().first->runAction(CCRepeatForever::create(splitAnimate));
	float posx = BAMAP->_mapvec[x][y]._screenpos.first, posy = BAMAP->_mapvec[x][y]._screenpos.second;
	_mapvec[x][y]._mmsprite._spritevec.back().first->setPosition(posx, posy);
	_pscene->addChild(_mapvec[x][y]._mmsprite._spritevec.back().first, 1);
	
	_bombvec[IPOS(x, y)] = _mapvec[x][y]._mmsprite._spritevec.back().first;

	BAMUTEX->AddValue("ActionDone");
}

void BAMap::MakeFire(int x, int y){
	safecheck(x, y);
	if (_mapvec[x][y]._pfiresprite != nullptr){ return; }
	else{


		BAF2C->push(x, y);
		int anilength = -1;
		CCAnimation *splitAnimation = nullptr;
		// ���������ɵ�plist�ļ�

		{
			std::lock_guard<std::mutex> lck(mtx);
			// ���������ɵ�plist�ļ�
			CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
			CCTexture2D* temtexture = CCTextureCache::getInstance()->textureForKey(PicPathMgr->GetFirePng()); //����������ȡ��Texure2D�������䵱��������addSpriteFramesWithFile������    
			cache->addSpriteFramesWithFile(PicPathMgr->GetFirePlist(), temtexture);

			anilength = PicPathMgr->GetFireLength();


			Vector<SpriteFrame* > fcache;
			char *frameNameChar = new char[20];
			for (int i = 0; i < anilength; i++)
			{
				//���������Ϊ���ǰ��ÿ��ͼƬ�����֣�����plist�ļ��ܷ����һ����ǿ��Ը��ݴ��ǰ��ͼƬ����ֱ�ӷ��ʵ���ͼƬ
				sprintf(frameNameChar, "fire_%d.png", i + 1);
				fcache.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameNameChar));
			}
			//����֡���д����������ڶ�������Ϊÿ֮֡��ļ��ʱ����
			splitAnimation = CCAnimation::createWithSpriteFrames(fcache, PicPathMgr->GetPNGInterval()*3.0f);

		}
		//���ݶ���������������
		CCAnimate *splitAnimate = CCAnimate::create(splitAnimation);

		_mapvec[x][y]._pfiresprite = CCSprite::create();

		//�þ���ִ�иö�������������������ѭ��
		_mapvec[x][y]._pfiresprite->runAction(CCRepeatForever::create(splitAnimate));
		float posx = BAMAP->_mapvec[x][y]._screenpos.first, posy = BAMAP->_mapvec[x][y]._screenpos.second;
		_mapvec[x][y]._pfiresprite->setPosition(posx, posy);
		_pscene->addChild(_mapvec[x][y]._pfiresprite, 3);

	}

	BAMUTEX->AddValue("ActionDone");
}

void BAMap::MakeDirtMan(int x, int y,int ex){
	safecheck(x, y);
	{
		std::lock_guard<std::mutex> lck(mtx);
		BAF2C->push_dman(x, y);
	//ɾ��ԭ����sprite
		auto iter = _mapvec[x][y]._mmsprite._spritevec.begin();
		for (; iter != _mapvec[x][y]._mmsprite._spritevec.end(); iter++){
			if (iter->second == BFGetManChar(ex-10)){
				if (iter->first != nullptr){
					_pscene->removeChild(iter->first);
					_mapvec[x][y]._mmsprite._spritevec.erase(iter);
					break;
				}
			}
		}
	//�����µ�

		CCTexture2D* temtexture = CCTextureCache::getInstance()->textureForKey(PicPathMgr->GetManBeBombed(ex-10)); //����������ȡ��Texure2D�������䵱��������addSpriteFramesWithFile������    

		_mapvec[x][y]._mmsprite._spritevec.push_back( make_pair(Sprite::create(),'-'));
		if (!_mapvec[x][y]._mmsprite._spritevec.back().first->initWithTexture(temtexture)) {
			throw BAError("DIRTMAN INIT ERROR");
		}
		_mapvec[x][y]._mmsprite._spritevec.back().first->setPosition(Vec2(_mapvec[x][y]._screenpos.first, _mapvec[x][y]._screenpos.second));
		_pscene->addChild(_mapvec[x][y]._mmsprite._spritevec.back().first, 2);
	}
}

void BAMap::MakeWood(int x, int y){
	safecheck(x, y);
	{
		std::lock_guard<std::mutex> lck(mtx);
		//ɾ��ԭ����sprite
		for (auto iter : _mapvec[x][y]._mmsprite._spritevec){
			if (iter.second == '$'){
				_pscene->removeChild(iter.first);
				iter.first = nullptr; iter.second = ' ';
				break;
			}
		}
		//�����µ�

		CCTexture2D* temtexture = CCTextureCache::getInstance()->textureForKey(PicPathMgr->GetWood()); //����������ȡ��Texure2D�������䵱��������addSpriteFramesWithFile������    
		_mapvec[x][y]._mmsprite._spritevec.push_back(make_pair(Sprite::create(),'+'));
		if (!_mapvec[x][y]._mmsprite._spritevec.back().first->initWithTexture(temtexture)) {
			throw BAError("WOOD INIT ERROR");
		}
		_mapvec[x][y]._mmsprite._spritevec.back().first->setPosition(Vec2(_mapvec[x][y]._screenpos.first, _mapvec[x][y]._screenpos.second));
		_pscene->addChild(_mapvec[x][y]._mmsprite._spritevec.back().first, 1);
	}
}

void BAMap::MakeProp(int x, int y, int propid){
	safecheck(x, y);
	//ɾ��ԭ����sprite
	auto iter = _mapvec[x][y]._mmsprite._spritevec.begin();
	for (; iter != _mapvec[x][y]._mmsprite._spritevec.end(); iter++){
		if (iter->second =='+'){
			if (iter->first != nullptr){
				_pscene->removeChild(iter->first);
				_mapvec[x][y]._mmsprite._spritevec.erase(iter);
				break;
			}
		}
	}
	//�����µ�
	int anilength = -1;
	CCAnimation *splitAnimation = nullptr;
	safecheck(x, y);
	{
		std::lock_guard<std::mutex> lck(mtx);
		// ���������ɵ�plist�ļ�
		CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		CCTexture2D* temtexture = CCTextureCache::getInstance()->textureForKey(PicPathMgr->GetPropPng(propid)); //����������ȡ��Texure2D�������䵱��������addSpriteFramesWithFile������    
		cache->addSpriteFramesWithFile(PicPathMgr->GetPropPlist(propid), temtexture);
		anilength = PicPathMgr->GetPropLength(propid);


		Vector<SpriteFrame* > fcache;
		char *frameNameChar = new char[20];
		for (int i = 0; i < anilength; i++)
		{
			//���������Ϊ���ǰ��ÿ��ͼƬ�����֣�����plist�ļ��ܷ����һ����ǿ��Ը��ݴ��ǰ��ͼƬ����ֱ�ӷ��ʵ���ͼƬ
			sprintf(frameNameChar, "prop%d_%d.png", propid + 1, i + 1);
			fcache.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameNameChar));
		}
		//����֡���д����������ڶ�������Ϊÿ֮֡��ļ��ʱ����
		splitAnimation = CCAnimation::createWithSpriteFrames(fcache, PicPathMgr->GetPNGInterval());

	}
	//���ݶ���������������
	CCAnimate *splitAnimate = CCAnimate::create(splitAnimation);
	_mapvec[x][y]._mmsprite._spritevec.push_back(make_pair(CCSprite::create(),BFGetPropChar(propid)));

	//�þ���ִ�иö�������������������ѭ��
	_mapvec[x][y]._mmsprite._spritevec.back().first->runAction(CCRepeatForever::create(splitAnimate));
	float posx = BAMAP->_mapvec[x][y]._screenpos.first, posy = BAMAP->_mapvec[x][y]._screenpos.second;
	_mapvec[x][y]._mmsprite._spritevec.back().first->setPosition(posx, posy);
	_pscene->addChild(_mapvec[x][y]._mmsprite._spritevec.back().first, 2);
}

void BAMap::MakeMan(int x, int y,int manid,int dir,bool b){
	safecheck(x, y);
	
	//ɾ��ԭ����sprite
	auto iter = _mapvec[x][y]._mmsprite._spritevec.begin();
	for (; iter != _mapvec[x][y]._mmsprite._spritevec.end(); iter++){
		if (iter->second == BFGetManChar(manid)){
			if (iter->first != nullptr){
				_pscene->removeChild(iter->first);
				_mapvec[x][y]._mmsprite._spritevec.erase(iter);
				break;
			}
		}
	}
	//�����µ�

	int anilength = -1;
	CCAnimate *splitAnimate = nullptr;
	CCAnimation *splitAnimation = nullptr;
	//���ڶ��̷߳��ʹ�����Դ�����ϻ�����
	{
		std::lock_guard<std::mutex> lck(mtx);
		// ���������ɵ�plist�ļ�
		CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		CCTexture2D* temtexture = CCTextureCache::getInstance()->textureForKey(PicPathMgr->GetManPng(manid, dir)); //����������ȡ��Texure2D�������䵱��������addSpriteFramesWithFile������    
		cache->addSpriteFramesWithFile(PicPathMgr->GetManPlist(manid, dir), temtexture);



		anilength = PicPathMgr->GetManLength(manid);


		Vector<SpriteFrame* > fcache;
		char *frameNameChar = new char[20];
		for (int i = 0; i < anilength; i++)
		{
			//���������Ϊ���ǰ��ÿ��ͼƬ�����֣�����plist�ļ��ܷ����һ����ǿ��Ը��ݴ��ǰ��ͼƬ����ֱ�ӷ��ʵ���ͼƬ
			sprintf(frameNameChar, "man%d%d_%d.png", manid + 1, dir, i + 1);
			fcache.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameNameChar));
		}
		//����֡���д����������ڶ�������Ϊÿ֮֡��ļ��ʱ����
		splitAnimation = CCAnimation::createWithSpriteFrames(fcache, PicPathMgr->GetPNGInterval());
	}
		//���ݶ���������������
	splitAnimate = CCAnimate::create(splitAnimation);
	//auto testSprite = _mapvec[x][y]._mmsprite._staticsprite;
	_mapvec[x][y]._mmsprite._spritevec.push_back(make_pair( CCSprite::create(),BFGetManChar(manid)));

	//�þ���ִ�иö�������������������ѭ��
	_mapvec[x][y]._mmsprite._spritevec.back().first->runAction(CCRepeatForever::create(splitAnimate));
	float posx = BAMAP->_mapvec[x][y]._screenpos.first, posy = BAMAP->_mapvec[x][y]._screenpos.second;
	_mapvec[x][y]._mmsprite._spritevec.back().first->setPosition(posx, posy);
	_pscene->addChild(_mapvec[x][y]._mmsprite._spritevec.back().first, 2);
	if (b){
		BAMUTEX->AddValue("ActionDone");
	}
}


void BAMap::RemoveSprite(int x, int y){
	safecheck(x, y);
	{
		//log("%d %d", x, y);
		std::lock_guard<std::mutex> lck(mtx);
		bool isadd = true;
		auto iter = _mapvec[x][y]._mmsprite._spritevec.begin();
		for (; iter != _mapvec[x][y]._mmsprite._spritevec.end(); ){
			isadd = true;
				if (iter->first != nullptr){
					_pscene->removeChild(iter->first);
					iter=_mapvec[x][y]._mmsprite._spritevec.erase(iter);
					isadd = false;
				}
				if (isadd){ iter++; }
		}
	}
}

void BAMap::RemoveBomb(int x, int y){
	safecheck(x, y);
	{
		std::lock_guard<std::mutex> lck(mtx);
		auto iter = _bombvec.begin();
		for (; iter != _bombvec.end(); iter++){
			if (iter->first == IPOS(x, y)&&_pscene!=nullptr){
				_pscene->removeChild(iter->second);
				iter = _bombvec.erase(iter);
				return;
			}
		}
		throw BAError("FINDBOMB ERROR");
	}
}

void BAMap::RemoveFire(int x, int y){
	safecheck(x, y);
	_pscene->removeChild(_mapvec[x][y]._pfiresprite);
	_mapvec[x][y]._pfiresprite = nullptr;
}

void BAMap::RemoveDirtMan(int x, int y){
	safecheck(x, y);
	auto iter = _mapvec[x][y]._mmsprite._spritevec.begin();
	for (; iter != _mapvec[x][y]._mmsprite._spritevec.end(); iter++){
		if (iter->second == '-'){
			if (iter->first != nullptr){
				_pscene->removeChild(iter->first);
				_mapvec[x][y]._mmsprite._spritevec.erase(iter);
				break;
			}
		}
	}
}

void BAMap::MakeWalkAni(int x,int y,int ex){
	safecheck(x, y);
	int dir = (ex - ex % 10) / 10, manid = ex - 10 * dir;

	if (dir == 1){
		float orix = -1.0f, oriy = -1.0f, minix = -1.0f, miniinter = -1.0f;
		int allnum = -1;
		{
			std::lock_guard<std::mutex> lck(mtx);
			//////////////////////////////////////////////////////////////////////////
			//RemoveSprite(x, y);
			//make man
			
			auto iter = _mapvec[x][y]._mmsprite._spritevec.begin();
			for (; iter != _mapvec[x][y]._mmsprite._spritevec.end(); iter++){
				if (iter->second == BFGetManChar(manid)){
					if (iter->first != nullptr){
						_pscene->removeChild(iter->first);
						_mapvec[x][y]._mmsprite._spritevec.erase(iter);
						break;
					}
				}
			}

			
			CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
			CCTexture2D* temtexture = CCTextureCache::getInstance()->textureForKey(PicPathMgr->GetManPng(manid, dir)); //����������ȡ��Texure2D�������䵱��������addSpriteFramesWithFile������    
			cache->addSpriteFramesWithFile(PicPathMgr->GetManPlist(manid, dir), temtexture);


			int anilength = -1;
			CCAnimate *splitAnimate = nullptr;
			CCAnimation *splitAnimation = nullptr;

			anilength = PicPathMgr->GetManLength(manid);


			Vector<SpriteFrame* > fcache;
			char *frameNameChar = new char[20];
			for (int i = 0; i < anilength; i++)
			{
				//���������Ϊ���ǰ��ÿ��ͼƬ�����֣�����plist�ļ��ܷ����һ����ǿ��Ը��ݴ��ǰ��ͼƬ����ֱ�ӷ��ʵ���ͼƬ
				sprintf(frameNameChar, "man%d%d_%d.png", manid + 1, dir, i + 1);
				fcache.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameNameChar));
			}
			//����֡���д����������ڶ�������Ϊÿ֮֡��ļ��ʱ����
			splitAnimation = CCAnimation::createWithSpriteFrames(fcache, PicPathMgr->GetPNGInterval());

			//���ݶ���������������
			splitAnimate = CCAnimate::create(splitAnimation);
			//auto testSprite = _mapvec[x][y]._mmsprite._staticsprite;
			_mapvec[x][y-1]._mmsprite._spritevec.push_back (make_pair( CCSprite::create(),BFGetManChar(manid)));

			//�þ���ִ�иö�������������������ѭ��
			_mapvec[x][y-1]._mmsprite._spritevec.back().first->runAction(CCRepeatForever::create(splitAnimate));
			float posx = BAMAP->_mapvec[x][y]._screenpos.first, posy = BAMAP->_mapvec[x][y]._screenpos.second;
			_mapvec[x][y-1]._mmsprite._spritevec.back().first->setPosition(posx, posy);


			CCPoint BOriginalPos = Vec2(_mapvec[x][y-1]._screenpos.first, _mapvec[x][y-1]._screenpos.second);  //this is the sprite where i want to move sprite A to  , which is : x160.000000 ,y:81.000000
			CCMoveTo* AMoveToB = CCMoveTo::create(0.3f, BOriginalPos);
			CCSequence* AMovesToBSequence = CCSequence::create(AMoveToB, NULL);
			_mapvec[x][y-1]._mmsprite._spritevec.back().first->runAction(AMovesToBSequence);


			_pscene->addChild(_mapvec[x][y-1]._mmsprite._spritevec.back().first, 2);
			/////////////////////////////////////////////////////////////////////////////
		}
	}
	else if (dir == 2){
		float orix = -1.0f, oriy=-1.0f,minix = -1.0f, miniinter = -1.0f;
		int allnum = -1;
			{
				std::lock_guard<std::mutex> lck(mtx);
				//////////////////////////////////////////////////////////////////////////
				//RemoveSprite(x, y);
				//make man
			
				auto iter = _mapvec[x][y]._mmsprite._spritevec.begin();
				for (; iter != _mapvec[x][y]._mmsprite._spritevec.end(); iter++){
					if (iter->second == BFGetManChar(manid)){
						if (iter->first != nullptr){
							_pscene->removeChild(iter->first);
							_mapvec[x][y]._mmsprite._spritevec.erase(iter);
							break;
						}
					}
				}
				

				CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
				CCTexture2D* temtexture = CCTextureCache::getInstance()->textureForKey(PicPathMgr->GetManPng(manid, dir)); //����������ȡ��Texure2D�������䵱��������addSpriteFramesWithFile������    
				cache->addSpriteFramesWithFile(PicPathMgr->GetManPlist(manid, dir), temtexture);


				int anilength = -1;
				CCAnimate *splitAnimate = nullptr;
				CCAnimation *splitAnimation = nullptr;

				anilength = PicPathMgr->GetManLength(manid);


				Vector<SpriteFrame* > fcache;
				char *frameNameChar = new char[20];
				for (int i = 0; i < anilength; i++)
				{
					//���������Ϊ���ǰ��ÿ��ͼƬ�����֣�����plist�ļ��ܷ����һ����ǿ��Ը��ݴ��ǰ��ͼƬ����ֱ�ӷ��ʵ���ͼƬ
					sprintf(frameNameChar, "man%d%d_%d.png", manid + 1, dir, i + 1);
					fcache.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameNameChar));
				}
				//����֡���д����������ڶ�������Ϊÿ֮֡��ļ��ʱ����
				splitAnimation = CCAnimation::createWithSpriteFrames(fcache, PicPathMgr->GetPNGInterval());

				//���ݶ���������������
				splitAnimate = CCAnimate::create(splitAnimation);
				//auto testSprite = _mapvec[x][y]._mmsprite._staticsprite;
				_mapvec[x ][y+1]._mmsprite._spritevec.push_back(make_pair(CCSprite::create(), BFGetManChar(manid)));

				//�þ���ִ�иö�������������������ѭ��
				_mapvec[x ][y+1]._mmsprite._spritevec.back().first->runAction(CCRepeatForever::create(splitAnimate));
				float posx = BAMAP->_mapvec[x][y]._screenpos.first, posy = BAMAP->_mapvec[x][y]._screenpos.second;
				_mapvec[x][y+1]._mmsprite._spritevec.back().first->setPosition(posx, posy);


				CCPoint BOriginalPos = Vec2(_mapvec[x][y + 1]._screenpos.first, _mapvec[x][y + 1]._screenpos.second);  //this is the sprite where i want to move sprite A to  , which is : x160.000000 ,y:81.000000
				CCMoveTo* AMoveToB = CCMoveTo::create(0.3f, BOriginalPos);
				CCSequence* AMovesToBSequence = CCSequence::create(AMoveToB, NULL);
				_mapvec[x ][y+1]._mmsprite._spritevec.back().first->runAction(AMovesToBSequence);


				_pscene->addChild(_mapvec[x][y+1]._mmsprite._spritevec.back().first, 2);
				/////////////////////////////////////////////////////////////////////////////
			}

	}
	else if (dir == 3){
		float orix = -1.0f, oriy = -1.0f, miniy = -1.0f, miniinter = -1.0f;
		int allnum = -1;
		{
			std::lock_guard<std::mutex> lck(mtx);
			//////////////////////////////////////////////////////////////////////////
			//RemoveSprite(x, y);
			//make man
			auto iter = _mapvec[x][y]._mmsprite._spritevec.begin();
			for (; iter != _mapvec[x][y]._mmsprite._spritevec.end();iter++){
				if (iter->second == BFGetManChar(manid)){
					if (iter->first != nullptr){
						_pscene->removeChild(iter->first);
						_mapvec[x][y]._mmsprite._spritevec.erase(iter);
						break;
					}
				}
			}

		
			CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
			CCTexture2D* temtexture = CCTextureCache::getInstance()->textureForKey(PicPathMgr->GetManPng(manid, dir)); //����������ȡ��Texure2D�������䵱��������addSpriteFramesWithFile������    
			cache->addSpriteFramesWithFile(PicPathMgr->GetManPlist(manid, dir), temtexture);


			int anilength = -1;
			CCAnimate *splitAnimate = nullptr;
			CCAnimation *splitAnimation = nullptr;

			anilength = PicPathMgr->GetManLength(manid);


			Vector<SpriteFrame* > fcache;
			char *frameNameChar = new char[20];
			for (int i = 0; i < anilength; i++)
			{
				//���������Ϊ���ǰ��ÿ��ͼƬ�����֣�����plist�ļ��ܷ����һ����ǿ��Ը��ݴ��ǰ��ͼƬ����ֱ�ӷ��ʵ���ͼƬ
				sprintf(frameNameChar, "man%d%d_%d.png", manid + 1, dir, i + 1);
				fcache.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameNameChar));
			}
			//����֡���д����������ڶ�������Ϊÿ֮֡��ļ��ʱ����
			splitAnimation = CCAnimation::createWithSpriteFrames(fcache, PicPathMgr->GetPNGInterval());

			//���ݶ���������������
			splitAnimate = CCAnimate::create(splitAnimation);
			//auto testSprite = _mapvec[x][y]._mmsprite._staticsprite;
			_mapvec[x-1][y]._mmsprite._spritevec.push_back(make_pair( CCSprite::create(),BFGetManChar(manid)));

			//�þ���ִ�иö�������������������ѭ��
			_mapvec[x-1][y]._mmsprite._spritevec.back().first->runAction(CCRepeatForever::create(splitAnimate));
			float posx = BAMAP->_mapvec[x][y]._screenpos.first, posy = BAMAP->_mapvec[x][y]._screenpos.second;
			_mapvec[x-1][y]._mmsprite._spritevec.back().first->setPosition(posx, posy);

			CCPoint BOriginalPos = Vec2(_mapvec[x-1][y]._screenpos.first, _mapvec[x-1][y]._screenpos.second);  //this is the sprite where i want to move sprite A to  , which is : x160.000000 ,y:81.000000
			CCMoveTo* AMoveToB = CCMoveTo::create(0.3f, BOriginalPos);
			CCSequence* AMovesToBSequence = CCSequence::create(AMoveToB, NULL);
			_mapvec[x-1][y]._mmsprite._spritevec.back().first->runAction(AMovesToBSequence);

			_pscene->addChild(_mapvec[x-1][y]._mmsprite._spritevec.back().first, 2);
			/////////////////////////////////////////////////////////////////////////////
		}
	}
	else if (dir == 4){
		float orix = -1.0f, oriy = -1.0f, miniy = -1.0f, miniinter = -1.0f;
		int allnum = -1;
		{
			std::lock_guard<std::mutex> lck(mtx);

			//////////////////////////////////////////////////////////////////////////
			//RemoveSprite(x, y);
			//make man
			auto iter = _mapvec[x][y]._mmsprite._spritevec.begin();
			for (; iter != _mapvec[x][y]._mmsprite._spritevec.end(); iter++){
				if (iter->second == BFGetManChar(manid)){
					if (iter->first != nullptr){
						_pscene->removeChild(iter->first);
						_mapvec[x][y]._mmsprite._spritevec.erase(iter);
						break;
					}
				}
			}

			CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
			CCTexture2D* temtexture = CCTextureCache::getInstance()->textureForKey(PicPathMgr->GetManPng(manid, dir)); //����������ȡ��Texure2D�������䵱��������addSpriteFramesWithFile������    
			cache->addSpriteFramesWithFile(PicPathMgr->GetManPlist(manid, dir), temtexture);


			int anilength = -1;
			CCAnimate *splitAnimate = nullptr;
			CCAnimation *splitAnimation = nullptr;

			anilength = PicPathMgr->GetManLength(manid);


			Vector<SpriteFrame* > fcache;
			char *frameNameChar = new char[20];
			for (int i = 0; i < anilength; i++)
			{
				//���������Ϊ���ǰ��ÿ��ͼƬ�����֣�����plist�ļ��ܷ����һ����ǿ��Ը��ݴ��ǰ��ͼƬ����ֱ�ӷ��ʵ���ͼƬ
				sprintf(frameNameChar, "man%d%d_%d.png", manid + 1, dir, i + 1);
				fcache.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameNameChar));
			}
			//����֡���д����������ڶ�������Ϊÿ֮֡��ļ��ʱ����
			splitAnimation = CCAnimation::createWithSpriteFrames(fcache, PicPathMgr->GetPNGInterval());

			//���ݶ���������������
			splitAnimate = CCAnimate::create(splitAnimation);
			//auto testSprite = _mapvec[x][y]._mmsprite._staticsprite;
			_mapvec[x+1][y]._mmsprite._spritevec.push_back(make_pair( CCSprite::create(),BFGetManChar(manid)));

			//�þ���ִ�иö�������������������ѭ��
			_mapvec[x+1][y]._mmsprite._spritevec.back().first->runAction(CCRepeatForever::create(splitAnimate));
			float posx = BAMAP->_mapvec[x][y]._screenpos.first, posy = BAMAP->_mapvec[x][y]._screenpos.second;
			_mapvec[x+1][y]._mmsprite._spritevec.back().first->setPosition(posx, posy);

			CCPoint BOriginalPos = Vec2(_mapvec[x + 1][y]._screenpos.first, _mapvec[x + 1][y]._screenpos.second);  //this is the sprite where i want to move sprite A to 
			CCMoveTo* AMoveToB = CCMoveTo::create(0.3f, BOriginalPos);
			CCSequence* AMovesToBSequence = CCSequence::create(AMoveToB, NULL);
			_mapvec[x+1][y]._mmsprite._spritevec.back().first->runAction(AMovesToBSequence);

			_pscene->addChild(_mapvec[x+1][y]._mmsprite._spritevec.back().first, 2);
			/////////////////////////////////////////////////////////////////////////////
		}
	}
	{
		std::lock_guard<std::mutex> lck(mtx);
		BAMUTEX->AddValue("ActionDone");
	}
}

void  BAMap::EndThreadFunc(){
	BAMUTEX->AddValue("ActionDone");
}

void BAMap::RemoveBoss(int x, int y) {
	safecheck(x, y);
	auto iter = _mapvec[x][y]._mmsprite._spritevec.begin();
	for (; iter != _mapvec[x][y]._mmsprite._spritevec.end(); iter++){
		if (iter->second == BFGetManChar('*')){
			if (iter->first != nullptr){
				_pscene->removeChild(iter->first);
				_mapvec[x][y]._mmsprite._spritevec.erase(iter);
				break;
			}
		}
	}
	BAMUTEX->AddValue("ActionDone");
}

void  BAMap::Clear(){
	if (_pscene != nullptr){
		for (auto iter : _bombvec){
			if (iter.second != nullptr){
				_pscene->removeChild(iter.second);
			}
		}
		_bombvec.clear();

		for (int i = 0; i < 15; i++){
			for (int j = 0; j < 15; j++){
				if (_mapvec[i][j]._pfiresprite != nullptr){
					_pscene->removeChild(_mapvec[i][j]._pfiresprite);
				}

				for (auto iter : _mapvec[i][j]._mmsprite._spritevec){
					if (iter.first != nullptr){
						_pscene->removeChild(iter.first);
					}
				}
			}
		}
	}
	_mapvec.clear();
	for (int i = 0; i < 15; i++){
		_mapvec.push_back(vector<BANode>());
	}
	_pscene = nullptr;
}