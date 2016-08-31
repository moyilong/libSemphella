#include <libSemphella/string.h>
#include <libSemphella/debug.h>
#include <libSemphella/crypt.h>
#include <libSkovia/libSkovia.h>
#include <libSemphella/argment.h>
#include <libSemphella/utils.h>

string host="localhost";
int port=7783;
bool server = false;

void proc(string name, string val)
{
	switch (name.at(0))
	{
	case 'h':
		host = val;
		break;
	case 's':
		server = true;
		break;
	case 'p':
		port = atoi(val.data());
		break;
	}
}
#define TRANST_LEN	(2*MB)
char *buff = (char*)malloc(TRANST_LEN);
void Caller(ESocket sock)
{
		int mode = sock.XRecv();
		if (mode == 0)
		{
			uint64_t len = 0;
			char *ptr = sock.XRecv(len);
			free(ptr);
		}
		else {
			time_t now = time(0);
			sock.XSend(buff, TRANST_LEN);
			time_t end = time(0);
			sock.XSend(end - now);
		}
}
uint64_t upload_count = 0,download_count=0;
uint64_t TestUpload()
{
	ESocket sock = SKOVIA_Client::Client(host, port);
	time_t now = time(0);
	sock.XSend(buff, TRANST_LEN);
	time_t end = time(0);
	uint64_t ano = end - now;
	upload_count += TRANST_LEN;
	sock.Close();
	return ano;
}

uint64_t TestDonalod()
{
	ESocket sock = SKOVIA_Client::Client(host, port);
	sock.XSend(1);
	uint64_t len = 0;
	char *ptr = sock.XRecv(len);
	uint64_t time_out = sock.XRecv();
	download_count += len;
	free(ptr);
	sock.Close();
	return time_out;
}


int main(int argc, char *argv[])
{
	KERNEL.SetDebugStat(false);
	argment args;
	debug << "Init Skovia..." << endl;
	args.load(argc, argv);
	args.for_each(proc);
	if (server)
	{
		SKOVIA_Server::Server(port, Caller);
	}
	else {
		uint64_t time_out = 0;
		uint64_t ano = 0;
		uint64_t count = 1024;
		cout << "Begin Test:" << count << endl;
		ShowProcessBar(1,"");
		esleep(1000);
		for (uint64_t n = 0; n < count ; n++)
		{
			ano += TestUpload();
			time_out += TestDonalod();
			debug << "Loop End!" << endl;
			if ((long long)n % 200 == 0)
			{
				double val = (double)n / (double)count;
				ShowProcessBar(val, "->");
			}
		}
		cout << endl;
		cout << " Upload:\t" << upload_count << "\t" << ano << endl;
		cout << " Download:\t" << download_count << "\t" << time_out << endl;
		cout << "Test End!" << endl;
		cout << "Spped: Up:" << human_read_storage_str( upload_count / dZero(ano)) << "/s Do:" <<human_read_storage_str( download_count / dZero(time_out)) <<"/s"<< endl;
	}
}