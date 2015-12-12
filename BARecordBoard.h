#ifndef __BARECORDBOARD_H__
#define __BARECORDBOARD_H__
#include "cocos2d.h"
#include "BAUtility.h"
USING_NS_CC;
class HelloWorld;
class BARecordBoard {
private:
	Label* _round;
	vector<Label*>  _name, _score;
	HelloWorld* _pscene;
	int _roundnum;
	float _boardx, _boardy;

	string _winnername;
	string _finalscore[2];
	int _resultid;

	static BARecordBoard* _pinstance;
	BARecordBoard(){}
	BARecordBoard(const BARecordBoard& x) = delete;
	BARecordBoard(BARecordBoard&& x) = delete;
	BARecordBoard& operator=(const BARecordBoard& x) = delete;
	BARecordBoard& operator=(BARecordBoard&& x) = delete;
public:
	static BARecordBoard* GetInstance();
	void init(int resultid,float boardx,float boardy,string group1, string group2, HelloWorld* pscene);
	void update(int score1,int score2);
	void Clear();
	inline string GetWinner(){ return _winnername; }
	void SetWinner(string winner){ _winnername = winner; }
	inline string GetFinalScore(int teamid){ return _finalscore[teamid]; }
	inline int GetResultID(){ return _resultid; }
	void SetResultText();
};
#define BARECORDBOARD BARecordBoard::GetInstance()
#endif

