#include "pch.h"
#include "PhdRegedit.h"

namespace Phd{

void PhdRegedit::SetKeyParentAndName(HKEY hKeyParent, LPCTSTR lpszKeyName)
{
	m_hKeyParent = hKeyParent;
	m_strKeyName = lpszKeyName;
}

bool PhdRegedit::SetValue(LPCTSTR lpszValueName, LPCTSTR lpszValue) const
{
	CRegKey reg;
	if (reg.Create(m_hKeyParent, m_strKeyName) != ERROR_SUCCESS)
		return false;

	if (reg.SetStringValue(lpszValueName, lpszValue) != ERROR_SUCCESS)
		return false;

	reg.Close();
	return true;
}

bool PhdRegedit::SetValue(LPCTSTR lpszValueName, DWORD dwValue) const
{
	CRegKey reg;
	if (reg.Create(m_hKeyParent, m_strKeyName) != ERROR_SUCCESS)
		return false;

	if (reg.SetDWORDValue(lpszValueName, dwValue) != ERROR_SUCCESS)
		return false;

	reg.Close();
	return true;
}

bool PhdRegedit::GetValue(LPCTSTR lpszValueName, CString& strValue) const
{
	CRegKey reg;
	if (reg.Open(m_hKeyParent, m_strKeyName) != ERROR_SUCCESS)
		return false;

	DWORD dwBufLen = 4096;
	CString str;
	if (reg.QueryStringValue(lpszValueName, str.GetBuffer(4096),  &dwBufLen) != ERROR_SUCCESS)
		return false;
	str.ReleaseBuffer();
	strValue = str;

	return true;
}

bool PhdRegedit::GetValue(LPCTSTR lpszValueName, DWORD& dwValue) const
{
	CRegKey reg;
	if (reg.Open(m_hKeyParent, m_strKeyName) != ERROR_SUCCESS)
		return false;

	if (reg.QueryDWORDValue( lpszValueName, dwValue) != ERROR_SUCCESS)
		return false;

	return true;
}

bool PhdRegedit::DeleteChildKeyName(LPCTSTR lpszChildKeyName) const
{
	CRegKey reg;
	if (reg.Open(m_hKeyParent, m_strKeyName, KEY_ALL_ACCESS) != ERROR_SUCCESS)
		return false;

	if (reg.RecurseDeleteKey(lpszChildKeyName) != ERROR_SUCCESS)
		return false;

	reg.Close();
	return true;
}

bool PhdRegedit::DeleteValue(LPCTSTR lpszValueName) const
{
	CRegKey reg;
	if (reg.Open(m_hKeyParent, m_strKeyName, KEY_ALL_ACCESS) != ERROR_SUCCESS)
		return false;

	if (reg.DeleteValue(lpszValueName) != ERROR_SUCCESS)
		return false;

	reg.Close();
	return true;
}

bool PhdRegedit::GetAllChildKeyName(std::vector<CString>& vecStrName) const
{
	HKEY  hKeyResult = NULL;
	if (RegOpenKeyEx(m_hKeyParent, m_strKeyName, 0, KEY_READ | KEY_WOW64_64KEY, &hKeyResult) == ERROR_SUCCESS)
	{
		DWORD dwSubKeyCnt = 0;         // 子键的数量  
		DWORD dwSubKeyNameMaxLen = 0;  // 子键名称的最大长度(不包含结尾的null字符)  
		DWORD dwKeyValueCnt = 0;       // 键值项的数量  
		DWORD dwKeyValueNameMaxLen = 0;// 键值项名称的最大长度(不包含结尾的null字符)  
		DWORD dwKeyValueDataMaxLen = 0;// 键值项数据的最大长度(in bytes)  
		int ret = RegQueryInfoKey(hKeyResult, NULL, NULL, NULL, &dwSubKeyCnt, &dwSubKeyNameMaxLen, NULL, &dwKeyValueCnt, &dwKeyValueNameMaxLen,
			&dwKeyValueDataMaxLen, NULL, NULL);
		if (ret != ERROR_SUCCESS) // Error  
			return false;

		LPTSTR lpszSubKeyName = new TCHAR[dwSubKeyNameMaxLen + 1];
		for (DWORD index = 0; index < dwSubKeyCnt; ++index)
		{
			memset(lpszSubKeyName, 0, sizeof(TCHAR)*(dwSubKeyNameMaxLen + 1));
			DWORD dwNameCnt = dwSubKeyNameMaxLen + 1;
			int ret = RegEnumKeyEx(hKeyResult, index, lpszSubKeyName, &dwNameCnt, NULL, NULL, NULL, NULL);
			if (ret != ERROR_SUCCESS)
			{
				delete[] lpszSubKeyName;
				return false;
			}
			CString strName = lpszSubKeyName;
			vecStrName.push_back(strName);
		}
		delete[] lpszSubKeyName;
	}

	//关闭注册表
	::RegCloseKey(hKeyResult);
	return true;
}

}