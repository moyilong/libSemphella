// mpshell.cpp : 定义控制台应用程序的入口点。
//

#include <libSemphella/libSemphella.h>
#include <libSemphella/debug.h>
#include <libSemphella/string.h>
#include <omp.h>

libDebug mpshell("MPShell");

vector<string>poll;

//USEAGE:	mpshell <args> -c <shell>
//REPLACE:	##OMP	OMP循环条目
//			##MP_TN		OMP线程编号
//			##MP_ID	OMP随机编号
bool testmode = false;
inline void exec(string command)
{
	DEBUG_LINE mpshell << "Exec:" << command << endl;
	if (!testmode)
	system(command.data());
}

int main(int argc,char* argv[])
{
	mpshell << "Init MPShell" << endl << "Init Thread:" << omp_get_max_threads()<<endl;
	int cmdstart = -1;
	ifstream input;
	int _s_start;
	int _s_stop;
	int _s_step;
	char buff[MAX_BUFF_SIZE] = { 0x00 };
	string sbuff;
	mpshell << "Init Option" << endl;
	for (int n = 1; n < argc; n++)
		if (argv[n][0] == '-')
			switch (argv[n][1])
		{
			case 'n':
				n++;
				omp_set_num_threads(atoi(argv[n]));
				break;
			case 'c':
				cmdstart = n;
				break;
			case 'T':
				testmode = true;
				break;
			case 'f':
				mpshell << "init poll with file" << endl;
				n++;
				input.open(argv[n]);
				if (!input.is_open())
				{
					mpshell << "Open File Faild!" << endl;
					return -1;
				}
				while (!input.eof())
				{
					sZero(buff, 0, sizeof(buff));
					string buff;
					getline(input, buff);
					poll.push_back(buff);
				}
				break;
			case 'l':
				n++;
				 _s_start = atoi(argv[n]);
				n++;
				 _s_step = atoi(argv[n]);
				n++;
				 _s_stop = atoi(argv[n])+1;
				for (int n = _s_start; n < _s_stop; n += _s_step)
				{
					
					sprintf(buff, "%d", n);
					sbuff = buff;
					poll.push_back(sbuff);
				}
				break;

			default:
				mpshell << "UNKNOW COMMAND MODE!" << endl;
				return -1;
				break;
		}
	string command;
	for (int n = cmdstart+1; n < argc; n++)
	{
		command += argv[n];
		command += " ";
	}
	//OMP_START
#pragma omp parallel for 
	for (int n = 0; n < poll.size(); n++)
	{
		string buff = strreplace(command.data(), "##OMP",poll.at(n).data());
		char cbuff[MAX_BUFF_SIZE] = { 0x00 };
		sprintf(cbuff, "%d", omp_get_thread_num());
		buff = strreplace(buff.data(), "##MP_TN", cbuff);
		memset(cbuff, 0, sizeof(cbuff));
		sprintf(cbuff, "%d", abs(rand()*rand()*rand()));
		buff = strreplace(buff.data(), "##MP_ID", cbuff);
		exec(buff);
	}
	return 1;
}
