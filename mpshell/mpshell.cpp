// mpshell.cpp : 定义控制台应用程序的入口点。
//

#include <libSemphella/libSemphella.h>
#include <libSemphella/debug.h>
#include <libSemphella/string.h>
#include <omp.h>

//libDebug mpshell("MPShell");

#define mpshell debug<<"[MPShell][" << omp_get_thread_num()<<"]"

vector<string>poll;

//USEAGE:	mpshell <args> -c <shell>
//REPLACE:	##OMP	OMP循环条目
//			##MP_TN		OMP线程编号
//			##MP_ID	OMP随机编号
bool testmode = false;
bool disable_nl = false;
vector<string> cmd;
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
			case 'h':
				cout << "MPShell Useage:" << endl;
				cout << "mpshell <args> -c <shell>"<<endl;
				cout << " Replace List:" << endl;
				cout << "\t##OMP\tOMP循环条目" << endl;
				cout << "\t##MP_TN\tOMP线程编号" << endl;
				cout << "\t##MP_ID\tOMP随机编号" << endl;
				exit(0);
			case 's':
				for (int x=n+1;x<argc;x++)
					if (argv[x][0] == '-')
						break;
					else
						poll.push_back(argv[x]);
				break;
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
				 if (_s_step == 0)
					 KERNEL.error("Step is zero!");
				 mpshell << _s_start << " + " << _s_step << " => " << _s_stop << endl;
				for (int n = _s_start; n < _s_stop; n += _s_step)
				{
					
					sprintf(buff, "%d", n);
					sbuff = buff;
					poll.push_back(sbuff);
				}
				break;
			case '-':
				if (streval(argv[n], "--disable-nullline"))
					disable_nl = true;
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
	mpshell << "into execute mode!" << endl;
	mpshell << "Command:" << command << endl;
	mpshell << "poll size:" << poll.size() << endl;
	//OMP_START
	omp_set_num_threads(1);
//#pragma omp parallel for 
	for (int n = 0; n < poll.size(); n++)
		if (!(disable_nl == true && poll.at(n).empty()))
		{
			string getd = strreplace(command.data(), "##OMP", poll.at(n).data());
			mpshell << "HaveCMD:" << getd << endl;
			cmd.push_back(getd);
		}
#pragma omp parallel for
	for (int n = 0; n < cmd.size(); n++)
	{
		mpshell << "Execing:" << cmd.at(n) << endl;
		if (testmode)
			cout << "Exec:" << cmd.at(n) << endl;
		else
			system(cmd.at(n).data());
	}
	return 1;
}
