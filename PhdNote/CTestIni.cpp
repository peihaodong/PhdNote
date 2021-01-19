#include "pch.h"
#include "CTestIni.h"
#include "PhdIni.h"

#define SECTION_CL						_T("³ÝÂÖ")
#define KEY_JD							_T("¾«¶È")

CTestIni& CTestIni::GetInstance()
{
	CString strIniFolder = _T("E:\\");
	CString strIniPath = strIniFolder + _T("Test.ini");	//iniÂ·¾¶

	static CTestIni inst(strIniPath,_T("ZwSoftIni"));
	return inst;
}

CString CTestIni::GetClJd() const
{
	return m_apIni->GetValue(SECTION_CL, KEY_JD, _T("0.01"));
}

bool CTestIni::SetClJd(LPCTSTR szValue) const
{
	return m_apIni->SetValue(SECTION_CL, KEY_JD, szValue);
}

CTestIni::CTestIni(LPCTSTR szIniPath, LPCTSTR szAppdataDirName)
	:m_apIni(std::make_shared<Phd::PhdIni>())
{
	m_apIni->SetIniPath(szIniPath, szAppdataDirName);
}
