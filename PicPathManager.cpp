#include "PicPathManager.h"
#include "cocos2d.h"
USING_NS_CC;

PicPathManager* PicPathManager::_pinstance=nullptr;

PicPathManager* PicPathManager::GetInstance(){
	if (_pinstance == nullptr){
		_pinstance = new PicPathManager;
	}
	return _pinstance;
}

bool PicPathManager::init(int len1,int len2,int len3
	, int plen1, int plen2, int plen3
	, int blen,int flen
	,string man11, string man12, string man13, string man14
	, string man21, string man22, string man23, string man24
	, string man31, string man32, string man33, string man34
	, string man41, string man42, string man43, string man44
	, string man51, string man52, string man53, string man54
	, string man61, string man62, string man63, string man64
	,string prop1,string prop2,string prop3
	,string bomb
	,string fire
	,string background,string recordbackground
	,string home1,string home2
	, string stone
	,string boss
	, string tree
	, string wood
	, string manbebombed1, string manbebombed2, string manbebombed3, string manbebombed4, string manbebombed5, string manbebombed6
	,float pnginterval
	,float nodex,float nodey
	,string team0,string team1
	) {
	_manlen[0] = len1;
	_manlen[1] = len2;
	_manlen[2] = len3;
	_manlen[3] = len1;
	_manlen[4] = len2;
	_manlen[5] = len3;
	_proplen[0] = plen1;
	_proplen[1] = plen2;
	_proplen[2] = plen3;
	_bomblen = blen;
	_firelen = flen;
	for (int i = 0; i < 6; i++){
		_man.push_back(vector<pair<string,string>>());
	}
	_man[0].push_back(make_pair(man11+"_all.plist",man11+"_all.png"));
	_man[0].push_back(make_pair(man12 + "_all.plist", man12 + "_all.png"));
	_man[0].push_back(make_pair(man13 + "_all.plist", man13 + "_all.png"));
	_man[0].push_back(make_pair(man14 + "_all.plist", man14 + "_all.png"));
	_man[1].push_back(make_pair(man21 + "_all.plist", man21 + "_all.png"));
	_man[1].push_back(make_pair(man22 + "_all.plist", man22 + "_all.png"));
	_man[1].push_back(make_pair(man23 + "_all.plist", man23 + "_all.png"));
	_man[1].push_back(make_pair(man24 + "_all.plist", man24 + "_all.png"));
	_man[2].push_back(make_pair(man31 + "_all.plist", man31 + "_all.png"));
	_man[2].push_back(make_pair(man32 + "_all.plist", man32 + "_all.png"));;
	_man[2].push_back(make_pair(man33 + "_all.plist", man33 + "_all.png"));
	_man[2].push_back(make_pair(man34 + "_all.plist", man34 + "_all.png"));
	_man[3].push_back(make_pair(man41 + "_all.plist", man41 + "_all.png"));
	_man[3].push_back(make_pair(man42 + "_all.plist", man42 + "_all.png"));
	_man[3].push_back(make_pair(man43 + "_all.plist", man43 + "_all.png"));
	_man[3].push_back(make_pair(man44 + "_all.plist", man44 + "_all.png"));
	_man[4].push_back(make_pair(man51 + "_all.plist", man51 + "_all.png"));
	_man[4].push_back(make_pair(man52 + "_all.plist", man52 + "_all.png"));
	_man[4].push_back(make_pair(man53 + "_all.plist", man53 + "_all.png"));
	_man[4].push_back(make_pair(man54 + "_all.plist", man54 + "_all.png"));
	_man[5].push_back(make_pair(man61 + "_all.plist", man61 + "_all.png"));
	_man[5].push_back(make_pair(man62 + "_all.plist", man62 + "_all.png"));
	_man[5].push_back(make_pair(man63 + "_all.plist", man63 + "_all.png"));
	_man[5].push_back(make_pair(man64 + "_all.plist", man64 + "_all.png"));
	_prop.push_back( make_pair(prop1 + "_all.plist", prop1 + "_all.png"));
	_prop.push_back( make_pair(prop2 + "_all.plist", prop2 + "_all.png"));
	_prop.push_back(make_pair(prop3 + "_all.plist", prop3 + "_all.png"));
	_home[0] = home1;
	_home[1] = home2;
	_bombplist = bomb + "_all.plist";
	_bombpng = bomb + "_all.png";
	_fireplist = fire + "_all.plist";
	_firepng = fire + "_all.png";
	_bg = background;
	_recordbg = recordbackground;
	_stone = stone;
	_boss = boss;
	_tree = tree;
	_wood = wood;
	_manbebombed[0] = manbebombed1;
	_manbebombed[1] = manbebombed2;
	_manbebombed[2] = manbebombed3;
	_manbebombed[3] = manbebombed4;
	_manbebombed[4] = manbebombed5;
	_manbebombed[5] = manbebombed6;
	_pnginterval = pnginterval;
	_nodex = nodex; _nodey = nodey;
	_teamname[0] = team0;
	_teamname[1] = team1;


	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			 CCTextureCache::getInstance()->addImage(_man[i][j].second);
		}
	}
	for (int i = 0; i < 6; i++){
		CCTextureCache::getInstance()->addImage(_manbebombed[i]);
	}
	for (int i = 0; i < 3; i++){
		CCTextureCache::getInstance()->addImage(_prop[i].second);
	}
	CCTextureCache::getInstance()->addImage(_bombpng);
	CCTextureCache::getInstance()->addImage(_firepng);
	CCTextureCache::getInstance()->addImage(_wood);
	return true;
}
