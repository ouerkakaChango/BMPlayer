#ifndef __BAMAP_H__
#define __BAMAP_H__
#include "BAUtility.h"
#include "cocos2d.h"
#include <map>
using std::map;
USING_NS_CC;

class GifBase;

class MMSprite {
public:
	GifBase* _pgif = nullptr;
	vector<pair<Sprite*,char>> _spritevec;
	int _modeid = -1;
	string _path = "";
	int _exid = -1;
	char _id = ' ';
	void init(char id,string path,int exid=-1);
};

class BANode{
private:
	friend class BAMap;
	friend class BAAction;
	FPOS _screenpos;
	MMSprite _mmsprite;
	Sprite* _pfiresprite = nullptr;
public:
	inline FPOS GetScreenPos(){ return _screenpos; }
	BANode(FPOS screenpos, MMSprite mmsprite);
};

typedef vector<vector<BANode> > BAMapVec;

class HelloWorld;

class BAMap {
private:
	static BAMap* _pinstance;
	BAMap();
	BAMap(const BAMap& x) = delete;
	BAMap(BAMap&& x) = delete;
	BAMap& operator=(const BAMap& x) = delete;
	BAMap& operator=(BAMap&& x) = delete;

	HelloWorld* _pscene = nullptr;

	map<IPOS,Sprite*> _bombvec;

	void safecheck(int x,int y);
public:
	static BAMap* GetInstance();
	BAMapVec _mapvec;

	bool BindScene(HelloWorld* p);
	HelloWorld* GetScene(){ return _pscene; }

	void MakeBomb(int x, int y);
	void MakeFire(int x, int y);
	void MakeDirtMan(int x, int y,int ex);
	void MakeWood(int x, int y);
	void MakeProp(int x, int y, int propid);
	void MakeMan(int x, int y,int manid,int dir=4,bool b=true);

	void RemoveSprite(int x, int y);
	void RemoveBomb(int x, int y);
	void RemoveFire(int x, int y);
	void RemoveDirtMan(int x, int y);
	void RemoveBoss(int x, int y);
	void MakeWalkAni(int x,int y,int dir);

	void EndThreadFunc();

	void Clear();
};
#define BAMAP BAMap::GetInstance()
#endif
