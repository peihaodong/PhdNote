#pragma once

/***********************************************
   >   Class Name: PhdRegedit
   >     Describe: 该类是对注册表进行读写的封装
   >       Author: peihaodong
   > Created Time: 2020年9月30日
   >         Blog: https://blog.csdn.net/phd17621680432
   >           QQ: 841382590
**********************************************/
namespace Phd{

class PHD_NOTE_API PhdRegedit
{
	//注册表  主要分为根键(hKeyParent)、子键(lpszKeyName)和键值项三部分   
	//键值项：它又由三部分组成，分别为：名称(lpszValueName)、类型、数据(lpszValue)
	//hKeyParent：	HKEY_CLASSES_ROOT		HKEY_CURRENT_USER		HKEY_LOCAL_MACHINE		HKEY_USERS
public:

	//************************************
	// Summary:   设置根键和子键
	// Explain:   子键就是注册表的路径
	// Time:      2020年9月29日 peihaodong
	//************************************
	void SetKeyParentAndName(HKEY hKeyParent, LPCTSTR lpszKeyName);

	//************************************
	// Summary:   设置键值项的值
	// Parameter: 
	//      lpszValueName -    输入键值项的名称
	//      lpszValue -    输入键值项的值
	// Explain:   
	// Time:      2020年9月29日 peihaodong
	//************************************
	bool SetValue(LPCTSTR lpszValueName, LPCTSTR lpszValue) const;
	bool SetValue(LPCTSTR lpszValueName, DWORD dwValue) const;

	//************************************
	// Summary:   得到键值项的值
	// Parameter: 
	//      lpszValueName -    输入键值项的名称
	//      strValue -    输出键值项的值
	// Explain:   
	// Time:      2020年9月29日 peihaodong
	//************************************
	bool GetValue(LPCTSTR lpszValueName, CString& strValue) const;
	bool GetValue(LPCTSTR lpszValueName, DWORD& dwValue) const;

	//************************************
	// Summary:   删除子键项
	// Parameter: 
	//      lpszChildKeyName -    输入子键项子级的名称（直接输入名字，而不是路径）
	// Explain:   
	// Time:      2020年9月29日 peihaodong
	//************************************
	bool DeleteChildKeyName(LPCTSTR lpszChildKeyName) const;

	//************************************
	// Summary:   删除键值项
	// Parameter: 
	//      lpszValueName -    输入键值项的名称
	// Explain:   
	// Time:      2020年9月29日 peihaodong
	//************************************
	bool DeleteValue(LPCTSTR lpszValueName) const;

	//************************************
	// Summary:   得到子键项下所有的子项名
	// Explain:   
	// Time:      2020年9月29日 peihaodong
	//************************************
	bool GetAllChildKeyName(std::vector<CString>& vecStrName) const;

protected:
	HKEY m_hKeyParent;
	CString m_strKeyName;
};

}

/*
例子：
	CPhdRegedit reg;
	reg.SetKeyParentAndName(HKEY_CURRENT_USER, _T("CNPE"));
	reg.GetRegInt(_T("Balloon_Adjust"), m_bAdjust);
	reg.SetRegInt(_T("Balloon_Adjust"), m_bAdjust);
*/