#pragma once

/***********************************************
   >   Class Name: PhdConver
   >     Describe: ���һ������ת������
   >       Author: peihaodong
   > Created Time: 2020��9��30��
   >         Blog: https://blog.csdn.net/phd17621680432
   >           QQ: 841382590
**********************************************/
namespace Phd{

class PHD_NOTE_API PhdConver
{
public:

	//************************************
	// Summary:   int�ͺ��ַ���֮���ת��
	// Explain:   
	// Time:      2020��9��29�� peihaodong
	//************************************
	CString IntToStr(int nValue) const;
	int StrToInt(LPCTSTR szStr) const;

	CString DoubleToStr(double dValue) const;
	double StrToDouble(LPCTSTR szStr) const;

	//************************************
	// Summary:   ָ�루��ַ��תΪ�ַ���
	// Explain:   
	// Time:      2020��9��29�� peihaodong
	//************************************
	CString PtrToStr(void* ptr) const;

	//************************************
	// Summary:   �ǶȺͻ���֮���ת��
	// Explain:   
	// Time:      2020��9��29�� peihaodong
	//************************************
	double AngleToRadian(double dAngle) const;
	double RadianToAngle(double dRadian) const;

	//************************************
	// Summary:   �ַ���ASCII��֮���ת��
	// Explain:   
	// Time:      2020��9��29�� peihaodong
	//************************************
	int CharToASCII(TCHAR s) const;
	TCHAR ASCIIToChar(int n) const;

	//���ܣ�������ת�������������룩
	int DoubleToInt(double dValue) const;
	//���ܣ�������ת����������ȡ����
	int DoubleToIntOfTop(double dValue) const;
	//���ܣ�������ת����������ȡ����
	int DoubleToIntOfBottom(double dValue) const;

#pragma region ���ֽ� ���ֽ� ֮���ת��

	//************************************
	// Summary:   ���ֽ��ַ���ת���ֽ��ַ���
	// Explain:   ��Ҫ�ֶ�delete
	// Time:      2020��9��29�� peihaodong
	//************************************
	wchar_t* CToW(char* str) const;

	//************************************
	// Summary:   ���ֽ��ַ���ת���ֽ��ַ���
	// Explain:   ��Ҫ�ֶ�delete
	// Time:      2020��9��29�� peihaodong
	//************************************
	char* WToC(wchar_t* str) const;

	std::wstring CToW_STL(const std::string& str) const;
	std::string WToC_STL(const std::wstring& str) const;

#pragma endregion

};

}