#pragma once

/***********************************************
   >   Class Name: PhdSocket
   >     Describe: ��socket�ķ�װ��֧�ֿ���̨�Ͷ��߳�
   >       Author: peihaodong
   > Created Time: 2020��9��30��
   >         Blog: https://blog.csdn.net/phd17621680432
   >           QQ: 841382590
**********************************************/

/*
1��   WIN32/_WIN32 ���������ж��Ƿ� Windows ϵͳ�����ڿ�ƽ̨����
2�� _WIN64 �����жϱ��뻷���� x86��32λ�� ���� x64��64λ��
		�� Win32 �����£�_WIN32 �ж��壬_WIN64 û�ж��塣
		 �� x64 �����£�_WIN32 ��_WIN64 ���ж��塣
*/

#include <stdlib.h>
#include <cwchar>
#include <cstring>
#include <ws2tcpip.h>

#ifdef _WIN32
#include <WinSock2.h>
#include <process.h>
#ifdef _UNICODE
#include <comdef.h>
//#include <WS2tcpip.h>
#endif
typedef int socklen_t;
typedef void RET_TYPE;
#else
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
typedef unsigned int SOCKET;
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#ifndef _T
#define _T(x) x
#endif
typedef struct in_addr IN_ADDR;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL            *PBOOL;
typedef BOOL             *LPBOOL;
typedef BYTE            *PBYTE;
typedef BYTE             *LPBYTE;
typedef int             *PINT;
typedef int              *LPINT;
typedef WORD            *PWORD;
typedef WORD             *LPWORD;
typedef long             *LPLONG;
typedef DWORD           *PDWORD;
typedef DWORD            *LPDWORD;
typedef void             *LPVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;
typedef const char* LPCTSTR, *LPCSTR;
typedef char* LPTSTR, *LPSTR;
typedef void* RET_TYPE;
inline int GetLastError()
{
	return errno;
}
#define closesocket(x) close(x)


#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif
#endif
#include <string>

class PHD_NOTE_API PhdSocket
{
public:
	PhdSocket();
	virtual ~PhdSocket();

	operator SOCKET() const{
		return m_hSocket;
	}

public:
	//�ر�socket
	void Close();

	// Summary:   �ö����Ƿ�Ϊ��
	// Time:	  2020��3��31�� peihaodong
	// Explain:	  
	bool IsNull() const;

	//����socket����
	//nSocketPort - �˿ڣ������0��������˿�
	//lpszSocketAddress - IP��ַ
	BOOL Create(UINT nSocketPort = 0, int nSocketType = SOCK_STREAM
		, LPCTSTR lpszSocketAddress = NULL);

	// Summary:   ����
	// Time:	  2020��3��30�� peihaodong
	// Explain:	  nConnectionBacklog - ����ͬʱ������5���ͻ��ˣ�����5����Ҫ�ȴ�
	BOOL Listen(int nConnectionBacklog = 5) const;

	// Summary:   ���գ������������пͻ������ӻ������µ�PhdSocket����
	// Time:	  2020��3��30�� peihaodong
	// Explain:	  ����PhdSocket�����IP�Ͷ˿�
	BOOL Accept(PhdSocket& rConnectedSocket,std::string& strIP,
		UINT *nPort) const;
	BOOL Accept(PhdSocket& rConnectedSocket, std::wstring& strIP,
		UINT *nPort) const;

	// Summary:   ���ӷ�������ͨ��������ǰ̨IP�Ͷ˿ڣ�
	// Time:	  2020��3��30�� peihaodong
	// Explain:	  
	BOOL Connect(LPCTSTR lpszHostAddress, UINT nHostPort) const;

	// Summary:   �������ݣ������ӵ�socket
	// Time:	  2020��3��30�� peihaodong
	// Explain:	  ���ط��͵��ַ�����
	int Send(const void* lpBuf, int nBufLen, int nFlags = 0) const;
	// Summary:   �������� ��õ�ַ�ṹ�� 
	// Time:	  2020��3��30�� peihaodong
	// Explain:	  ���ط��͵��ַ�����
	int SendTo(const void* lpBuf, int nBufLen, UINT nHostPort,
		LPCTSTR lpszHostAddress = NULL) const;	

	// Summary:   �������ݣ������ӵ�socket
	// Time:	  2020��3��30�� peihaodong
	// Explain:	  ���ؽ��յ��ַ�����
	int Receive(void* lpBuf, int nBufLen, int nFlags = 0) const;
	// Summary:   �������� �õ����Ͷ˵�IP�Ͷ˿�
	// Time:	  2020��3��30�� peihaodong
	// Explain:	  ���ؽ��յ��ַ�����
	int ReceiveFrom(void* lpBuf, int nBufLen,
		std::string& strIP, UINT& rSocketPort) const;
	int ReceiveFrom(void* lpBuf, int nBufLen,
		std::wstring& strIP, UINT& rSocketPort) const;

	// Summary:   �õ�socket��Ϣ
	// Time:	  2020��3��30�� peihaodong
	// Explain:	  
	BOOL GetPeerName(std::string& strIP, UINT& rSocketPort) const;
	BOOL GetPeerName(std::wstring& strIP, UINT& rSocketPort) const;
	BOOL GetSockName(std::string& strIP, UINT& rSocketPort) const;
	BOOL GetSockName(std::wstring& strIP, UINT& rSocketPort) const;

private:
	//ͨ����ַ�ṹ��õ�IP��ַ
	void Ntop(const sockaddr_in& sa, std::wstring& strIP) const;
	void Ntop(const sockaddr_in& sa, std::string& strIP) const;

#ifdef _UNICODE
	//IP��ַ���뵽��ַ�ṹ����
	inline void Pton(LPCTSTR sHostAddr, sockaddr_in& sa) const {
		InetPton(AF_INET, sHostAddr, &sa.sin_addr.s_addr);
	}
#else
	//IP��ַ���뵽��ַ�ṹ����
	inline void Pton(LPCTSTR sHostAddr, sockaddr_in& sa) const {
		inet_pton(AF_INET, sHostAddr, &sa.sin_addr.s_addr);
	}
#endif

	//************************************
	// Summary:  ���ֽ��ַ���ת���ֽ��ַ���
	// Parameter:
	//	  str -	
	// Explain:	 ��Ҫ�ֶ�delete
	// Time:	  2020��9��4�� peihaodong
	//************************************
	wchar_t* CToW(char* str) const;
	std::wstring CToW(const std::string& str) const;
	//************************************
	// Summary:  ���ֽ��ַ���ת���ֽ��ַ���
	// Parameter:
	//	  str -	
	// Explain:	 ��Ҫ�ֶ�delete
	// Time:	  2020��9��4�� peihaodong
	//************************************
	char* WToC(wchar_t* str) const;
	std::string WToC(const std::wstring& str) const;
	

private:
	SOCKET m_hSocket;//���ĳ�Ա����
};