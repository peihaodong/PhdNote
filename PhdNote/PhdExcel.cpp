#include "pch.h"
#include "PhdExcel.h"
#include "excel9.h"

namespace Phd{

PhdExcel::PhdExcel()
	:m_apApp(std::make_shared<_ExApplication>())
	, m_apBooks(std::make_shared<Workbooks>())
	, m_apBook(std::make_shared<_Workbook>())
	, m_apSheets(std::make_shared<_ExSheets>())
	, m_apSheet(std::make_shared<_Worksheet>())
	, m_apRange(std::make_shared<_ExRange>())
{
}

PhdExcel::~PhdExcel()
{
	if (m_bExcelIsOpen)
		Clear();
	else
		Quit();
}

bool PhdExcel::CreateExcel(LPCTSTR szExcelPath)
{
	CoInitialize(NULL);

	if (!m_apApp->CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(_T("创建Excel失败!"));
		exit(1);
		return false;
	}

	try
	{
		m_apApp->SetVisible(false);
		//将当前电脑上打开的所有excel工作簿绑定m_books
		m_apBooks->AttachDispatch(m_apApp->GetWorkbooks(), true);
		//绑定新增的工作簿
		m_apBook->AttachDispatch(m_apBooks->Add(vtMissing));
		//绑定m_sheets
		m_apSheets->AttachDispatch(m_apBook->GetWorksheets(), true);
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}

	//另存为
	SaveAs(szExcelPath);

	return true;
}

bool PhdExcel::CreateExcelByTemplate(LPCTSTR szTemplatePath, LPCTSTR szExcelPath)
{
	CoInitialize(NULL);

	if (!m_apApp->CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(_T("创建Excel失败!"));
		exit(1);
		return false;
	}

	try
	{
		m_apApp->SetVisible(false);
		//将当前电脑上打开的所有excel工作簿绑定m_books
		m_apBooks->AttachDispatch(m_apApp->GetWorkbooks(), true);
		//绑定新增的工作簿
		m_apBook->AttachDispatch(m_apBooks->Add(_variant_t(szTemplatePath)));
		//绑定m_sheets
		m_apSheets->AttachDispatch(m_apBook->GetWorksheets(), true);
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}

	//另存为
	SaveAs(szExcelPath);

	return true;
}

bool PhdExcel::Open(LPCTSTR szExcelPath)
{
	CoInitialize(NULL);

	if (!m_apApp->CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(_T("创建Excel失败!"));
		exit(1);
		return false;
	}

	try
	{
		m_apApp->SetVisible(false);
		//将当前电脑上打开的所有excel工作簿绑定m_books
		m_apBooks->AttachDispatch(m_apApp->GetWorkbooks(), true);
		//绑定指定的工作簿
		LPDISPATCH book = m_apBooks->Open(szExcelPath, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing);
		_Workbook* pBook = new _Workbook(book);
		m_apBook.reset(pBook);
// 		m_book = m_apBooks->Open(szExcelPath, vtMissing, vtMissing,
// 			vtMissing, vtMissing, vtMissing, vtMissing,
// 			vtMissing, vtMissing, vtMissing, vtMissing,
// 			vtMissing, vtMissing);
		//绑定m_sheets
		m_apSheets->AttachDispatch(m_apBook->GetWorksheets(), true);
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}

	//绑定m_sheet和m_range
	if (!OpenActiveSheet())
		return false;

	m_bExcelIsOpen = false;
	return true;
}

bool PhdExcel::OpenTheAlreadyOpenExcel(LPCTSTR szExcelPath)
{
	//初始化com库，以单线程的方式创建com对象
	CoInitialize(NULL);

	LPDISPATCH lpDisp = NULL;
	CLSID clsid;
	HRESULT hr;
	hr = ::CLSIDFromProgID(_T("Excel.Application"), &clsid); //通过ProgID取得CLSID
	if (FAILED(hr))
		return FALSE;//电脑上没有excel软件

	IUnknown *pUnknown = NULL;
	IDispatch *pDispatch = NULL;
	hr = ::GetActiveObject(clsid, NULL, &pUnknown); //查找是否有程序在运行
	if (FAILED(hr))
		return false;//没有excel软件在运行
	else
	{//有excel软件在运行
		try
		{
			_ExApplication* pApp = nullptr;
			hr = pUnknown->QueryInterface(IID_IDispatch, (LPVOID *)pApp);
			if (FAILED(hr))
				throw(_T("没有取得IDispatchPtr"));
			m_apApp.reset(pApp);
			//hr = pUnknown->QueryInterface(IID_IDispatch, (LPVOID *)&m_app);
			if (FAILED(hr))
				throw(_T("没有取得IDispatchPtr"));
			pUnknown->Release();
			pUnknown = NULL;

			lpDisp = m_apApp->GetWorkbooks();
			m_apBooks->AttachDispatch(lpDisp, TRUE);
			int nLen = m_apBooks->GetCount();
			CString strName;
			bool bIsFind = false;
			for (int i = 1; i <= nLen; i++)
			{
				m_apBook->ReleaseDispatch();
				lpDisp = m_apBooks->GetItem(_variant_t(i));
				m_apBook->AttachDispatch(lpDisp, TRUE);
				strName = m_apBook->GetFullName();

				if (_tcscmp(strName, szExcelPath) == 0)
				{
					bIsFind = true;
					break;
				}
			}
			if (!bIsFind)
			{
				m_apBook->ReleaseDispatch();
				m_apBooks->ReleaseDispatch();
				m_apApp->ReleaseDispatch();
				CoUninitialize();//关闭当前线程上的com库，卸载该线程加载的所有dll,释放该线程维护的所有其他资源，并强制关闭该线程上的所有RPC连接。
				return false;//没找到
			}

			//绑定m_sheets
			m_apSheets->AttachDispatch(m_apBook->GetWorksheets(), true);
		}
		catch (CException* e)
		{
			TCHAR szError[1024];
			e->GetErrorMessage(szError, 1024);
			return false;
		}
	}

	//绑定m_sheet和m_range
	if (!OpenActiveSheet())
		return false;

	m_bExcelIsOpen = true;
	return true;
}

bool PhdExcel::AutoOpen(LPCTSTR szExcelPath)
{
	if (!IsOpen(szExcelPath))
	{
		return Open(szExcelPath);
	}
	else
	{
		return OpenTheAlreadyOpenExcel(szExcelPath);
	}
}

bool PhdExcel::IsOpen(LPCTSTR szExcelPath)
{
	LPDISPATCH lpDisp = NULL;
	CoInitialize(NULL);
	CLSID clsid;
	HRESULT hr;
	hr = ::CLSIDFromProgID(_T("Excel.Application"), &clsid); //通过ProgID取得CLSID
	if (FAILED(hr))
	{
		return FALSE;//电脑上没有excel软件
	}

	IUnknown *pUnknown = NULL;
	IDispatch *pDispatch = NULL;
	hr = ::GetActiveObject(clsid, NULL, &pUnknown); //查找是否有程序在运行
	if (FAILED(hr))
	{//没有excel软件在运行
		return false;
	}
	else
	{//有excel软件在运行
		try
		{
			_ExApplication appTemp;
			hr = pUnknown->QueryInterface(IID_IDispatch, (LPVOID *)&appTemp);
			if (FAILED(hr))
				throw(_T("没有取得IDispatchPtr"));
			pUnknown->Release();
			pUnknown = NULL;

			lpDisp = appTemp.GetWorkbooks();
			Workbooks booksTemp;
			booksTemp.AttachDispatch(lpDisp, TRUE);
			int nLen = booksTemp.GetCount();
			CString strName;
			bool bIsFind = false;
			_Workbook bookTemp;
			for (int i = 1; i <= nLen; i++)
			{
				if (bookTemp)
					bookTemp.ReleaseDispatch();
				lpDisp = booksTemp.GetItem(_variant_t(i));
				bookTemp.AttachDispatch(lpDisp, TRUE);
				strName = bookTemp.GetFullName();

				if (_tcscmp(strName, szExcelPath) == 0)
				{
					bIsFind = true;
					break;
				}
			}
			//
			bookTemp.ReleaseDispatch();
			booksTemp.ReleaseDispatch();
			appTemp.ReleaseDispatch();
			//关闭当前线程上的com库，卸载该线程加载的所有dll,释放该线程维护的所有其他资源，并强制关闭该线程上的所有RPC连接。
			CoUninitialize();
			//
			if (!bIsFind)
				return false;//没找到
			else
				return true;
		}
		catch (CException* e)
		{
			TCHAR szError[1024];
			e->GetErrorMessage(szError, 1024);
			return false;
		}
	}
}

bool PhdExcel::OpenActiveSheet()
{
	try
	{
		m_apSheet->AttachDispatch(m_apBook->GetActiveSheet(), true);
		m_apRange->AttachDispatch(m_apSheet->GetCells(), true);
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}

	return true;
}

bool PhdExcel::OpenSheet(LPCTSTR szSheetName)
{
	try
	{
		m_apSheet->ReleaseDispatch();
		m_apSheet->AttachDispatch(m_apSheets->GetItem(_variant_t(szSheetName)), true);
		m_apRange->ReleaseDispatch();
		m_apRange->AttachDispatch(m_apSheet->GetCells(), true);
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}

	return true;
}

bool PhdExcel::OpenSheet(int nIndex)
{
	try
	{
		m_apSheet->ReleaseDispatch();
		m_apSheet->AttachDispatch(m_apSheets->GetItem(COleVariant((long)nIndex)), true);
		m_apRange->ReleaseDispatch();
		m_apRange->AttachDispatch(m_apSheet->GetCells(), true);
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}

	return true;
}

bool PhdExcel::Save()
{
	try
	{
		//设置不提示是否覆盖警报
		m_apApp->SetAlertBeforeOverwriting(FALSE);
		//设置不显示警报
		m_apApp->SetDisplayAlerts(FALSE);
		//保存工作簿
		m_apBook->Save();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::SaveAs(LPCTSTR szExcelPath)
{
	try
	{
		//设置不提示是否覆盖警报
		m_apApp->SetAlertBeforeOverwriting(FALSE);
		//设置不显示警报
		m_apApp->SetDisplayAlerts(FALSE);
		//另存为工作簿
		m_apBook->SaveAs(_variant_t(szExcelPath), vtMissing, vtMissing, vtMissing, vtMissing, vtMissing
			, 0, vtMissing, vtMissing, vtMissing, vtMissing);
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::Clear()
{
	try
	{
		m_apRange->ReleaseDispatch();
		m_apSheet->ReleaseDispatch();
		m_apSheets->ReleaseDispatch();
		m_apBook->ReleaseDispatch();
		m_apBooks->ReleaseDispatch();
		m_apApp->ReleaseDispatch();

		CoUninitialize();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::Quit()
{
	try
	{
		m_apApp->SetVisible(FALSE);
		m_apApp->SetDisplayAlerts(FALSE);

		m_apBooks->Close();	//关闭工作簿集合
		m_apApp->Quit();		//退出excel应用程序

		m_apRange->ReleaseDispatch();
		m_apSheet->ReleaseDispatch();
		m_apSheets->ReleaseDispatch();
		m_apBook->ReleaseDispatch();
		m_apBooks->ReleaseDispatch();
		m_apApp->ReleaseDispatch();

		CoUninitialize();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

int PhdExcel::GetSheetCount()
{
	long lCount = 0;
	try
	{
		lCount = m_apSheets->GetCount();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return 0;
	}
	return lCount;
}

CString PhdExcel::GetSheetNameByIndex(int nIndex)
{
	CString strSheetName;
	try
	{
		_Worksheet sheetTemp;
		sheetTemp.AttachDispatch(m_apSheets->GetItem(COleVariant((long)(nIndex + 1))), true);
		strSheetName = sheetTemp.GetName();
		sheetTemp.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return _T("");
	}
	return strSheetName;
}

int PhdExcel::GetSheetIndexByName(LPCTSTR szSheetName)
{
	try
	{
		int nSheetCount = GetSheetCount();
		for (int i = 0; i < nSheetCount; i++)
		{
			CString strName = GetSheetNameByIndex(i);
			if (_tcscmp(szSheetName, strName) == 0)
			{
				return i;
			}
		}
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return -1;
	}

	return -1;
}

bool PhdExcel::ModifyCurSheetName(LPCTSTR szNewSheetName)
{
	try
	{
		m_apSheet->SetName(szNewSheetName);
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::ModifySheetName(int nSheetIndex, LPCTSTR szNewSheetName)
{
	try
	{
		_Worksheet sheetTemp;
		sheetTemp.AttachDispatch(m_apSheets->GetItem(COleVariant((long)(nSheetIndex + 1))), true);
		sheetTemp.SetName(szNewSheetName);
		sheetTemp.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

int PhdExcel::GetCurSheetIndex()
{
	CString strSheetName = m_apSheet->GetName();
	int nIndex = GetSheetIndexByName(strSheetName);
	return nIndex;
}

bool PhdExcel::AddSheet(LPCTSTR szSheetName)
{
	try
	{
		_Worksheet sheetTemp;
		sheetTemp.AttachDispatch(m_apSheets->Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing), true);
		sheetTemp.SetName(szSheetName);
		sheetTemp.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::DeleteSheet(int nSheetIndex)
{
	int nSheetCount = GetSheetCount();
	if (nSheetCount <= 1)
		return false;

	try
	{
		//得到当前页的索引
		int nCurIndex = GetCurSheetIndex();
		if (nCurIndex == nSheetIndex)
		{
			m_apSheet->Delete();
			m_apSheet->ReleaseDispatch();
			//m_sheet绑定第一个工作表
			m_apSheet->AttachDispatch(m_apSheets->GetItem(COleVariant((long)(1))), true);
			m_apRange->ReleaseDispatch();
			m_apRange->AttachDispatch(m_apSheet->GetCells(), true);
		}
		else
		{
			_Worksheet sheetTemp;
			sheetTemp.AttachDispatch(m_apSheets->GetItem(COleVariant((long)(nSheetIndex + 1))), true);
			sheetTemp.Delete();
			sheetTemp.ReleaseDispatch();
		}
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::DeleteSheet(LPCTSTR szSheetName)
{
	int nSheetCount = GetSheetCount();
	if (nSheetCount <= 1)
		return false;

	try
	{
		//得到当前页的索引
		int nCurIndex = GetCurSheetIndex();
		int nSheetIndex = GetSheetIndexByName(szSheetName);
		if (nCurIndex == nSheetIndex)
		{
			m_apSheet->Delete();
			m_apSheet->ReleaseDispatch();
			//m_sheet绑定第一个工作表
			m_apSheet->AttachDispatch(m_apSheets->GetItem(COleVariant((long)(1))), true);
			m_apRange->ReleaseDispatch();
			m_apRange->AttachDispatch(m_apSheet->GetCells(), true);
		}
		else
		{
			_Worksheet sheetTemp;
			sheetTemp.AttachDispatch(m_apSheets->GetItem(COleVariant((long)(nSheetIndex + 1))), true);
			sheetTemp.Delete();
			sheetTemp.ReleaseDispatch();
		}
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::SwitchSheet(int nSheetIndex)
{
	try
	{
		m_apSheet->ReleaseDispatch();
		m_apSheet->AttachDispatch(m_apSheets->GetItem(COleVariant((long)(nSheetIndex + 1))), true);
		//
		m_apRange->ReleaseDispatch();
		m_apRange->AttachDispatch(m_apSheet->GetCells(), true);
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::SwitchSheet(LPCTSTR szSheetName)
{
	try
	{
		m_apSheet->ReleaseDispatch();
		m_apSheet->AttachDispatch(m_apSheets->GetItem(_variant_t(szSheetName)), true);
		m_apRange->ReleaseDispatch();
		m_apRange->AttachDispatch(m_apSheet->GetCells(), true);
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::SetCurSheetActivate()
{
	try
	{
		m_apSheet->Activate();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

int PhdExcel::GetUsedRowCount()
{
	int nRowNum = 0;
	try
	{
		_ExRange rangeUsed;
		rangeUsed.AttachDispatch(m_apSheet->GetUsedRange(), true);
		_ExRange rangeRow;
		rangeRow.AttachDispatch(rangeUsed.GetRows());
		nRowNum = rangeRow.GetCount();
		rangeUsed.ReleaseDispatch();
		rangeRow.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return 0;
	}
	return nRowNum;
}

int PhdExcel::GetUsedColCount()
{
	int nColNum = 0;
	try
	{
		_ExRange rangeUsed;
		rangeUsed.AttachDispatch(m_apSheet->GetUsedRange(), true);
		_ExRange rangeCol;
		rangeCol.AttachDispatch(rangeUsed.GetColumns());
		nColNum = rangeCol.GetCount();
		rangeUsed.ReleaseDispatch();
		rangeCol.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return 0;
	}
	return nColNum;
}

bool PhdExcel::GetPrintArea(int& nRow1, int& nCol1, int& nRow2, int& nCol2)
{
	try
	{
		PageSetup pagesetupTemp;
		pagesetupTemp.AttachDispatch(m_apSheet->GetPageSetup());
		CString strPrintArea = pagesetupTemp.GetPrintArea();
		pagesetupTemp.ReleaseDispatch();
		//
		int nIndex = strPrintArea.Find(_T(':'));
		if (nIndex == -1)
			return false;
		CString strLeft = strPrintArea.Left(nIndex);
		CString strRight = strPrintArea.Right(strPrintArea.GetLength() - nIndex - 1);
		//
		nIndex = strLeft.ReverseFind(_T('$'));
		if (nIndex == -1)
			return false;
		CString strRow1 = strLeft.Right(strLeft.GetLength() - nIndex - 1);
		strLeft = strLeft.Left(nIndex);
		nIndex = strLeft.ReverseFind(_T('$'));
		if (nIndex == -1)
			return false;
		CString strCol1 = strLeft.Right(strLeft.GetLength() - nIndex - 1);
		//
		nIndex = strRight.ReverseFind(_T('$'));
		if (nIndex == -1)
			return false;
		CString strRow2 = strRight.Right(strRight.GetLength() - nIndex - 1);
		strRight = strRight.Left(nIndex);
		nIndex = strRight.ReverseFind(_T('$'));
		if (nIndex == -1)
			return false;
		CString strCol2 = strRight.Right(strRight.GetLength() - nIndex - 1);
		//
		nRow1 = _wtoi(strRow1);
		nCol1 = UpperStrToNumber(strCol1);
		nRow2 = _wtoi(strRow2);
		nCol2 = UpperStrToNumber(strCol2);
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

double PhdExcel::GetRowHeight(int nRow, int nCol)
{
	double dHeight = 0;
	try
	{
		CString strCell = GetCellStr(nRow, nCol);
		_ExRange rangeTemp;
		rangeTemp.AttachDispatch(m_apSheet->GetRange(_variant_t(strCell), _variant_t(strCell)));
		dHeight = rangeTemp.GetRowHeight().dblVal;
		rangeTemp.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return 0;
	}
	return dHeight;
}

double PhdExcel::GetColWidth(int nRow, int nCol)
{
	double dWidth = 0;
	try
	{
		CString strCell = GetCellStr(nRow, nCol);
		_ExRange rangeTemp;
		rangeTemp.AttachDispatch(m_apSheet->GetRange(_variant_t(strCell), _variant_t(strCell)));
		dWidth = rangeTemp.GetColumnWidth().dblVal;
		rangeTemp.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return 0;
	}
	return dWidth;
}

bool PhdExcel::SetRowHeight(int nRow, double dHeight)
{
	try
	{
		CString strCell = GetCellStr(nRow, 1);
		_ExRange rangeTemp;
		rangeTemp.AttachDispatch(m_apSheet->GetRange(_variant_t(strCell), _variant_t(strCell)));
		rangeTemp.SetRowHeight((_variant_t)dHeight);
		rangeTemp.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::SetColWidth(int nCol, double dWidth)
{
	try
	{
		CString strCell = GetCellStr(1, nCol);
		_ExRange rangeTemp;
		rangeTemp.AttachDispatch(m_apSheet->GetRange(_variant_t(strCell), _variant_t(strCell)));
		rangeTemp.SetColumnWidth((_variant_t)dWidth);
		rangeTemp.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::SetAllRowHeight(double dHeight)
{
	try
	{
		m_apRange->SetRowHeight((_variant_t)dHeight);
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::SetAllColWidth(double dWidth)
{
	try
	{
		m_apRange->SetColumnWidth((_variant_t)dWidth);
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

CString PhdExcel::GetCellText(int nRow, int nCol)
{
	CString strText;
	SYSTEMTIME st;
	CString stry, strm, strd;

	try
	{
		VARIANT lpDisp = m_apRange->GetItem(_variant_t(nRow), _variant_t(nCol));
		_ExRange rangeTemp;
		rangeTemp.AttachDispatch(lpDisp.pdispVal);
		_variant_t vtVal = rangeTemp.GetValue2();
		vtVal.ChangeType(VT_BSTR, NULL);
		switch (vtVal.vt)
		{
		case VT_BSTR:    //OLE Automation string
		{
			strText = vtVal.bstrVal;
			break;
		}
		case VT_R8: // 8 byte real
		{
			strText.Format(_T("%.f"), vtVal.dblVal);
			break;
		}
		case VT_DATE: //date
		{
			VariantTimeToSystemTime(vtVal.date, &st);
			stry.Format(_T("%d"), st.wYear);
			strm.Format(_T("%d"), st.wMonth);
			strd.Format(_T("%d"), st.wDay);
			strText = stry + _T("-") + strm + _T("-") + strd;
			break;
		}
		case VT_EMPTY: //empty
		{
			strText.Empty();
			break;
		}
		default:
		{
			strText.Empty();
			break;
		}
		}
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return _T("");
	}
	return strText;
}

bool PhdExcel::SetCellText(int nRow, int nCol, LPCTSTR szText)
{
	try
	{
		m_apRange->SetItem(_variant_t(nRow), _variant_t(nCol), _variant_t(szText));
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::ClearContents()
{
	try
	{
		m_apRange->ClearContents();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::SetCellActivate(int nRow, int nCol)
{
	try
	{
		VARIANT lpDisp = m_apRange->GetItem(_variant_t(nRow), _variant_t(nCol));
		_ExRange rangeTemp;
		rangeTemp.AttachDispatch(lpDisp.pdispVal);
		lpDisp = rangeTemp.Select();
		rangeTemp.Activate();
		rangeTemp.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::MergeCell(int nCellRow1, int nCellCol1, int nCellRow2,
	int nCellCol2, bool bCenterAlign)
{
	try
	{
		CString strCell1 = GetCellStr(nCellRow1, nCellCol1);
		CString strCell2 = GetCellStr(nCellRow2, nCellCol2);
		_ExRange rangeTemp;
		rangeTemp.AttachDispatch(m_apSheet->GetRange(_variant_t(strCell1),
			_variant_t(strCell2)));
		rangeTemp.Merge(_variant_t((long)0));
		if (bCenterAlign)
			rangeTemp.SetHorizontalAlignment(_variant_t((long)-4108));
		else
			rangeTemp.SetHorizontalAlignment(COleVariant((short)3));
		rangeTemp.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::SetAllCellCenterAlign()
{
	try
	{
		m_apRange->SetHorizontalAlignment(_variant_t((long)-4108));
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::SetAutoWrapText(int nRow, int nCol, bool bWrapText)
{
	try
	{
		VARIANT lpDisp = m_apRange->GetItem(_variant_t(nRow), _variant_t(nCol));
		_ExRange rangeTemp;
		rangeTemp.AttachDispatch(lpDisp.pdispVal);
		rangeTemp.SetWrapText((_variant_t)(short)bWrapText);
		rangeTemp.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::SetFrame(int nCellRow1, int nCellCol1, int nCellRow2, int nCellCol2, bool bOuterFrame, bool bInnerFrame)
{
	try
	{
		CString strCell1 = GetCellStr(nCellRow1, nCellCol1);
		CString strCell2 = GetCellStr(nCellRow2, nCellCol2);
		_ExRange rangeTemp = m_apSheet->GetRange(_variant_t(strCell1), _variant_t(strCell2));
		Borders borders;
		borders.AttachDispatch(rangeTemp.GetBorders());
		if (bOuterFrame)
		{
			Border bLeft;
			bLeft.AttachDispatch(borders.GetItem(7));
			bLeft.SetLineStyle(COleVariant((long)1));
			bLeft.SetWeight(COleVariant((long)3));

			Border bTop;
			bTop.AttachDispatch(borders.GetItem(8));
			bTop.SetLineStyle(COleVariant((long)1));
			bTop.SetWeight(COleVariant((long)3));

			Border bBottom;
			bBottom.AttachDispatch(borders.GetItem(9));
			bBottom.SetLineStyle(COleVariant((long)1));
			bBottom.SetWeight(COleVariant((long)3));

			Border bRight;
			bRight.AttachDispatch(borders.GetItem(10));
			bRight.SetLineStyle(COleVariant((long)1));
			bRight.SetWeight(COleVariant((long)3));

			bLeft.ReleaseDispatch();
			bTop.ReleaseDispatch();
			bBottom.ReleaseDispatch();
			bRight.ReleaseDispatch();
		}

		if (bInnerFrame)
		{
			Border bVertical;
			bVertical.AttachDispatch(borders.GetItem(11));
			bVertical.SetLineStyle(COleVariant((long)1));
			bVertical.SetWeight(COleVariant((long)2));

			Border bHorizontal;
			bHorizontal.AttachDispatch(borders.GetItem(12));
			bHorizontal.SetLineStyle(COleVariant((long)1));
			bHorizontal.SetWeight(COleVariant((long)2));

			bVertical.ReleaseDispatch();
			bHorizontal.ReleaseDispatch();
		}

		borders.ReleaseDispatch();
		rangeTemp.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::SetFontSize(int nRow, int nCol, int nSize)
{
	try
	{
		VARIANT lpDisp = m_apRange->GetItem(_variant_t(nRow), _variant_t(nCol));
		_ExRange rangeTemp;
		rangeTemp.AttachDispatch(lpDisp.pdispVal);
		_ExFont fontTemp;
		fontTemp.AttachDispatch(rangeTemp.GetFont());
		fontTemp.SetSize(COleVariant((long)nSize));//设置字体大小
		fontTemp.ReleaseDispatch();
		rangeTemp.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::SetBoldFont(int nRow, int nCol, bool bBold)
{
	try
	{
		VARIANT lpDisp = m_apRange->GetItem(_variant_t(nRow), _variant_t(nCol));
		_ExRange rangeTemp;
		rangeTemp.AttachDispatch(lpDisp.pdispVal);
		_ExFont fontTemp;
		fontTemp.AttachDispatch(rangeTemp.GetFont());
		fontTemp.SetBold(COleVariant((long)(bBold ? 1 : 0)));//设置字体加粗
		fontTemp.ReleaseDispatch();
		rangeTemp.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::SetFontType(int nRow, int nCol, LPCTSTR szTextType)
{
	try
	{
		VARIANT lpDisp = m_apRange->GetItem(_variant_t(nRow), _variant_t(nCol));
		_ExRange rangeTemp;
		rangeTemp.AttachDispatch(lpDisp.pdispVal);
		_ExFont fontTemp;
		fontTemp.AttachDispatch(rangeTemp.GetFont());
		fontTemp.SetName(COleVariant(szTextType));
		fontTemp.ReleaseDispatch();
		rangeTemp.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::SetCellTextColor(int nRow, int nCol, int nColorIndex)
{
	try
	{
		CString strCell = GetCellStr(nRow, nCol);
		_ExRange rangeTemp = m_apSheet->GetRange(_variant_t(strCell), _variant_t(strCell));
		_ExFont fontTemp;
		fontTemp.AttachDispatch(rangeTemp.GetFont());
		fontTemp.SetColorIndex(_variant_t(nColorIndex));
		fontTemp.ReleaseDispatch();
		rangeTemp.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::SetCellBackgroundColor(int nRow, int nCol, int nColorIndex)
{
	try
	{
		CString strCell = GetCellStr(nRow, nCol);
		_ExRange rangeTemp = m_apSheet->GetRange(_variant_t(strCell), _variant_t(strCell));
		Interior interTemp;
		interTemp.AttachDispatch(rangeTemp.GetInterior());
		interTemp.SetColorIndex(_variant_t(nColorIndex));
		interTemp.ReleaseDispatch();
		rangeTemp.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::InsertRow(int nRow)
{
	try
	{
		VARIANT lpDisp = m_apRange->GetItem(_variant_t(nRow), _variant_t(1));
		_ExRange copyFrom, copyTo;
		copyTo.AttachDispatch(lpDisp.pdispVal);
		copyFrom.AttachDispatch(copyTo.GetEntireRow());
		copyFrom.Insert(vtMissing);

		copyFrom.ReleaseDispatch();
		copyTo.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::InsertCol(int nCol)
{
	try
	{
		VARIANT lpDisp = m_apRange->GetItem(_variant_t(1), _variant_t(nCol));
		_ExRange copyFrom, copyTo;
		copyTo.AttachDispatch(lpDisp.pdispVal);
		copyFrom.AttachDispatch(copyTo.GetEntireColumn());
		copyFrom.Insert(vtMissing);

		copyFrom.ReleaseDispatch();
		copyTo.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::DeleteRow(int nRow)
{
	try
	{
		VARIANT lpDisp = m_apRange->GetItem(_variant_t(nRow), _variant_t(1));
		_ExRange copyFrom, copyTo;
		copyTo.AttachDispatch(lpDisp.pdispVal);
		copyFrom.AttachDispatch(copyTo.GetEntireRow());
		copyFrom.Delete(vtMissing);

		copyFrom.ReleaseDispatch();
		copyTo.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::DeleteCol(int nCol)
{
	try
	{
		VARIANT lpDisp = m_apRange->GetItem(_variant_t(1), _variant_t(nCol));
		_ExRange copyFrom, copyTo;
		copyTo.AttachDispatch(lpDisp.pdispVal);
		copyFrom.AttachDispatch(copyTo.GetEntireColumn());
		copyFrom.Delete(vtMissing);

		copyFrom.ReleaseDispatch();
		copyTo.ReleaseDispatch();
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

bool PhdExcel::CloseCurOpenExcel(LPCTSTR szExcelPath)
{
	LPDISPATCH lpDisp = NULL;
	CoInitialize(NULL);
	CLSID clsid;
	HRESULT hr;
	hr = ::CLSIDFromProgID(_T("Excel.Application"), &clsid); //通过ProgID取得CLSID
	if (FAILED(hr))
	{
		return FALSE;//电脑上没有excel软件
	}

	IUnknown *pUnknown = NULL;
	IDispatch *pDispatch = NULL;
	hr = ::GetActiveObject(clsid, NULL, &pUnknown); //查找是否有程序在运行
	if (FAILED(hr))
	{//没有excel软件在运行
		return false;
	}
	else
	{//有excel软件在运行
		try
		{
			_ExApplication appTemp;
			hr = pUnknown->QueryInterface(IID_IDispatch, (LPVOID *)&appTemp);
			if (FAILED(hr))
				throw(_T("没有取得IDispatchPtr"));
			pUnknown->Release();
			pUnknown = NULL;

			lpDisp = appTemp.GetWorkbooks();
			Workbooks booksTemp;
			booksTemp.AttachDispatch(lpDisp, TRUE);
			int nLen = booksTemp.GetCount();
			CString strName;
			bool bIsFind = false;
			_Workbook bookTemp;
			for (int i = 1; i <= nLen; i++)
			{
				if (bookTemp)
					bookTemp.ReleaseDispatch();
				lpDisp = booksTemp.GetItem(_variant_t(i));
				bookTemp.AttachDispatch(lpDisp, TRUE);
				strName = bookTemp.GetFullName();

				if (_tcscmp(strName, szExcelPath) == 0)
				{
					bIsFind = true;
					break;
				}
			}
			if (!bIsFind)
			{
				bookTemp.ReleaseDispatch();
				booksTemp.ReleaseDispatch();
				appTemp.ReleaseDispatch();
				CoUninitialize();//关闭当前线程上的com库，卸载该线程加载的所有dll,释放该线程维护的所有其他资源，并强制关闭该线程上的所有RPC连接。
				return false;//没找到
			}

			//保存当前工作簿
			appTemp.SetAlertBeforeOverwriting(FALSE);
			appTemp.SetDisplayAlerts(FALSE);
			bookTemp.Save();
			//关闭指定的工作簿
			COleVariant covFalse((short)FALSE);
			COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
			bookTemp.Close(covFalse, _variant_t(szExcelPath), covOptional);
			bookTemp.ReleaseDispatch();
			//booksTemp.Close();
			booksTemp.ReleaseDispatch();
			//appTemp.Quit();
			appTemp.ReleaseDispatch();
			CoUninitialize();//关闭当前线程上的com库，卸载该线程加载的所有dll,释放该线程维护的所有其他资源，并强制关闭该线程上的所有RPC连接。
			return true;
		}
		catch (...)
		{
			return FALSE;
		}
	}
}

bool PhdExcel::ShowExcel()
{
	try
	{
		m_apApp->SetVisible(true);
		m_apApp->SetWindowState(SW_SHOWNORMAL);
		::BringWindowToTop(HWND(m_apApp->GetHwnd()));//设置窗口顶层显示
	}
	catch (CException* e)
	{
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		return false;
	}
	return true;
}

void PhdExcel::NumberToUpperStr(int nNumber, CString& strNew)
{
	if (nNumber < 1)
		return;
	nNumber--;

	int num2 = nNumber / (25 + 1);
	int num3 = nNumber % (25 + 1);
	TCHAR newStr1 = TCHAR(num3 + 65);//个位字符
	strNew.Insert(0, newStr1);
	if (num2 == 0)
	{
		return;
	}
	else if (0 < num2 && num2 <= 25)
	{
		newStr1 = TCHAR(num2 + 64);//前位字符
		strNew.Insert(0, newStr1);
	}
	else if (num2 > 25)
	{
		NumberToUpperStr(num2, strNew);
	}
}

int PhdExcel::UpperStrToNumber(LPCTSTR szStr)
{
	int nNumber = 0;
	CString str = szStr;
	int nCount = str.GetLength();
	int j = nCount;
	for (int i = 0; i < nCount; i++)
	{
		TCHAR curStr = str[j - 1];
		if (curStr < _T('A') && curStr > _T('Z'))
			return 0;
		if (i == 0)
		{
			int nNumTemp = curStr;
			nNumber = nNumTemp - 64;
		}
		else
		{
			//求指数
			int nPow = std::pow(26.0, i);
			//
			int nNumTemp = curStr;
			int nNumTemp2 = (nNumTemp - 64) * nPow;

			nNumber += nNumTemp2;
		}
		j--;
	}

	return nNumber;
}

CString PhdExcel::GetCellStr(int nRow, int nCol)
{
	CString strCol;
	NumberToUpperStr(nCol, strCol);
	CString strCell;
	strCell.Format(_T("%s%d"), strCol, nRow);
	return strCell;
}

bool PhdExcel::IsNumber(LPCTSTR sz, bool bAcceptDouble)
{
	TCHAR cCurrent = 0;
	int   iDot = 0;
	CString sValue = sz;

	// check
	if (sValue.IsEmpty())
		return false;

	// Check each character of the string
	for (int i = 0; i < sValue.GetLength(); i++)
	{
		cCurrent = sValue.GetAt(i);

		// The minus sign may only be at the begin
		if (cCurrent == _T('-') && i == 0)
			continue;

		// A dot may not occure of we do not accept doubles
		if (cCurrent == _T('.') && !bAcceptDouble)
			return false;

		// A dot may only occure once
		if (cCurrent == _T('.'))
		{
			iDot++;
			if (iDot == 1)
				continue;
			else
				return false;
		}

		// A number is something between 0 and 9, dah!
		if (cCurrent < _T('0') || cCurrent > _T('9'))
			return false;
	}

	// We passed our wonderfull check,
	// so the string is an int or double Yiihaa
	return true;
}

}