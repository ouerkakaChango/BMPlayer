#ifndef __BAUTILITY_H__
#define __BAUTILITY_H__
#include <string>
#include <utility>
#include <vector>
#include <sstream>
using std::string;
using std::pair;
using std::vector;
using std::make_pair;
using std::istringstream;

class BAUtility {
public:
};

template <class Type>
Type string2Num(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

template <class T>
string Num2string(const T& num){
	std::stringstream ss;
	std::string str;
	ss << num;
	ss >> str;
	return str;
}

class BAError {
private:
	string _errinfo;
public:
	BAError(string errinfo="") :_errinfo(errinfo){}

};
int FModeID(char* str);
string FGetRecordPath(char* str);
float FGetInterVal(char* str);
static int Vflag;

char BFIDManAtBomb(char orid);

char BFGetManChar(int manid);
char BFGetPropChar(int propid);

typedef pair<float, float> FPOS;
typedef pair<int, int> IPOS;

char* wchar2char(wchar_t* wc);

#endif
