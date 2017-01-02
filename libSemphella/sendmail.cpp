#include "net.h"
//author: Zero

//facade of function send()
__inline void _Send(SOCKET& s, string data) {
	if (send(s, data.c_str(), data.length(), 0) == SOCKET_ERROR) {
		debug << "send data \"" << data << "\" error" << endl;
	}
	else {
		debug << "send data \"" << data << "\" success!" << endl;
	}
}

//facade of function recv()
__inline void _Recv(SOCKET& s, char* buf, int len) {
	memset(buf, 0, len);
	if (recv(s, buf, len, 0) == SOCKET_ERROR) {
		debug << "error, while receiving data" << endl;
	}
	else
		debug << "success recv data:" << buf << endl;
}

#define Send _Send
#define Recv _Recv

API bool SendEmail(const string& smtpServer, const string& username, const string& pw, const string& to, const string& data) {
	hostent *ph = gethostbyname(smtpServer.data());
	if (ph == NULL) {
		debug << "no host: " << smtpServer << endl;
		return false;
	}

	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(25);    //port of SMTP 
#ifndef __linux__
	memcpy(&sin.sin_addr.S_un.S_addr, 
#else
	memcpy(&sin.sin_addr.s_un.s_addr, 
#endif
		ph->h_addr_list[0], ph->h_length);
	debug << "Prepare to connect..." << endl;
	//connect to the mail server
	SOCKET s = socket(PF_INET, SOCK_STREAM, 0);
	if (connect(s, (sockaddr*)&sin, sizeof(sin))) {
		debug << "failed to connect the mail server" << endl;
		return false;
	}

	//
	debug << "Waitting for greet message" << endl;
	char recvBuffer[1024];
	Recv(s, recvBuffer, sizeof(recvBuffer));    //wait for greeting message
	cout << "Remote Service is Connect!" << endl << recvBuffer << endl;
	Send(s, "HELO " + smtpServer + "\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));    //should recv "250 OK"
	debug << "start to log in    " << endl;
	Send(s, "auth login\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));    //should recv "334 username:"(This is the decode message)

	Send(s, Base64Encode(username) + "\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));
	if (string(recvBuffer).substr(0, 3) != "334") {
		cout << "username is error!" << endl;
		return false;
	}

	Send(s, Base64Encode(pw) + "\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));
	if (string(recvBuffer).substr(0, 3) != "235") {
		cout << "password error" << endl;
		return false;
	}

	//Set sender
	Send(s, "mail from:<" + username + ">\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));    //should recv "250 Mail OK"

												//set receiver
	Send(s, "rcpt to:<" + to + ">\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));    //should recv "250 Mail OK"

												//send data
	Send(s, "data\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));    //should recv "354 End data with <CR><LF>.<CR><LF>"
	Send(s, "to:" + to + "\r\n" + "subject:the newest IP\r\n\r\n" + data + "\r\n.\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));

	Send(s, "quit\r\n");
	Recv(s, recvBuffer, sizeof(recvBuffer));
	closesocket(s);
	return true;
}