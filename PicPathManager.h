#ifndef __PICPATHMANAGER_H__
#define __PICPATHMANAGER_H__
#include "BAUtility.h"

class PicPathManager {
private:
	vector<vector<pair<string, string> > > _man;
	int _manlen[6];
	vector<pair<string, string> > _prop;
	int _proplen[3];
	string _bombplist, _bombpng;
	string _fireplist, _firepng;
	int _bomblen,_firelen;
	string _home[2];
	string _manbebombed[6];
	string  _bg,_recordbg, _stone,_boss,_tree,_wood;
	string _teamname[2];
	float _pnginterval,_nodex,_nodey;

	static PicPathManager* _pinstance;
	PicPathManager(){}
	PicPathManager(const PicPathManager& x) = delete;
	PicPathManager(PicPathManager&& x) = delete;
	PicPathManager& operator=(const PicPathManager& x) = delete;
	PicPathManager& operator=(PicPathManager&& x) = delete;
public:
	bool init(int len1,int len2,int len3
		,int plen1,int plen2,int plen3
		,int blen,int flen
		,string man11, string man12, string man13, string man14
		, string man21, string man22, string man23, string man24
		, string man31, string man32, string man33, string man34
		, string man41, string man42, string man43, string man44
		, string man51, string man52, string man53, string man54
		, string man61, string man62, string man63, string man64
		, string prop1, string prop2, string prop3
		, string bomb
		,string fire
		, string background,string recordbackground
		, string home1, string home2
		,string stone
		,string boss
		,string tree
		,string wood
		, string manbebombed1, string manbebombed2, string manbebombed3, string manbebombed4, string manbebombed5, string manbebombed6
		,float pnginterval
		,float nodex,float nodey
		,string team0,string team1
		);
	static PicPathManager* GetInstance();
	inline string GetManPlist(int manid, int dir){
		return _man[manid][dir - 1].first;
	}
	inline string GetManPng(int manid, int dir){
		return _man[manid][dir - 1].second;
	}
	inline int GetManLength(int manid){
		return _manlen[manid];
	}
	inline string GetPropPlist(int propid){
		return _prop[propid].first;
	}
	inline string GetPropPng(int propid){
		return _prop[propid].second;
	}
	inline int GetPropLength(int propid){
		return _proplen[propid];
	}
	inline string GetBombPlist(){ return _bombplist; }
	inline string GetBombPng(){ return _bombpng; }
	inline int GetBombLength(){return _bomblen;}

	inline string GetFirePlist(){ return _fireplist; }
	inline string GetFirePng(){ return _firepng; }
	inline int GetFireLength(){ return _firelen; }
	
	inline string GetHome(int teamid){ return _home[teamid]; }
	inline string GetBackground(){ return _bg; }
	inline string GetRecordBackground(){ return _recordbg; }
	inline string GetManBeBombed(int manid){ return _manbebombed[manid]; }
	inline string GetStone(){ return _stone; }
	inline string GetBoss(){ return _boss; }
	inline string GetTree(){ return _tree; }
	inline string GetWood(){ return _wood; }
	inline float GetPNGInterval(){ return _pnginterval; }
	inline float GetBaseX(){ return _nodex; }
	inline float GetBaseY(){ return _nodey; }
	inline string GetGroupName(int groupid){ return _teamname[groupid]; }
};

#define PicPathMgr PicPathManager::GetInstance()
#endif
