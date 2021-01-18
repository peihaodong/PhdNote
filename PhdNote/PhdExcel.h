#pragma once
/***********************************************
   >   Class Name: PhdExcel
   >     Describe: 通过com接口可对excel文件进行读写操作的封装类
   >       Author: peihaodong
   > Created Time: 2020年9月5日 
   >         Blog: https://blog.csdn.net/phd17621680432
 **********************************************/


#include <comdef.h>

 //LPDISPATCH 是一个指向接口的指针
 //_variant_t() 将字符串转为const VARIANT&
 //COleVariant() 将short或long型转为const VARIANT&

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

	//创建excel
	//注意：创建的默认是xlsx格式
	bool CreateExcel(LPCTSTR szExcelPath);
	//通过模板创建excel
	//注意：创建的默认是xlsx格式
	bool CreateExcelByTemplate(LPCTSTR szTemplatePath, LPCTSTR szExcelPath);

	//重新创建excel应用程序绑定excel
	bool Open(LPCTSTR szExcelPath);
	//绑定现已打开的excel
	bool OpenTheAlreadyOpenExcel(LPCTSTR szExcelPath);
	//如果excel没有打开采用Open，
	bool AutoOpen(LPCTSTR szExcelPath);

	//保存excel
	bool Save();
	//另存为excel
	bool SaveAs(LPCTSTR szExcelPath);

	//打开指定页
	bool OpenSheet(LPCTSTR szSheetName);
	bool OpenSheet(int nSheetIndex);
	//得到页的数量
	int GetSheetCount();
	//通过页的索引得到页的名称
	CString GetSheetNameByIndex(int nIndex);
	//通过页的名称得到页的索引
	int GetSheetIndexByName(LPCTSTR szSheetName);
	//修改当前sheet页名
	bool ModifyCurSheetName(LPCTSTR szNewSheetName);
	//修改sheet页名
	bool ModifySheetName(int nSheetIndex, LPCTSTR szNewSheetName);
	//得到当前页索引
	int GetCurSheetIndex();
	//添加页
	bool AddSheet(LPCTSTR szSheetName);
	//删除页
	bool DeleteSheet(int nSheetIndex);
	bool DeleteSheet(LPCTSTR szSheetName);
	//切换页
	bool SwitchSheet(int nSheetIndex);
	bool SwitchSheet(LPCTSTR szSheetName);
	//设置当前页为活动页
	bool SetCurSheetActivate();

	//得到有效行数
	int GetUsedRowCount();
	//得到有效列数
	int GetUsedColCount();
	//得到打印区域
	bool GetPrintArea(int& nRow1, int& nCol1, int& nRow2, int& nCol2);


	//得到指定单元格行高（默认14.25）
	double GetRowHeight(int nRow, int nCol);
	//得到指定单元格列宽（默认8.38）
	double GetColWidth(int nRow, int nCol);
	//设置指定行的行高
	bool SetRowHeight(int nRow, double dHeight);
	//设置指定列的列宽
	bool SetColWidth(int nCol, double dWidth);
	//设置当前页所有单元格的行高
	bool SetAllRowHeight(double dHeight);
	//设置当前页所有单元格的列宽
	bool SetAllColWidth(double dWidth);

	//得到指定单元格文本
	CString GetCellText(int nRow, int nCol);
	//设置指定单元格文本
	bool SetCellText(int nRow, int nCol, LPCTSTR szText);
	//清除当前页所有单元格文本内容
	bool ClearContents();
	//设置指定单元格被选中
	bool SetCellActivate(int nRow, int nCol);

	//合并单元格
	bool MergeCell(int nCellRow1, int nCellCol1, int nCellRow2, int nCellCol2,
		bool bCenterAlign);
	//设置当前页所有单元格居中对齐
	bool SetAllCellCenterAlign();
	//设置指定单元格自动换行
	bool SetAutoWrapText(int nRow, int nCol, bool bWrapText);
	//设置边框
	bool SetFrame(int nCellRow1, int nCellCol1, int nCellRow2, int nCellCol2,
		bool bOuterFrame, bool bInnerFrame);
	//设置指定单元格字体大小
	bool SetFontSize(int nRow, int nCol, int nSize);
	//设置指定单元格字体加粗
	bool SetBoldFont(int nRow, int nCol, bool bBold);
	//设置指定单元格字体样式
	bool SetFontType(int nRow, int nCol, LPCTSTR szTextType);

	//设置指定单元格字体颜色
	bool SetCellTextColor(int nRow, int nCol, int nColorIndex);
	//设置指定单元格背景颜色
	bool SetCellBackgroundColor(int nRow, int nCol, int nColorIndex);

	//插入一行
	//注意：会在第nRow行插入新行，原nRow行变为nRow+1行
	//注意：插入的新行会拷贝第nRow-1行的格式
	bool InsertRow(int nRow);
	//插入一列
	//注意：会在第nCol列插入新列，原nCol列变为nCol+1列
	//注意：插入的新列会拷贝第nCol-1列的格式
	bool InsertCol(int nCol);
	//删除指定行
	bool DeleteRow(int nRow);
	//删除指定列
	bool DeleteCol(int nCol);

	//判断excel在电脑上是否已经打开
	bool IsOpen(LPCTSTR szExcelPath);

	//关闭当前打开的excel
	bool CloseCurOpenExcel(LPCTSTR szExcelPath);

	//显示excel
	bool ShowExcel();

private:
	//打开默认页
	bool OpenActiveSheet();

	//清理缓存
	bool Clear();
	//退出excel
	bool Quit();

private:
	//将数字转为对应的大写字母
	//数字从1开始，对应A
	void NumberToUpperStr(int nNumber, CString& strNew);
	//将大写字母转为数字
	//字母不标准返回0
	int UpperStrToNumber(LPCTSTR szStr);
	//得到单元格字符串
	CString GetCellStr(int nRow, int nCol);
	//判断字符串是不是数字，接不接受double型
	bool IsNumber(LPCTSTR sz, bool bAcceptDouble);

private:
	std::shared_ptr<_ExApplication> m_apApp;		//整个excel应用程序
	std::shared_ptr<Workbooks> m_apBooks;			//excel应用程序中当前打开的所有workbook集合

	std::shared_ptr<_Workbook> m_apBook;			//一个工作簿
	std::shared_ptr<_ExSheets>  m_apSheets;		//指定或活动工作中簿中所有工作表的集合
	std::shared_ptr<_Worksheet> m_apSheet;			//一个工作表
	std::shared_ptr<_ExRange>  m_apRange;			//某一单元格、某一行、某一列、某一选定区域（该对象代表当前页的所有单元格）

	//Interior  m_interior;		//代表一个对象的内部	
	//_ExFont m_font;			//包含对象的字体属性（字体名称、字号、颜色等）
	//Border m_border;			//内外边框
	//PageSetup m_pagesetup;	//页面设置

private:
	bool m_bExcelIsOpen;

};

}