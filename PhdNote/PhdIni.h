#pragma once

/***********************************************
   >   Class Name: PhdIni
   >     Describe: ��ini�ļ���д�ķ�װ��
   >       Author: peihaodong
   > Created Time: 2020��9��30��
   >         Blog: https://blog.csdn.net/phd17621680432
   >           QQ: 841382590
**********************************************/
namespace Phd{

class PHD_NOTE_API PhdIni
{
	/*�����ļ���Ҫ��Ϊ�����֣��ֶ���(section)	����(key)	��ֵ(value)*/
public:

	//************************************
	// Summary:  ����ini�ļ�·��
	// Parameter:
	//      szIniPath -    ini�ļ�·��
	//      szAppdataDirName -    AppdataĿ¼�µ�Ŀ¼��
	// Explain:   
	// Time:      2020��9��28�� peihaodong
	//************************************
	void SetIniPath(LPCTSTR szIniPath, LPCTSTR szAppdataDirName);

	//************************************
	// Summary:  ��ȡ��ֵ
	// Explain:   
	// Time:      2020��9��28�� peihaodong
	//************************************
	CString GetValue(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault = nullptr) const;
	int GetValue(LPCTSTR szSection, LPCTSTR szKey, int nDefault) const;
	double GetValue(LPCTSTR szSection, LPCTSTR szKey, double fDefault) const;

	//************************************
	// Summary:  д���ֵ
	// Explain:   
	// Time:      2020��9��28�� peihaodong
	//************************************
	bool SetValue(LPCTSTR szSection,LPCTSTR szKey,LPCTSTR szValue) const;
	bool SetValue(LPCTSTR szSection, LPCTSTR szKey, int value) const;
	bool SetValue(LPCTSTR szSection, LPCTSTR szKey, double value) const;

	//************************************
	// Summary:  �õ��������ļ��������ֶ���
	// Explain:   
	// Time:      2020��9��28�� peihaodong
	//************************************
	std::vector<CString> GetAllSections() const;

	//************************************
	// Summary:  ɾ��ĳһ�ֶ�
	// Explain:   
	// Time:      2020��9��28�� peihaodong
	//************************************
	bool DelSection(LPCTSTR lpSection) const;

	//************************************
	// Summary:  ɾ�������ֶ�
	// Explain:   
	// Time:      2020��9��28�� peihaodong
	//************************************
	bool DelAllSections() const;

	//************************************
	// Summary:  ɾ������
	// Explain:   
	// Time:      2020��9��28�� peihaodong
	//************************************
	bool DelKey(LPCTSTR lpSection, LPCTSTR lpKey) const;

	//************************************
	// Summary:  �õ��ֶ������еļ����ͼ�ֵ
	// Explain:   �����ֶ��¼�������
	// Time:      2020��9��28�� peihaodong
	//************************************
	int GetSectionData(LPCTSTR lpszSection, std::vector<CString>& vecKey, std::vector<CString>& vecValue) const;

protected:
	//************************************
	// Summary:  ��ȡָ���ֶ��£�����ֵ
	// Parameter:
	//      section -   �ֶ��� 
	//      valueName -    ����
	//      sz_default -    ����Ĭ��ֵ
	// Explain:   
	// Time:      2020��9��28�� peihaodong
	//************************************
	CString GetIniValue(LPCTSTR section, LPCTSTR valueName, LPCTSTR sz_default) const;

	//************************************
	// Summary:  д��ָ���ֶ��£�����ֵ
	// Parameter:
	//      section -    �ֶ���
	//      valueName -    ����
	//      value -    ��ֵ
	// Explain:   
	// Time:      2020��9��28�� peihaodong
	//************************************
	bool SetIniValue(LPCTSTR section, LPCTSTR valueName, LPCTSTR value) const;


protected:
	CString m_strIniPath;		
	CString m_strAppdataPath;
};

}