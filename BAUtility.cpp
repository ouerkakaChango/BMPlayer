#include "BAUtility.h"
#include "json.h"
#include <fstream>
#include <sstream>
#include "windows.h"
using std::wcslen;
using std::istringstream;

int FModeID(char* str){
	char cmodeid = str[0];
	int dis = 1 - '1';
	return cmodeid + dis;
}

string FGetRecordPath(char* str){
	char* iter = str + Vflag;
	string re = "";
	while (*iter != '\0'){
		if (*iter == '\\'){ re += string(1, '\\'); }
		re += string(1,*iter);
		iter += 1;
	}

	return re;
}

float FGetInterVal(char* str){
	Vflag = 0;
	int i = 2;
	char* iter = str + 2;
	string restr = "";
	while (*iter != ' '){
		restr += string(1, *iter);
		iter += 1;
		i++;
	}
	Vflag = i + 1;
	return string2Num<float>(restr);
}

char BFIDManAtBomb(char orid){
	if (orid == 'x'){
		return 'u';
	}
	else if (orid == 'y'){
		return 'v';
	}
	else if (orid == 'z'){
		return 'w';
	}
	else if (orid == 'X'){
		return 'U';
	}
	else if (orid == 'Y'){
		return 'V';
	}
	else if (orid == 'Z'){
		return 'W';
	}
	else{
		throw("MANID ERROR"); return ' ';
	}
}

char* wchar2char(wchar_t* wc){
	int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
	char* m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}

char BFGetManChar(int manid){
	if (manid ==0){
		return 'A';
	}
	else if (manid ==1){
		return 'B';
	}
	else if (manid == 2){
		return 'C';
	}
	else if (manid == 3){
		return 'X';
	}
	else if (manid == 4){
		return 'Y';
	}
	else if (manid == 5){
		return 'Z';
	}
	else{
		throw BAError("MANID ERROR");
	}
}

char BFGetPropChar(int propid){
	if (propid == 0){
		return '0';
	}
	else if (propid == 1){
		return '1';
	}
	else if (propid == 2){
		return '2';
	}
	else{
		throw BAError("PROPID ERROR");
	}
}