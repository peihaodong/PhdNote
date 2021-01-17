#pragma once

/***********************************************
   >   Class Name: PhdConver
   >     Describe: 这个一个用于转换的类
   >       Author: peihaodong
   > Created Time: 2020年9月30日
   >         Blog: https://blog.csdn.net/phd17621680432
   >           QQ: 841382590
**********************************************/
namespace Phd{

class PHD_NOTE_API PhdConver
{
public:

	//************************************
	// Summary:   int型和字符串之间的转换
	// Explain:   
	// Time:      2020年9月29日 peihaodong
	//************************************
	CString IntToStr(int nValue) const;
	int StrToInt(LPCTSTR szStr) const;

	CString DoubleToStr(double dValue) const;
	double StrToDouble(LPCTSTR szStr) const;

	//************************************
	// Summary:   指针（地址）转为字符串
	// Explain:   
	// Time:      2020年9月29日 peihaodong
	//************************************
	CString PtrToStr(void* ptr) const;

	//************************************
	// Summary:   角度和弧度之间的转换
	// Explain:   
	// Time:      2020年9月29日 peihaodong
	//************************************
	double AngleToRadian(double dAngle) const;
	double RadianToAngle(double dRadian) const;

	//************************************
	// Summary:   字符和ASCII码之间的转换
	// Explain:   
	// Time:      2020年9月29日 peihaodong
	//************************************
	int CharToASCII(TCHAR s) const;
	TCHAR ASCIIToChar(int n) const;

	//功能：浮点数转整数（四舍五入）
	int DoubleToInt(double dValue) const;
	//功能：浮点数转整数（向上取整）
	int DoubleToIntOfTop(double dValue) const;
	//功能：浮点数转整数（向下取整）
	int DoubleToIntOfBottom(double dValue) const;

#pragma region 多字节 宽字节 之间的转换

	//************************************
	// Summary:   多字节字符串转宽字节字符串
	// Explain:   需要手动delete
	// Time:      2020年9月29日 peihaodong
	//************************************
	wchar_t* CToW(char* str) const;

	//************************************
	// Summary:   宽字节字符串转多字节字符串
	// Explain:   需要手动delete
	// Time:      2020年9月29日 peihaodong
	//************************************
	char* WToC(wchar_t* str) const;

	std::wstring CToW_STL(const std::string& str) const;
	std::string WToC_STL(const std::wstring& str) const;

#pragma endregion

};

}