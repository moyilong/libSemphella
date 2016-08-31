/*
* Source file about portable socket class.
*
* design:odison
* e-mail:odison@126.com>
*
*/



#include "libSkovia.h"


ESocket::ESocket(SOCKET sock)
{
	m_sock = sock;
}

ESocket::~ESocket()
{
}

int ESocket::Init()
{
#ifdef WIN32
	/*
	http://msdn.microsoft.com/zh-cn/vstudio/ms741563(en-us,VS.85).aspx

	typedef struct WSAData {
	WORD wVersion;								//winsock version
	WORD wHighVersion;							//The highest version of the Windows Sockets specification that the Ws2_32.dll can support
	char szDescription[WSADESCRIPTION_LEN+1];
	char szSystemStatus[WSASYSSTATUS_LEN+1];
	unsigned short iMaxSockets;
	unsigned short iMaxUdpDg;
	char FAR * lpVendorInfo;
	}WSADATA, *LPWSADATA;
	*/
	WSADATA wsaData;
	//#define MAKEWORD(a,b) ((WORD) (((BYTE) (a)) | ((WORD) ((BYTE) (b))) << 8)) 
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);//win sock start up
	if (ret) {
		cerr << "Initilize winsock error !" << endl;
		return -1;
	}
#endif

	return 0;
}
//this is just for windows
int ESocket::Clean()
{
#ifdef WIN32
	return (WSACleanup());
#endif
	return 0;
}

ESocket& ESocket::operator = (SOCKET s)
{
	m_sock = s;
	return (*this);
}

bool ESocket::operator==(const SOCKET s)
{
	if (s == m_sock)
		return true;
	return false;
}

ESocket::operator SOCKET ()
{
	return m_sock;
}
//create a socket object win/lin is the same
// af:
bool ESocket::Create(int af, int type, int protocol)
{
	m_sock = socket(af, type, protocol);
	if (m_sock == INVALID_SOCKET) {
		return false;
	}
	return true;
}

bool ESocket::Connect(const char* ip, unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ip);
	svraddr.sin_port = htons(port);
	int ret = connect(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if (ret == SOCKET_ERROR) {
		return false;
	}
	return true;
}

bool ESocket::Bind(unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = INADDR_ANY;
	svraddr.sin_port = htons(port);

	int opt = 1;
	if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0)
		return false;

	int ret = bind(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if (ret == SOCKET_ERROR) {
		return false;
	}
	return true;
}
//for server
bool ESocket::Listen(int backlog)
{
	int ret = listen(m_sock, backlog);
	if (ret == SOCKET_ERROR) {
		return false;
	}
	return true;
}

bool ESocket::Accept(ESocket& s, char* fromip)
{
	struct sockaddr_in cliaddr;
	int addrlen = sizeof(cliaddr);
	SOCKET sock = accept(m_sock, (struct sockaddr*)&cliaddr, &addrlen);
	if (sock == SOCKET_ERROR) {
		return false;
	}

	s = sock;
	if (fromip != NULL)
		sprintf(fromip, "%s", inet_ntoa(cliaddr.sin_addr));

	return true;
}

int ESocket::Send(const char* buf, int len, int flags)
{
	/*int bytes;
	int count = 0;

	while (count < len) {

		bytes = send(m_sock, buf + count, len - count, flags);
		if (bytes == -1 || bytes == 0)
			return -1;
		count += bytes;
	}
	*/
	return send(m_sock,buf,len,flags);
}

int ESocket::Recv(char* buf, int len, int flags)
{
	return recv(m_sock, buf, len, flags);
}

void ESocket::XSend(const char * buff, uint64_t len)
{
	debug << "Send Leng:" << len << endl;
	Send((char*)&len, sizeof(uint64_t));
	Send(buff, len);
}

char* ESocket::XRecv(uint64_t & len)
{
	uint64_t get;
	char buff[sizeof(uint64_t)];
	Recv(buff, sizeof(uint64_t));
	memcpy(&get, buff, sizeof(uint64_t));
	char *ptr = (char*)malloc(get);
	len = get;
	debug << "Recv Len:" << len << endl;
	Recv(ptr, len);
	return ptr;
}
void ESocket::XSend(uint64_t data)
{
	Send((char*)&data, sizeof(uint64_t));
}
uint64_t ESocket::XRecv()
{
	uint64_t get;
	char buff[sizeof(uint64_t)];
	Recv(buff, sizeof(uint64_t));
	memcpy(&get, buff,sizeof(uint64_t));
	free(buff);
	return get;
}

int ESocket::Close()
{
#ifdef WIN32
	return (closesocket(m_sock));
#else
	return (close(m_sock));
#endif
}

int ESocket::GetError()
{
#ifdef WIN32
	return (WSAGetLastError());
#else
	return (errno);
#endif
}

bool ESocket::DnsParse(const char* domain, char* ip)
{
	struct hostent* p;
	if ((p = gethostbyname(domain)) == NULL)
		return false;

	sprintf(ip,
		"%u.%u.%u.%u",
		(unsigned char)p->h_addr_list[0][0],
		(unsigned char)p->h_addr_list[0][1],
		(unsigned char)p->h_addr_list[0][2],
		(unsigned char)p->h_addr_list[0][3]);

	return true;
}
