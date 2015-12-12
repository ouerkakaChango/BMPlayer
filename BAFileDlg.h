#ifndef __BCMFCUTILITY_H__
#define __BCMFCUTILITY_H__
#include "BAUtility.h"

class BCMFCFileDialog {
private:
	vector<wchar_t>* _pthecopy = nullptr;
	BCMFCFileDialog(){}
	BCMFCFileDialog(const BCMFCFileDialog& x) = delete;
	BCMFCFileDialog(BCMFCFileDialog&& x) = delete;
	BCMFCFileDialog& operator=(const BCMFCFileDialog& x) = delete;
	BCMFCFileDialog& operator=(BCMFCFileDialog&& x) = delete;
	static BCMFCFileDialog* _pinstance;
public:
	static BCMFCFileDialog* GetInstance();
	bool OpenFile();
	inline const wchar_t* FilePath(){ return _pthecopy->data(); }
};
#define BAFILEDLG BCMFCFileDialog::GetInstance()
#endif

