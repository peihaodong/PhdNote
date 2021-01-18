#pragma once
/***********************************************
   >   Class Name: PhdExcel
   >     Describe: ͨ��com�ӿڿɶ�excel�ļ����ж�д�����ķ�װ��
   >       Author: peihaodong
   > Created Time: 2020��9��5�� 
   >         Blog: https://blog.csdn.net/phd17621680432
 **********************************************/


#include <comdef.h>

 //LPDISPATCH ��һ��ָ��ӿڵ�ָ��
 //_variant_t() ���ַ���תΪconst VARIANT&
 //COleVariant() ��short��long��תΪconst VARIANT&

class _ExApplication;
class Workbooks;
class _Workbook;
class _ExSheets;
class _Worksheet;
class _ExRange;

namespace Phd{

class PHD_NOTE_API PhdExcel
{
public:
	PhdExcel();
	~PhdExcel();

	//����excel
	//ע�⣺������Ĭ����xlsx��ʽ
	bool CreateExcel(LPCTSTR szExcelPath);
	//ͨ��ģ�崴��excel
	//ע�⣺������Ĭ����xlsx��ʽ
	bool CreateExcelByTemplate(LPCTSTR szTemplatePath, LPCTSTR szExcelPath);

	//���´���excelӦ�ó����excel
	bool Open(LPCTSTR szExcelPath);
	//�����Ѵ򿪵�excel
	bool OpenTheAlreadyOpenExcel(LPCTSTR szExcelPath);
	//���excelû�д򿪲���Open��
	bool AutoOpen(LPCTSTR szExcelPath);

	//����excel
	bool Save();
	//���Ϊexcel
	bool SaveAs(LPCTSTR szExcelPath);

	//��ָ��ҳ
	bool OpenSheet(LPCTSTR szSheetName);
	bool OpenSheet(int nSheetIndex);
	//�õ�ҳ������
	int GetSheetCount();
	//ͨ��ҳ�������õ�ҳ������
	CString GetSheetNameByIndex(int nIndex);
	//ͨ��ҳ�����Ƶõ�ҳ������
	int GetSheetIndexByName(LPCTSTR szSheetName);
	//�޸ĵ�ǰsheetҳ��
	bool ModifyCurSheetName(LPCTSTR szNewSheetName);
	//�޸�sheetҳ��
	bool ModifySheetName(int nSheetIndex, LPCTSTR szNewSheetName);
	//�õ���ǰҳ����
	int GetCurSheetIndex();
	//���ҳ
	bool AddSheet(LPCTSTR szSheetName);
	//ɾ��ҳ
	bool DeleteSheet(int nSheetIndex);
	bool DeleteSheet(LPCTSTR szSheetName);
	//�л�ҳ
	bool SwitchSheet(int nSheetIndex);
	bool SwitchSheet(LPCTSTR szSheetName);
	//���õ�ǰҳΪ�ҳ
	bool SetCurSheetActivate();

	//�õ���Ч����
	int GetUsedRowCount();
	//�õ���Ч����
	int GetUsedColCount();
	//�õ���ӡ����
	bool GetPrintArea(int& nRow1, int& nCol1, int& nRow2, int& nCol2);


	//�õ�ָ����Ԫ���иߣ�Ĭ��14.25��
	double GetRowHeight(int nRow, int nCol);
	//�õ�ָ����Ԫ���п�Ĭ��8.38��
	double GetColWidth(int nRow, int nCol);
	//����ָ���е��и�
	bool SetRowHeight(int nRow, double dHeight);
	//����ָ���е��п�
	bool SetColWidth(int nCol, double dWidth);
	//���õ�ǰҳ���е�Ԫ����и�
	bool SetAllRowHeight(double dHeight);
	//���õ�ǰҳ���е�Ԫ����п�
	bool SetAllColWidth(double dWidth);

	//�õ�ָ����Ԫ���ı�
	CString GetCellText(int nRow, int nCol);
	//����ָ����Ԫ���ı�
	bool SetCellText(int nRow, int nCol, LPCTSTR szText);
	//�����ǰҳ���е�Ԫ���ı�����
	bool ClearContents();
	//����ָ����Ԫ��ѡ��
	bool SetCellActivate(int nRow, int nCol);

	//�ϲ���Ԫ��
	bool MergeCell(int nCellRow1, int nCellCol1, int nCellRow2, int nCellCol2,
		bool bCenterAlign);
	//���õ�ǰҳ���е�Ԫ����ж���
	bool SetAllCellCenterAlign();
	//����ָ����Ԫ���Զ�����
	bool SetAutoWrapText(int nRow, int nCol, bool bWrapText);
	//���ñ߿�
	bool SetFrame(int nCellRow1, int nCellCol1, int nCellRow2, int nCellCol2,
		bool bOuterFrame, bool bInnerFrame);
	//����ָ����Ԫ�������С
	bool SetFontSize(int nRow, int nCol, int nSize);
	//����ָ����Ԫ������Ӵ�
	bool SetBoldFont(int nRow, int nCol, bool bBold);
	//����ָ����Ԫ��������ʽ
	bool SetFontType(int nRow, int nCol, LPCTSTR szTextType);

	//����ָ����Ԫ��������ɫ
	bool SetCellTextColor(int nRow, int nCol, int nColorIndex);
	//����ָ����Ԫ�񱳾���ɫ
	bool SetCellBackgroundColor(int nRow, int nCol, int nColorIndex);

	//����һ��
	//ע�⣺���ڵ�nRow�в������У�ԭnRow�б�ΪnRow+1��
	//ע�⣺��������л´����nRow-1�еĸ�ʽ
	bool InsertRow(int nRow);
	//����һ��
	//ע�⣺���ڵ�nCol�в������У�ԭnCol�б�ΪnCol+1��
	//ע�⣺��������л´����nCol-1�еĸ�ʽ
	bool InsertCol(int nCol);
	//ɾ��ָ����
	bool DeleteRow(int nRow);
	//ɾ��ָ����
	bool DeleteCol(int nCol);

	//�ж�excel�ڵ������Ƿ��Ѿ���
	bool IsOpen(LPCTSTR szExcelPath);

	//�رյ�ǰ�򿪵�excel
	bool CloseCurOpenExcel(LPCTSTR szExcelPath);

	//��ʾexcel
	bool ShowExcel();

private:
	//��Ĭ��ҳ
	bool OpenActiveSheet();

	//������
	bool Clear();
	//�˳�excel
	bool Quit();

private:
	//������תΪ��Ӧ�Ĵ�д��ĸ
	//���ִ�1��ʼ����ӦA
	void NumberToUpperStr(int nNumber, CString& strNew);
	//����д��ĸתΪ����
	//��ĸ����׼����0
	int UpperStrToNumber(LPCTSTR szStr);
	//�õ���Ԫ���ַ���
	CString GetCellStr(int nRow, int nCol);
	//�ж��ַ����ǲ������֣��Ӳ�����double��
	bool IsNumber(LPCTSTR sz, bool bAcceptDouble);

private:
	std::shared_ptr<_ExApplication> m_apApp;		//����excelӦ�ó���
	std::shared_ptr<Workbooks> m_apBooks;			//excelӦ�ó����е�ǰ�򿪵�����workbook����

	std::shared_ptr<_Workbook> m_apBook;			//һ��������
	std::shared_ptr<_ExSheets>  m_apSheets;		//ָ���������в������й�����ļ���
	std::shared_ptr<_Worksheet> m_apSheet;			//һ��������
	std::shared_ptr<_ExRange>  m_apRange;			//ĳһ��Ԫ��ĳһ�С�ĳһ�С�ĳһѡ�����򣨸ö������ǰҳ�����е�Ԫ��

	//Interior  m_interior;		//����һ��������ڲ�	
	//_ExFont m_font;			//����������������ԣ��������ơ��ֺš���ɫ�ȣ�
	//Border m_border;			//����߿�
	//PageSetup m_pagesetup;	//ҳ������

private:
	bool m_bExcelIsOpen;

};

}