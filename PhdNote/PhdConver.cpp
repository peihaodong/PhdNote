#include "pch.h"
#include "PhdConver.h"
#include <clocale>

namespace Phd{

CString PhdConver::IntToStr(int nValue) const
{
	TCHAR sz[10];
	_itot_s(nValue, sz, 10);
	return sz;
}

int PhdConver::StrToInt(LPCTSTR szStr) const
{
	return _ttoi(szStr);
}

CString PhdConver::DoubleToStr(double dValue) const
{
	CString strValue;
	strValue.Format(_T("%G"), dValue);
	return strValue;
}

double PhdConver::StrToDouble(LPCTSTR szStr) const
{
	return _ttof(szStr);
}

CString PhdConver::PtrToStr(void* ptr) const
{
	CString str;
	str.Format(_T("%p"), ptr);
	return str;
}

double PhdConver::AngleToRadian(double dAngle) const
{
	double dPI = (std::atan(1.0) * 4);
	return(dAngle * (dPI / 180.0));
}

double PhdConver::RadianToAngle(double dRadian) const
{
	double dPI = (std::atan(1.0) * 4);
	return(dRadian * (180.0 / dPI));
}

int PhdConver::CharToASCII(TCHAR s) const
{
	return s;
}

TCHAR PhdConver::ASCIIToChar(int n) const
{
	return n;
}

int PhdConver::DoubleToInt(double dValue) const
{
	double dNewValue = std::floor(dValue + 0.5);
	return static_cast<int>(dNewValue);
}

int PhdConver::DoubleToIntOfTop(double dValue) const
{
	double dNewValue = std::ceil(dValue);
	return static_cast<int>(dNewValue);
}

int PhdConver::DoubleToIntOfBottom(double dValue) const
{
	double dNewValue = std::floor(dValue);
	return static_cast<int>(dNewValue);
}

wchar_t* PhdConver::CToW(char* str) const
{
	//计算char *数组大小，以字节为单位，一个汉字占两个字节
	std::size_t charLen = std::strlen(str);

	//计算多字节字符的大小，按字符计算。
	std::size_t len = ::MultiByteToWideChar(CP_ACP, 0, str, charLen, NULL, 0);

	//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
	wchar_t* buf = new wchar_t[len + 1];

	//多字节编码转换成宽字节编码
	::MultiByteToWideChar(CP_ACP, 0, str, charLen, buf, len);

	buf[len] = '\0'; //添加字符串结尾，注意不是len+1

	return buf;
}

char* PhdConver::WToC(wchar_t* str) const
{
	//计算wchar_t *数组大小，以字节为单位，一个汉字占两个字节
	std::size_t wcharLen = std::wcslen(str);

	//获取宽字节字符的大小，大小是按字节计算的
	std::size_t len = WideCharToMultiByte(CP_ACP, 0, str, wcharLen, NULL, 0, NULL, NULL);

	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	char* buf = new char[len + 1]; //以字节为单位

	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP, 0, str, wcharLen, buf, len, NULL, NULL);

	buf[len] = '\0'; //多字节字符以'\0'结束

	return buf;
}

std::wstring PhdConver::CToW_STL(const std::string& str) const
{
	size_t i;
	std::string curLocale = std::setlocale(LC_ALL, NULL);
	std::setlocale(LC_ALL, "chs");
	const char* _source = str.c_str();
	size_t _dsize = str.size() + 1;
	wchar_t* _dest = new wchar_t[_dsize];
	std::wmemset(_dest, 0x0, _dsize);
	mbstowcs_s(&i, _dest, _dsize, _source, _dsize);
	std::wstring result = _dest;
	delete[] _dest;
	std::setlocale(LC_ALL, curLocale.c_str());
	return result;
}

std::string PhdConver::WToC_STL(const std::wstring& str) const
{
	size_t i;
	std::string curLocale = std::setlocale(LC_ALL, NULL);
	std::setlocale(LC_ALL, "chs");
	const wchar_t* _source = str.c_str();
	size_t _dsize = 2 * str.size() + 1;
	char* _dest = new char[_dsize];
	std::memset(_dest, 0x0, _dsize);
	wcstombs_s(&i, _dest, _dsize, _source, _dsize);
	std::string result = _dest;
	delete[] _dest;
	std::setlocale(LC_ALL, curLocale.c_str());
	return result;
}

}