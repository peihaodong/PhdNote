#pragma once

/***********************************************
   >   Class Name: PhdIni
   >     Describe: 对ini文件读写的封装类
   >       Author: peihaodong
   > Created Time: 2020年9月30日
   >         Blog: https://blog.csdn.net/phd17621680432
   >           QQ: 841382590
**********************************************/
namespace Phd{

class PHD_NOTE_API PhdIni
{
	/*配置文件主要分为三部分：字段名(section)	键名(key)	键值(value)*/
public:

	//************************************
	// Summary:  设置ini文件路径
	// Parameter:
	//      szIniPath -    ini文件路径
	//      szAppdataDirName -    Appdata目录下的目录名
	// Explain:   
	// Time:      2020年9月28日 peihaodong
	//************************************
	void SetIniPath(LPCTSTR szIniPath, LPCTSTR szAppdataDirName);

	//************************************
	// Summary:  读取键值
	// Explain:   
	// Time:      2020年9月28日 peihaodong
	//************************************
	CString GetValue(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault = nullptr) const;
	int GetValue(LPCTSTR szSection, LPCTSTR szKey, int nDefault) const;
	double GetValue(LPCTSTR szSection, LPCTSTR szKey, double fDefault) const;

	//************************************
	// Summary:  写入键值
	// Explain:   
	// Time:      2020年9月28日 peihaodong
	//************************************
	bool SetValue(LPCTSTR szSection,LPCTSTR szKey,LPCTSTR szValue) const;
	bool SetValue(LPCTSTR szSection, LPCTSTR szKey, int value) const;
	bool SetValue(LPCTSTR szSection, LPCTSTR szKey, double value) const;

	//************************************
	// Summary:  得到该配置文件的所有字段名
	// Explain:   
	// Time:      2020年9月28日 peihaodong
	//************************************
	std::vector<CString> GetAllSections() const;

	//************************************
	// Summary:  删除某一字段
	// Explain:   
	// Time:      2020年9月28日 peihaodong
	//************************************
	bool DelSection(LPCTSTR lpSection) const;

	//************************************
	// Summary:  删除所有字段
	// Explain:   
	// Time:      2020年9月28日 peihaodong
	//************************************
	bool DelAllSections() const;

	//************************************
	// Summary:  删除键名
	// Explain:   
	// Time:      2020年9月28日 peihaodong
	//************************************
	bool DelKey(LPCTSTR lpSection, LPCTSTR lpKey) const;

	//************************************
	// Summary:  得到字段下所有的键名和键值
	// Explain:   返回字段下键的总数
	// Time:      2020年9月28日 peihaodong
	//************************************
	int GetSectionData(LPCTSTR lpszSection, std::vector<CString>& vecKey, std::vector<CString>& vecValue) const;

protected:
	//************************************
	// Summary:  读取指定字段下，键的值
	// Parameter:
	//      section -   字段名 
	//      valueName -    键名
	//      sz_default -    键的默认值
	// Explain:   
	// Time:      2020年9月28日 peihaodong
	//************************************
	CString GetIniValue(LPCTSTR section, LPCTSTR valueName, LPCTSTR sz_default) const;

	//************************************
	// Summary:  写入指定字段下，键的值
	// Parameter:
	//      section -    字段名
	//      valueName -    键名
	//      value -    键值
	// Explain:   
	// Time:      2020年9月28日 peihaodong
	//************************************
	bool SetIniValue(LPCTSTR section, LPCTSTR valueName, LPCTSTR value) const;


protected:
	CString m_strIniPath;		
	CString m_strAppdataPath;
};

}