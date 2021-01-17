#pragma once

/***********************************************
   >   Class Name: PhdRegedit
   >     Describe: �����Ƕ�ע�����ж�д�ķ�װ
   >       Author: peihaodong
   > Created Time: 2020��9��30��
   >         Blog: https://blog.csdn.net/phd17621680432
   >           QQ: 841382590
**********************************************/
namespace Phd{

class PHD_NOTE_API PhdRegedit
{
	//ע���  ��Ҫ��Ϊ����(hKeyParent)���Ӽ�(lpszKeyName)�ͼ�ֵ��������   
	//��ֵ���������������ɣ��ֱ�Ϊ������(lpszValueName)�����͡�����(lpszValue)
	//hKeyParent��	HKEY_CLASSES_ROOT		HKEY_CURRENT_USER		HKEY_LOCAL_MACHINE		HKEY_USERS
public:

	//************************************
	// Summary:   ���ø������Ӽ�
	// Explain:   �Ӽ�����ע����·��
	// Time:      2020��9��29�� peihaodong
	//************************************
	void SetKeyParentAndName(HKEY hKeyParent, LPCTSTR lpszKeyName);

	//************************************
	// Summary:   ���ü�ֵ���ֵ
	// Parameter: 
	//      lpszValueName -    �����ֵ�������
	//      lpszValue -    �����ֵ���ֵ
	// Explain:   
	// Time:      2020��9��29�� peihaodong
	//************************************
	bool SetValue(LPCTSTR lpszValueName, LPCTSTR lpszValue) const;
	bool SetValue(LPCTSTR lpszValueName, DWORD dwValue) const;

	//************************************
	// Summary:   �õ���ֵ���ֵ
	// Parameter: 
	//      lpszValueName -    �����ֵ�������
	//      strValue -    �����ֵ���ֵ
	// Explain:   
	// Time:      2020��9��29�� peihaodong
	//************************************
	bool GetValue(LPCTSTR lpszValueName, CString& strValue) const;
	bool GetValue(LPCTSTR lpszValueName, DWORD& dwValue) const;

	//************************************
	// Summary:   ɾ���Ӽ���
	// Parameter: 
	//      lpszChildKeyName -    �����Ӽ����Ӽ������ƣ�ֱ���������֣�������·����
	// Explain:   
	// Time:      2020��9��29�� peihaodong
	//************************************
	bool DeleteChildKeyName(LPCTSTR lpszChildKeyName) const;

	//************************************
	// Summary:   ɾ����ֵ��
	// Parameter: 
	//      lpszValueName -    �����ֵ�������
	// Explain:   
	// Time:      2020��9��29�� peihaodong
	//************************************
	bool DeleteValue(LPCTSTR lpszValueName) const;

	//************************************
	// Summary:   �õ��Ӽ��������е�������
	// Explain:   
	// Time:      2020��9��29�� peihaodong
	//************************************
	bool GetAllChildKeyName(std::vector<CString>& vecStrName) const;

protected:
	HKEY m_hKeyParent;
	CString m_strKeyName;
};

}

/*
���ӣ�
	CPhdRegedit reg;
	reg.SetKeyParentAndName(HKEY_CURRENT_USER, _T("CNPE"));
	reg.GetRegInt(_T("Balloon_Adjust"), m_bAdjust);
	reg.SetRegInt(_T("Balloon_Adjust"), m_bAdjust);
*/