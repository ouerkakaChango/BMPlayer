#include "BAFileDlg.h"
#include <Afxdlgs.h>
BCMFCFileDialog* BCMFCFileDialog::_pinstance = nullptr;



BCMFCFileDialog* BCMFCFileDialog::GetInstance(){
	if (_pinstance == nullptr){
		_pinstance = new BCMFCFileDialog;
	}
	return _pinstance;
}

bool BCMFCFileDialog::OpenFile(){
	TCHAR szBuffer[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	//ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = _T("TXT文件(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0");//要选择的文件后缀 
	ofn.lpstrInitialDir = _T("D:\\Program Files");//默认的文件路径 
	ofn.lpstrFile = szBuffer;//存放文件的缓冲区 
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//标志如果是多选要加上OFN_ALLOWMULTISELECT
	BOOL bSel = GetOpenFileName(&ofn);
	if (bSel){
		_pthecopy = new vector<wchar_t>(wcslen(szBuffer) + 1); // add one for null terminator
		wcscpy_s(_pthecopy->data(), _pthecopy->size(), szBuffer);
		return true;
	}
	else{
		_pthecopy = nullptr;
		return false;
	}
}