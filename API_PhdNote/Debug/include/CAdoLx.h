// AdoLx.h: Version 1.1��֧��Unicode��֧��VS2015��VC6���뻷����
//֧��Unicode��֧��Access��SQLServer��Oracle�������ݿ�
//Author:������ʦ��װ�� 
//������ʦ΢����http://t.qq.com/richmain ������ʦ΢�ţ�richmain
//������õ�����Ŀ������Ƶ�̳̣����¼http://www.baojy.com
//////////////////////////////////////////////////////////////////////
#pragma once
#pragma warning (disable:4146)
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF","adoEOF")rename("BOF","adoBOF")rename ("EOS","adoEOS")
//#import "E:\A�ҵıʼ�\PhdNotes\Sql\msado15.dll" no_namespace rename ("EOF","EndOfFile") rename ("EOS","EndOfStream")


class PHD_NOTE_API CAdoLx
{
	_RecordsetPtr m_pRst;		//��¼��������ָ��
	_ConnectionPtr m_pConn;		//���Ӷ�������ָ��

	CString m_szLastError;
public:
	CAdoLx();
	virtual ~CAdoLx();

	void Release()
	{
		m_pConn->Release();
	}
	void Close()
	{
		m_pConn->Close();
	}

	BOOL GetFieldType(long nIndex, int &nType);

	//�������ݿ������
	enum EType { DBT_ACCESS, DBT_SQL, DBT_ORACLE };

	
	// Summary:   �������ݿ�
	// Time:	  2020��3��21�� peihaodong
	// Explain:	  ���ݿ����ͣ����ݿ��������룻�û�����ip��ַ���˿ںţ�1433��
	BOOL Connect(EType eType, LPCTSTR szDatabase, LPCTSTR szPass = NULL, LPCTSTR szUser = NULL, LPCTSTR szHost = NULL,LPCTSTR szPort = NULL);

	//��ɾ�Ĳ�������
	int ExecSQL(LPCTSTR szSQL);

	//��ѯ����
	BOOL Select(LPCTSTR szSQL);

	//��������
	BOOL IsEOF();
	BOOL MoveNext();
	BOOL MovePrev();

	BOOL MoveFirst();
	BOOL MoveLast();

	//�õ��ֶ�����
	int GetFieldCount();
	//ͨ���±�õ��ֶ���
	BOOL GetFieldName(long nIndex, CString &szName);
	//�õ���¼����
	int GetRecordCount();
	//�õ�������Ϣ
	CString GetLastError()
	{
		CString str = m_szLastError;
		m_szLastError.Empty();
		return str;
	}

	//�����ֶ�����ȡ��������
	BOOL GetFieldValue(LPCTSTR szField, COleDateTime &tValue);
	BOOL GetFieldValue(LPCTSTR szField, double &fValue);
	BOOL GetFieldValue(LPCTSTR szField, CString &sValue);
	BOOL GetFieldValue(LPCTSTR szField, int &iValue);

	//����������ȡ��������
	BOOL GetFieldByIndex(long nIndex, COleDateTime &tValue);
	BOOL GetFieldByIndex(long nIndex, double &fValue);
	BOOL GetFieldByIndex(long nIndex, CString &sValue);
	BOOL GetFieldByIndex(long nIndex, int &iValue);
	BOOL GetFieldByIndex(long nIndex, long &lValue);
	BOOL GetFieldByIndex(long nIndex, DWORD &dwValue);
	BOOL GetFieldByIndex(long nIndex, UINT &iValue);
	BOOL GetFieldByIndex(long nIndex, char &cValue);
	BOOL GetFieldByIndex(long nIndex, short &nValue);
	BOOL GetFieldByIndex(long nIndex, BYTE &cbValue);
	BOOL GetFieldByIndex(long nIndex, WORD &wValue);
	BOOL GetFieldByIndex(long nIndex, TCHAR sValue[], int nSize);

	class CProc
	{
		/*
		enum ParameterDirectionEnum
		{
		adParamUnknown = 0,
		adParamInput = 1,
		adParamOutput = 2,
		adParamInputOutput = 3,
		adParamReturnValue = 4
		};
		*/
		CString m_szLastError;
		_CommandPtr m_pCmd;
		//_ConnectionPtr & m_pConn;
	public:
		LPCTSTR GetLastError() const
		{
			return m_szLastError;
		}
		int Exec();
		BOOL Create(_ConnectionPtr &pConn, LPCTSTR szProc);

		BOOL AddParam(LPCTSTR szVName, long lParam, ParameterDirectionEnum eDir = adParamInput);
		BOOL AddParam(LPCTSTR szVName, LPCTSTR szParam, ParameterDirectionEnum eDir = adParamInput);

		BOOL GetValue(LPCTSTR szVName, long &lParam);
		BOOL GetValue(LPCTSTR szVName, CString &lParam);

		void Release()
		{
			delete this;
		}
		CProc()
		{
		}

	};
	CProc* CreateProc(LPCTSTR szProc);
	int ExecProc(CProc* pProc);

};

