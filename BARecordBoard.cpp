#include "BARecordBoard.h"
#include "WorldScene.h"

BARecordBoard* BARecordBoard::_pinstance = nullptr;
BARecordBoard* BARecordBoard::GetInstance(){
	if (_pinstance == nullptr){
		_pinstance = new BARecordBoard;
	}
	return _pinstance;
}

void BARecordBoard::init(int resultid, float boardx, float boardy, string group1, string group2, HelloWorld* pscene){
	_resultid = resultid;
	_boardx = boardx; _boardy = boardy;
	_roundnum = 0;
	_pscene = pscene;
	_round = Label::createWithTTF("round:"+Num2string<int>(_roundnum), "fonts/Marker Felt.ttf", 35);
	_round->setColor(Color3B::WHITE);
	_round->setPosition(boardx, boardy+110);
	_pscene->addChild(_round, 3);
	
	_name.push_back(Label::createWithTTF(group1, "fonts/Marker Felt.ttf", 13));
	_name[0]->setColor(Color3B::BLUE);
	_name[0]->setPosition(boardx-40, boardy +65);
	_pscene->addChild(_name[0], 3);

	_name.push_back(Label::createWithTTF( group2, "fonts/Marker Felt.ttf", 13));
	_name[1]->setColor(Color3B::GREEN);
	_name[1]->setPosition(boardx + 40, boardy + 65);
	_pscene->addChild(_name[1], 3);

	_score.push_back(Label::createWithTTF(Num2string<int>(0), "fonts/Marker Felt.ttf", 35));
	_score[0]->setColor(Color3B::BLACK);
	_score[0]->setPosition(boardx - 40, boardy -10);
	_pscene->addChild(_score[0], 3);

	_score.push_back(Label::createWithTTF(Num2string<int>(0), "fonts/Marker Felt.ttf", 35));
	_score[1]->setColor(Color3B::BLACK);
	_score[1]->setPosition(boardx + 40, boardy -10);
	_pscene->addChild(_score[1], 3);

}

void BARecordBoard::update(int score1, int score2){
		_roundnum++;
		_round->setString("round:" + Num2string<int>(_roundnum));
		_score[0]->setString(Num2string<int>(score1));
		_score[1]->setString(Num2string<int>(score2));
		_finalscore[0] = Num2string<int>(score1);
		_finalscore[1] = Num2string<int>(score2);
}

void BARecordBoard::Clear(){
	if (_pscene != nullptr){
		_pscene->removeChild(_round);
		for (auto iter : _score){
			_pscene->removeChild(iter);
			iter = nullptr;
		}
		_score.clear();
		for (auto iter : _name){
			_pscene->removeChild(iter);
			iter = nullptr;
		}
	}
	_name.clear();
	_pscene = nullptr;
}

void BARecordBoard::SetResultText(){
	string str = "";
	if (_resultid == 2){
		str = "Tie";
	}
	else{
		str = "Winner:" + _winnername;
	}
	
		auto result0 = Label::createWithTTF(str, "fonts/Marker Felt.ttf", 18);
		result0->setColor(Color3B::WHITE);
		result0->setPosition(_boardx, _boardy - 85);
		_pscene->addChild(result0, 3);
	
	
		auto result1 = Label::createWithTTF(_finalscore[0] + ":" + _finalscore[1], "fonts/Marker Felt.ttf", 18);
		result1->setColor(Color3B::RED);
		result1->setPosition(_boardx, _boardy - 105);
		_pscene->addChild(result1, 3);
	

	if (_resultid != 2){

			
				auto result2 = Label::createWithTTF(_winnername, "fonts/Marker Felt.ttf", 45);
				result2->setColor(Color3B::RED);
				result2->setPosition(_boardx - 210, _boardy);
				_pscene->addChild(result2, 3);
			
            
	            auto result3 = Label::createWithTTF("Winner", "fonts/Marker Felt.ttf", 45);
	            result3->setColor(Color3B::RED);
	            result3->setPosition(_boardx - 210, _boardy + 50);
	            _pscene->addChild(result3, 3);
             
	} else {
				auto result4 = Label::createWithTTF("Tie", "fonts/Marker Felt.ttf", 45);
				result4->setColor(Color3B::RED);
				result4->setPosition(_boardx - 210, _boardy);
				_pscene->addChild(result4, 3);
			
	}
}