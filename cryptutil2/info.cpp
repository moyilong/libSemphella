#include "cryptutil2.h"


int information_process()
{
	if (input.empty())
	{
		cout << "Argment Error!" << endl;
	}
	else {
		if (!file_name_check(input))
		{
			cout << "File Name Secure Check Faild!" << endl;
			cout << "The ERT File last name is must be .ert2 or .ert3" << endl;
			return -1;
		}
		ifstream in;
		in.open(input.data());
		if (!in.is_open())
		{
			cout << "Open File Faid!" << endl;
			exit(-1);
		}
		HEAD head;
		cout << hex;
		in.read((char*)&head, sizeof(head));
		head.check();
		cout << "Password Checksum:" << head.password_sum << endl;
		cout << "Checksum:" << head.sum << endl;
		cout << "Block size:" << head.bs << endl;
		cout << "Algorithm:" << (int)head.algrthom << endl;
		cout << "FHandle:" << (int)head.ext[EXT_FHANDLE] << endl;
		if (crack_get)
		{
			uint64_t count = 0;
			time_t missing = time(0);
			MP_BLOCK *block = new MP_BLOCK[omp_get_num_procs()];
			for (uint64_t v = 0; v < numeric_limits<uint64_t>::max(); v += omp_get_num_procs())
			{
#pragma omp parallel
			{
				int id = omp_get_thread_num();
				uint64_t n = v + id;
				block[id].count++;
				char str[MAX_BUFF_SIZE];
				eitoa((uint64_t)n, str, strlen(strtbl), strtbl);
				if (APOLL[trans_id(head.algrthom)].px(password) == head.password_sum)
				{
					cout << "Password is Found!" << endl;
					cout << str << endl;
					cout << "Timeout:" << time(0) - missing << endl;
					exit(0);
				}

				if (block[id].count % 1000000 == 0)
				{
					block[id].ilen = strlen(str);
					if (time(0) - missing > 0)
						block[id].iops = count / (time(0) - missing);
					else
						block[id].iops = -1;
					block[id].temp = str;
					block[id].n = n;
					block[id].per = (double)strfind(strtbl, str[0], true) / strlen(strtbl);
				}
			}
			if (block[0].count % 1000000 == 0 && block[0].count > 10000000)
			{
				uint64_t ccc = 0;
				double max = 0;
				for (int x = 0; x < omp_get_num_procs(); x++)
				{
					cout << "[" << x << "][";
					cout << " iLen:" << int2s(block[x].ilen) << " Temp:\"" << block[x].temp << "\" ";
					cout << "] ";
					max = emax(max, block[x].per);
					ccc += block[x].count;
				}
				cout << endl;
				double iops = ccc / (time(0) - missing);
				string dis = "iLen:" + int2s(block[0].temp.size()) + " IOPS:" + ull2s(iops);
				ShowProcessBar(max, dis);
				cout << "\r";
			}

			/*str_add(str);*/
			}
		}
		exit(0);
	}
	return 0;
}
