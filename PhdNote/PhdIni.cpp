#include "pch.h"
#include "PhdIni.h"

namespace Phd{

#define MAX_ALLKEYS 6000  //ȫ���ļ���
#define MAX_KEY 260  //һ����������
#define MAX_ALLSECTIONS 2048  //ȫ���Ķ���
#define MAX_SECTION 260  //һ����������

void PhdIni::SetIniPath(LPCTSTR szIniPath, LPCTSTR szAppdataDirName)
{
	m_strIniPath = szIniPath;

	LPTSTR pszPath = NULL;
	pszPath = ::PathFindFileNameW(szIniPath);//��ȡ·�����ļ���

	//�õ�AppdataĿ¼
	TCHAR szAppdataDir[_MAX_PATH];
	::SHGetSpecialFolderPath(nullptr, szAppdataDir, CSIDL_APPDATA, TRUE);
	m_strAppdataPath = szAppdataDir;
	m_strAppdataPath += szAppdataDirName;
	::CreateDirectory(m_strAppdataPath, nullptr);//������Ŀ¼

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
	::_itot_s(nDefault, sz, 10);//int��תΪ�ַ���
	const auto result = GetIniValue(szSection, szKey, sz);
	return _ttoi(result);//�ַ���תΪint��
}

double PhdIni::GetValue(LPCTSTR szSection, LPCTSTR szKey, double fDefault) const
{
	CString strDefault;
	strDefault.Format(_T("%G"), fDefault);//double��ת�ַ���

	const auto result = GetIniValue(szSection, szKey, strDefault);
	return _ttof(result);//�ַ�תdouble��
}

bool PhdIni::SetValue(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue) const
{
	return SetIniValue(szSection, szKey, szValue);
}

bool PhdIni::SetValue(LPCTSTR szSection, LPCTSTR szKey, int value) const
{
	TCHAR sz[10];
	_itot_s(value, sz, 10);//��int��תΪ�ַ�����
	return SetIniValue(szSection, szKey, sz);
}

bool PhdIni::SetValue(LPCTSTR szSection, LPCTSTR szKey, double value) const
{
	CString strValue;
	strValue.Format(_T("%G"), value);//��double��תΪ�ַ�����
	return SetIniValue(szSection, szKey, strValue);
}

std::vector<CString> PhdIni::GetAllSections() const
{
	/*
	������������
	GetPrivateProfileSectionNames - �� ini �ļ��л�� Section ������
	��� ini �������� Section: [sec1] �� [sec2]���򷵻ص��� 'sec1',0,'sec2',0,0 �����㲻֪��
	ini ������Щ section ��ʱ���������� api ����ȡ����
	*/
	std::vector<CString> vecSection;

	int i;
	int iPos = 0;
	int iMaxCount;
	TCHAR chSectionNames[MAX_ALLSECTIONS] = { 0 }; //�ܵ���������ַ���
	TCHAR chSection[MAX_SECTION] = { 0 }; //���һ��������
	GetPrivateProfileSectionNames(chSectionNames, MAX_ALLSECTIONS, m_strIniPath);

	//����ѭ�����ضϵ�����������0
	for (i = 0; i < MAX_ALLSECTIONS; i++)
	{
		if (chSectionNames[i] == 0)
			if (chSectionNames[i] == chSectionNames[i + 1])
				break;
	}

	iMaxCount = i + 1; //Ҫ��һ��0��Ԫ�ء����ҳ�ȫ���ַ����Ľ������֡�

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
	������������
	GetPrivateProfileSection- �� ini �ļ��л��һ��Section��ȫ��������ֵ��
	���ini����һ���Σ������� "��1=ֵ1" "��2=ֵ2"���򷵻ص��� '��1=ֵ1',0,'��2=ֵ2',0,0 �����㲻֪��
	���һ�����е����м���ֵ�����������
	*/
	int i = 0;
	int iPos = 0;
	CString strKeyValue;
	int iMaxCount = 0;
	TCHAR chKeyNames[MAX_ALLKEYS] = { 0 }; //�ܵ���������ַ���
	TCHAR chKey[MAX_KEY] = { 0 }; //�������һ������

	GetPrivateProfileSection(lpszSection, chKeyNames, MAX_ALLKEYS, m_strIniPath);

	for (i = 0; i < MAX_ALLKEYS; i++)
	{
		if (chKeyNames[i] == 0)
			if (chKeyNames[i] == chKeyNames[i + 1])
				break;
	}

	iMaxCount = i + 1; //Ҫ��һ��0��Ԫ�ء����ҳ�ȫ���ַ����Ľ������֡�

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
	//��ȥAppdataĿ¼����
	auto dwRs = ::GetPrivateProfileString(section, valueName, sz_default, value.GetBuffer(256), 256, m_strAppdataPath);
	value.ReleaseBuffer();
	if (0x2 != GetLastError())
	{
		// �ɹ�
		return value;
	}
	//���appdataĿ¼��û�У����ڱ���Ŀ¼����
	dwRs = ::GetPrivateProfileString(section, valueName, sz_default, value.GetBuffer(256), 256, m_strIniPath);
	value.ReleaseBuffer();
	if (0x2 != GetLastError())
	{
		//�ɹ�
		return value;
	}
	//�����û�У���ѡ��Ĭ��ֵ
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