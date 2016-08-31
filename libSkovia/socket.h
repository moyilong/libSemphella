#pragma once
/*
* define file about portable socket class.
* description:this sock is suit both windows and linux
* design:odison
* e-mail:odison@126.com>
*
*/

class SAPI ESocket {
public:
	ESocket(SOCKET sock = INVALID_SOCKET);
	~ESocket();
	bool Create(int af, int type, int protocol = 0);
	bool Connect(const char* ip, unsigned short port);
	bool Bind(unsigned short port);
	bool Listen(int backlog = 5);
	bool Accept(ESocket& s, char* fromip = NULL);
	int Send(const char* buf, int len, int flags = 0);
	int Recv(char* buf, int len, int flags = 0);
	void XSend(const char *buff, uint64_t len);
	char* XRecv(uint64_t &len);
	void XSend(uint64_t data);
	uint64_t  XRecv();
	int Close();
	int GetError();
	static int Init();
	static int Clean();
	static bool DnsParse(const char* domain, char* ip);
	ESocket& operator = (SOCKET s);
	bool operator ==(const SOCKET s);

	operator SOCKET ();

protected:
	SOCKET m_sock;

};
