#pragma once
#include "PhdIni.h"

/***********************************************
   >   Class Name: CTestIni
   >     Describe: ����һ��ʹ��PhdIni��������࣬ʹ�õ���ģʽ������һ��ȫ�ֶ���ͨ��������ini�ļ�
   >       Author: peihaodong
   > Created Time: 2020��9��30��
   >         Blog: https://blog.csdn.net/phd17621680432
   >           QQ: 841382590
**********************************************/
class CTestIni
{
public:
	static CTestIni& GetInstance();

public:
	//����-����
	CString GetClJd() const;
	bool SetClJd(LPCTSTR szValue) const;

private:
	CTestIni(LPCTSTR szIniPath, LPCTSTR szAppdataDirName);

private:
	Phd::PhdIni m_ini;
};

#define g_IniTest CTestIni::GetInstance()