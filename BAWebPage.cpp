#include "BAWebPage.h"
#include "BAWebNode.h"
#include "WebScene.h"
#include <cmath>
BAWebPage* BAWebPage::_pinstance = nullptr;
BAWebPage* BAWebPage::GetInstance(){
	if (_pinstance == nullptr){
		_pinstance = new BAWebPage;
	}
	return _pinstance;
}

string BAWebPage::GetRecordID(string nodestr){
	auto iter = nodestr.begin() + 10;
	string numstr="";
	while (*iter != ' '){
		numstr += string(1, *iter);
		iter++;
	}
	return numstr;
}

bool BAWebPage::IsInSameNode(float x,float y){
	for (auto iter : _vec){
		FPOS centerpoint = iter.GetScreenPos();
		if (abs(centerpoint.first - _lastpos.first) <= _nodex / 2 && abs(centerpoint.second - _lastpos.second) <= _nodey / 2
			&& abs(centerpoint.first - x) <= _nodex / 2 && abs(centerpoint.second - y) <= _nodey / 2
			){
			_selectedinfo = GetRecordID(iter.GetInfo());
			log("!!!%s", _selectedinfo.c_str());
			_selectsprite->setPosition(centerpoint.first, centerpoint.second);
			_selectsprite->setVisible(true);
		}
	}
	return false;
}

void BAWebPage::update(int page,const vector<string>& infovec){
	Clear();

	for (int i = 0; i < _edgex * _edgey; i++){
		if (i + 1>infovec.size()){ break; }
		float screenx = ((i + 1) % _edgex == 0 ? _edgex : (i + 1) % _edgex)*_nodex - _nodex / 2 + _orx - 240;
		float screeny = _height - _nodey*((i - i % _edgex) / _edgex + 1) + _nodey / 2.0f + _ory;
		_vec.push_back(BAWebNode(FPOS(screenx, screeny), infovec[i]));
	}
}

void BAWebPage::Clear(){
	//调整——selectsprite使其看不见
	//对每个节点removeChild (getRunningScene)
	//_selectedinfo,_lastpos
	_selectsprite->setVisible(false);
	for (auto iter : _vec){
		_pscene->removeChild(iter._label);
		_pscene->removeChild(iter._psprite);
	}
	_vec.clear();
	_selectedinfo = "";
	_lastpos = FPOS(-500.0f, -500.0f);
}

void BAWebPage::init(int edgex,int edgey,float nodex, float nodey, string spritepath, float orx, float ory, float height,string nodepath,WebScene* pscene){
	_pscene = pscene;
	_edgex = edgex; _edgey = edgey;
	_nodex = nodex;
	_nodey = nodey;
	_selectsprite = Sprite::create(spritepath);
	_selectsprite->setPosition(0, 0);
	_selectsprite->setVisible(false);
	pscene->addChild(_selectsprite, 4);
	_orx = orx; _ory = ory;
	_height = height;
	_nodepath = nodepath;
}