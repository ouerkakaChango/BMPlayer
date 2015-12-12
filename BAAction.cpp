#include "BAAction.h"
#include "BAMap.h"
#include "BAMutex.h"

BAJsonRound::BAJsonRound(const Json::Value& root, int round){
	int n = root["replay"][round]["msg"].size();
	for (int i = 0; i<n; i++) {
		_msgvec.push_back(BAMsg(
			root["replay"][round]["msg"][i]["fname"].asString(),
			root["replay"][round]["msg"][i]["pos"][0].asInt(),
			root["replay"][round]["msg"][i]["pos"][1].asInt(),
			root["replay"][round]["msg"][i]["x"].asInt()
			));
	}
}

void BFRealMainFunc(string fname, int x, int y, int ex){
	if (fname == "mov") {
		BAMAP->MakeWalkAni(x, y, ex);//yes
	} else if (fname == "bomb") {
		BAMAP->MakeBomb(x, y);//yes
	} else if (fname == "exp") {
		if (ex == 0){
			BAMAP->MakeFire(x, y);//yes
		} else if (ex >= 10 && ex <= 15) {//人被炸死
			BAMAP->MakeDirtMan(x, y, ex);
			BAMAP->MakeFire(x, y);//yes
		} else if (ex == 2) {//木头被炸没
			BAMAP->RemoveSprite(x, y);
			BAMAP->MakeFire(x, y);//yes
		} else if (ex == 3) {
			BAMAP->MakeWood(x, y);
			BAMAP->MakeFire(x, y);//yes
		}
		else if (ex == 4){
			BAMAP->RemoveBomb(x, y);
			BAMAP->MakeFire(x, y);//yes
		} else if (ex == 5) {//道具被炸没
			/*
			BAMAP->RemoveSprite(x, y);
			BAMAP->MakeFire(x, y);//yes
			*/
			throw BAError("hehe");
		} else if (ex >= 60 && ex <= 62) {
			BAMAP->MakeProp(x, y, ex-60);
			BAMAP->MakeFire(x, y);//yes
		} else if (ex == 7) {//道具被吃
			/*
			BAMAP->RemoveSprite(x, y);
			BAMAP->EndThreadFunc();//yes*/
			throw BAError("hehe");
		} 
		else if (ex == 8){//boss被炸
			BAMAP->RemoveBoss(x, y);//yes
		}
		else {
			throw BAError("EXP X PRAMATER ERROR");
		}
	} else {
		throw BAError("FNAME ERROR");
	}
}


BAAction* BAAction::_pinstance = nullptr;

BAAction* BAAction::GetInstance(){
	if (_pinstance == nullptr){
		_pinstance = new BAAction;
	}
	return _pinstance;
}

void BAAction::PreRound(const BAJsonRound& bajr){
	_threadvec.clear();
	for (auto iter:bajr._msgvec){
		_threadvec.push_back(thread(&BFRealMainFunc, iter.GetFName(), iter.GetY()-1, iter.GetX()-1, iter.GetEX()));
		//log("%s %d %d %d", iter.GetFName(), iter.GetX(), iter.GetY(), iter.GetEX());
	}
}

void BAAction::DoAction(){
	for (auto& iter : _threadvec){
		iter.join();
	}
}
