#include "pch.h"
#include "PhdIni.h"

namespace Phd{

#define MAX_ALLKEYS 6000  //全部的键名
#define MAX_KEY 260  //一个键名长度
#define MAX_ALLSECTIONS 2048  //全部的段名
#define MAX_SECTION 260  //一个段名长度

void PhdIni::SetIniPath(LPCTSTR szIniPath, LPCTSTR szAppdataDirName)
{
	m_strIniPath = szIniPath;

	LPTSTR pszPath = NULL;
	pszPath = ::PathFindFileNameW(szIniPath);//获取路径的文件名

	//得到Appdata目录
	TCHAR szAppdataDir[_MAX_PATH];
	::SHGetSpecialFolderPath(nullptr, szAppdataDir, CSIDL_APPDATA, TRUE);
	m_strAppdataPath = szAppdataDir;
	m_strAppdataPath += szAppdataDirName;
	::CreateDirectory(m_strAppdataPath, nullptr);//创建该目录

	m_strAppdataPath += _T("\\");
	m_strAppdataPath += pszPath;
}

CString PhdIni::GetValue(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault /*= nullptr*/) const
{
	return GetIniValue(szSection, szKey, szDefault);
}

int PhdIni::GetValue(LPCTSTR szSection, LPCTSTR szKey, int nDefault) const
{
	TCHAR sz[10];
	::_itot_s(nDefault, sz, 10);//int型转为字符型
	const auto result = GetIniValue(szSection, szKey, sz);
	return _ttoi(result);//字符型转为int型
}

double PhdIni::GetValue(LPCTSTR szSection, LPCTSTR szKey, double fDefault) const
{
	CString strDefault;
	strDefault.Format(_T("%G"), fDefault);//double型转字符型

	const auto result = GetIniValue(szSection, szKey, strDefault);
	return _ttof(result);//字符转double型
}

bool PhdIni::SetValue(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue) const
{
	return SetIniValue(szSection, szKey, szValue);
}

bool PhdIni::SetValue(LPCTSTR szSection, LPCTSTR szKey, int value) const
{
	TCHAR sz[10];
	_itot_s(value, sz, 10);//将int型转为字符串型
	return SetIniValue(szSection, szKey, sz);
}

bool PhdIni::SetValue(LPCTSTR szSection, LPCTSTR szKey, double value) const
{
	CString strValue;
	strValue.Format(_T("%G"), value);//将double型转为字符串型
	return SetIniValue(szSection, szKey, strValue);
}

std::vector<CString> PhdIni::GetAllSections() const
{
	/*
	本函数基础：
	GetPrivateProfileSectionNames - 从 ini 文件中获得 Section 的名称
	如果 ini 中有两个 Section: [sec1] 和 [sec2]，则返回的是 'sec1',0,'sec2',0,0 ，当你不知道
	ini 中有哪些 section 的时候可以用这个 api 来获取名称
	*/
	std::vector<CString> vecSection;

	int i;
	int iPos = 0;
	int iMaxCount;
	TCHAR chSectionNames[MAX_ALLSECTIONS] = { 0 }; //总的提出来的字符串
	TCHAR chSection[MAX_SECTION] = { 0 }; //存放一个段名。
	GetPrivateProfileSectionNames(chSectionNames, MAX_ALLSECTIONS, m_strIniPath);

	//以下循环，截断到两个连续的0
	for (i = 0; i < MAX_ALLSECTIONS; i++)
	{
		if (chSectionNames[i] == 0)
			if (chSectionNames[i] == chSectionNames[i + 1])
				break;
	}

	iMaxCount = i + 1; //要多一个0号元素。即找出全部字符串的结束部分。

	for (i = 0; i < iMaxCount; i++)
	{
		chSection[iPos++] = chSectionNames[i];
		if (chSectionNames[i] == 0)
		{
			if (chSection != _T(""))
			{
				vecSection.push_back(chSection);
			}
			std::memset(chSection, 0, MAX_SECTION);
			iPos = 0;
		}
	}

	return vecSection;
}

bool PhdIni::DelSection(LPCTSTR lpSection) const
{
	return WritePrivateProfileString(lpSection, NULL, NULL, m_strIniPath);
}

bool PhdIni::DelAllSections() const
{
	std::vector<CString> vecSection = GetAllSections();
	for (int i = 0; i < vecSection.size(); i++)
		DelSection(vecSection[i]);

	return true;
}

bool PhdIni::DelKey(LPCTSTR lpSection, LPCTSTR lpKey) const
{
	return WritePrivateProfileString(lpSection, lpKey, NULL, m_strIniPath);
}

int PhdIni::GetSectionData(LPCTSTR lpszSection, std::vector<CString>& vecKey, std::vector<CString>& vecValue) const
{
	/*
	本函数基础：
	GetPrivateProfileSection- 从 ini 文件中获得一个Section的全部键名及值名
	如果ini中有一个段，其下有 "段1=值1" "段2=值2"，则返回的是 '段1=值1',0,'段2=值2',0,0 ，当你不知道
	获得一个段中的所有键及值可以用这个。
	*/
	int i = 0;
	int iPos = 0;
	CString strKeyValue;
	int iMaxCount = 0;
	TCHAR chKeyNames[MAX_ALLKEYS] = { 0 }; //总的提出来的字符串
	TCHAR chKey[MAX_KEY] = { 0 }; //提出来的一个键名

	GetPrivateProfileSection(lpszSection, chKeyNames, MAX_ALLKEYS, m_strIniPath);

	for (i = 0; i < MAX_ALLKEYS; i++)
	{
		if (chKeyNames[i] == 0)
			if (chKeyNames[i] == chKeyNames[i + 1])
				break;
	}

	iMaxCount = i + 1; //要多一个0号元素。即找出全部字符串的结束部分。

	for (i = 0; i < iMaxCount; i++)
	{
		chKey[iPos++] = chKeyNames[i];
		if (chKeyNames[i] == 0)
		{
			strKeyValue = chKey;
			CString strKey = strKeyValue.Left(strKeyValue.Find(_T('=')));
			if (strKey != _T(""))
			{
				vecKey.push_back(strKeyValue.Left(strKeyValue.Find(_T('='))));
				vecValue.push_back(strKeyValue.Mid(strKeyValue.Find(_T('=')) + 1));
			}
			std::memset(chKey, 0, MAX_KEY);
			iPos = 0;
		}
	}

	return (int)vecKey.size();
}

CString PhdIni::GetIniValue(LPCTSTR section, LPCTSTR valueName, LPCTSTR sz_default) const
{
	CString value;
	//先去Appdata目录下找
	auto dwRs = ::GetPrivateProfileString(section, valueName, sz_default, value.GetBuffer(256), 256, m_strAppdataPath);
	value.ReleaseBuffer();
	if (0x2 != GetLastError())
	{
		// 成功
		return value;
	}
	//如果appdata目录下没有，就在本地目录下找
	dwRs = ::GetPrivateProfileString(section, valueName, sz_default, value.GetBuffer(256), 256, m_strIniPath);
	value.ReleaseBuffer();
	if (0x2 != GetLastError())
	{
		//成功
		return value;
	}
	//如果都没有，就选用默认值
	return value = sz_default;
}

bool PhdIni::SetIniValue(LPCTSTR section, LPCTSTR valueName, LPCTSTR value) const
{
	bool bFlag = ::WritePrivateProfileString(section, valueName, value, m_strAppdataPath);
	if (!bFlag)
	{
		auto err = GetLastError();
	}
	return bFlag;
}

}