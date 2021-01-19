#pragma once

/***********************************************
   >   Class Name: CTestIni
   >     Describe: 这是一个使用PhdIni类的例子类，使用单例模式，生成一个全局对象，通过它访问ini文件
   >       Author: peihaodong
   > Created Time: 2020年9月30日
   >         Blog: https://blog.csdn.net/phd17621680432
   >           QQ: 841382590
**********************************************/
namespace Phd{
class PhdIni;
}

class CTestIni
{
public:
	static CTestIni& GetInstance();

public:
	//齿轮-精度
	CString GetClJd() const;
	bool SetClJd(LPCTSTR szValue) const;

private:
	CTestIni(LPCTSTR szIniPath, LPCTSTR szAppdataDirName);

private:
	 std::shared_ptr<Phd::PhdIni> m_apIni;
};

#define g_IniTest CTestIni::GetInstance()
