#include "pch.h"
#include "PhdSocket.h"
#include <ws2tcpip.h>
#include <clocale>
#include <cstdlib>

#ifdef _WIN32
#pragma comment (lib,"ws2_32.lib")
#endif

PhdSocket::PhdSocket()
{
#ifdef _WIN32
	WSAData wd;
	WSAStartup(0x0202, &wd);
#endif
	m_hSocket = INVALID_SOCKET;
}

PhdSocket::~PhdSocket()
{
	this->Close();
}

void PhdSocket::Close()
{
	closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;
}

bool PhdSocket::IsNull() const
{
	if (m_hSocket == INVALID_SOCKET)
		return true;
	else
		return false;
}

BOOL PhdSocket::Create(UINT nSocketPort /*= 0*/, int nSocketType /*= SOCK_STREAM */, LPCTSTR lpszSocketAddress /*= NULL*/)
{
	//����socket
	m_hSocket = socket(AF_INET, nSocketType, 0);
	if (m_hSocket == INVALID_SOCKET)
	{
		return FALSE;
	}
	//������ַ��Ϣ�ṹ�壺IP�Ͷ˿�
	sockaddr_in sa = { AF_INET,htons(nSocketPort) };
	if (lpszSocketAddress)
	{
		//IP��ַ���뵽��ַ�ṹ���У�InetPton���ڿ��ֽڣ�inet_pton���ڶ��ֽڣ�
		Pton(lpszSocketAddress, sa);
	}
	//socket�󶨵�ַ�ṹ��
	return !bind(m_hSocket, (sockaddr*)&sa, sizeof(sa));
}

BOOL PhdSocket::Listen(int nConnectionBacklog /*= 5*/) const
{
	return !listen(m_hSocket, nConnectionBacklog);
}

BOOL PhdSocket::Accept(PhdSocket& rConnectedSocket, std::string& strIP, UINT *nPort) const
{
	//���������ַ�ṹ��
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	rConnectedSocket.m_hSocket = accept(m_hSocket, (sockaddr*)&sa, &nLen);
	if (INVALID_SOCKET == rConnectedSocket.m_hSocket)
		return FALSE;

	//�õ�ip��ַ
	Ntop(sa, strIP);

	//�õ��˿ں�
	*nPort = htons(sa.sin_port);

	return TRUE;
}

BOOL PhdSocket::Accept(PhdSocket& rConnectedSocket, std::wstring& strIP, UINT *nPort) const
{
	//���������ַ�ṹ��
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	rConnectedSocket.m_hSocket = accept(m_hSocket, (sockaddr*)&sa, &nLen);
	if (INVALID_SOCKET == rConnectedSocket.m_hSocket)
		return FALSE;

	//�õ�ip��ַ
	Ntop(sa, strIP);

	//�õ��˿ں�
	*nPort = htons(sa.sin_port);

	return TRUE;
}

BOOL PhdSocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort) const
{
	sockaddr_in sa = { AF_INET,htons(nHostPort) };
	Pton(lpszHostAddress, sa);
	return !connect(m_hSocket, (sockaddr*)&sa, sizeof(sa));
}

int PhdSocket::Send(const void* lpBuf, int nBufLen, int nFlags /*= 0*/) const
{
	return send(m_hSocket, (const char*)lpBuf, nBufLen, nFlags);
}

int PhdSocket::SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress /*= NULL*/) const
{
	sockaddr_in sa = { AF_INET,htons(nHostPort) };
	Pton(lpszHostAddress, sa);
	sendto(m_hSocket, (const char*)lpBuf, nBufLen, 0, (sockaddr*)&sa, sizeof(sa));
	return 0;
}

int PhdSocket::Receive(void* lpBuf, int nBufLen, int nFlags /*= 0*/) const
{
	return recv(m_hSocket, (char*)lpBuf, nBufLen, nFlags);
}

int PhdSocket::ReceiveFrom(void* lpBuf, int nBufLen, std::string& strIP, UINT& rSocketPort) const
{
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	int nRet = recvfrom(m_hSocket, (char*)lpBuf, nBufLen, 0, (sockaddr*)&sa, &nLen);
	if (nRet <= 0)
		return nRet;

	rSocketPort = htons(sa.sin_port);
	Ntop(sa, strIP);

	return nRet;
}

int PhdSocket::ReceiveFrom(void* lpBuf, int nBufLen, std::wstring& strIP, UINT& rSocketPort) const
{
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	int nRet = recvfrom(m_hSocket, (char*)lpBuf, nBufLen, 0, (sockaddr*)&sa, &nLen);
	if (nRet <= 0)
		return nRet;

	rSocketPort = htons(sa.sin_port);
	Ntop(sa, strIP);

	return nRet;
}

BOOL PhdSocket::GetPeerName(std::string& strIP, UINT& rSocketPort) const
{
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	if (getpeername(m_hSocket, (sockaddr*)&sa, &nLen) < 0)
		return FALSE;
	rSocketPort = htons(sa.sin_port);
	Ntop(sa, strIP);
	return TRUE;
}

BOOL PhdSocket::GetPeerName(std::wstring& strIP, UINT& rSocketPort) const
{
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	if (getpeername(m_hSocket, (sockaddr*)&sa, &nLen) < 0)
		return FALSE;
	rSocketPort = htons(sa.sin_port);
	Ntop(sa, strIP);
	return TRUE;
}

BOOL PhdSocket::GetSockName(std::string& strIP, UINT& rSocketPort) const
{
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	if (getsockname(m_hSocket, (sockaddr*)&sa, &nLen) < 0)
		return FALSE;
	rSocketPort = htons(sa.sin_port);
	Ntop(sa, strIP);
	return TRUE;
}

BOOL PhdSocket::GetSockName(std::wstring& strIP, UINT& rSocketPort) const
{
	sockaddr_in sa = { AF_INET };
	socklen_t nLen = sizeof(sa);
	if (getsockname(m_hSocket, (sockaddr*)&sa, &nLen) < 0)
		return FALSE;
	rSocketPort = htons(sa.sin_port);
	Ntop(sa, strIP);
	return TRUE;
}

void PhdSocket::Ntop(const sockaddr_in& sa, std::wstring& strIP) const
{
#ifdef _UNICODE
	wchar_t wcIP[INET_ADDRSTRLEN];
	InetNtop(AF_INET, (void *)&sa.sin_addr, wcIP, sizeof(wcIP));
	strIP.assign(wcIP);
#else
	char cIP[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, (void *)&sa.sin_addr, cIP, sizeof(cIP));
	wchar_t* wcIP = CToW(cIP);
	strIP.assign(wcIP);
	delete[] wcIP;
#endif	
}

void PhdSocket::Ntop(const sockaddr_in& sa, std::string& strIP) const
{
#ifdef _UNICODE
	wchar_t wcIP[INET_ADDRSTRLEN];
	InetNtop(AF_INET, (void *)&sa.sin_addr, wcIP, sizeof(wcIP));
	char* cIP = WToC(wcIP);
	strIP.assign(cIP);
	delete[] cIP;
#else
	char cIP[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, (void *)&sa.sin_addr, cIP, sizeof(cIP));
	strIP.assign(cIP);
#endif
}

wchar_t* PhdSocket::CToW(char* str) const
{
	//����char *�����С�����ֽ�Ϊ��λ��һ������ռ�����ֽ�
	int charLen = std::strlen(str);

	//������ֽ��ַ��Ĵ�С�����ַ����㡣
	int len = MultiByteToWideChar(CP_ACP, 0, str, charLen, NULL, 0);

	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ķ��ֽ��ַ���С
	wchar_t* buf = new wchar_t[len + 1];

	//���ֽڱ���ת���ɿ��ֽڱ���
	MultiByteToWideChar(CP_ACP, 0, str, charLen, buf, len);

	buf[len] = '\0'; //����ַ�����β��ע�ⲻ��len+1

	return buf;
}

std::wstring PhdSocket::CToW(const std::string& str) const
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

char* PhdSocket::WToC(wchar_t* str) const
{
	//����wchar_t *�����С�����ֽ�Ϊ��λ��һ������ռ�����ֽ�
	int wcharLen = std::wcslen(str);

	//��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����
	int len = WideCharToMultiByte(CP_ACP, 0, str, wcharLen, NULL, 0, NULL, NULL);

	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ŀ��ֽ��ֽڴ�С
	char* buf = new char[len + 1]; //���ֽ�Ϊ��λ

	//���ֽڱ���ת���ɶ��ֽڱ���
	WideCharToMultiByte(CP_ACP, 0, str, wcharLen, buf, len, NULL, NULL);

	buf[len] = '\0'; //���ֽ��ַ���'\0'����

	return buf;
}

std::string PhdSocket::WToC(const std::wstring& str) const
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

