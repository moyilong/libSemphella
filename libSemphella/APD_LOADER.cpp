#include "apd.h"
#include "debug.h"
#include "string.h"
#include "crypt.h"
libDebug apd("APD Framework");

namespace APD_UTILS
{

	APD::APD()
	{

	}

	APD::APD(string filename, bool bin_stat, bool crypt_stats, string passwd)
	{
		if (!bin_stat)
		load(filename);
		else{
			bin_mode = true;
			cryptd = crypt_stats;
			p_password = passwd;
			import_bin(filename);
		}
	}

	APD::~APD()
	{
		poll.clear();
		fileio.close();
	}

	void APD::load(string filename)
	{
		fileio.open(filename.data(), ios::in | ios::out);
		if (!fileio.is_open())
		{
			apd << "open file faild!" << endl << "\tOpen:" << filename << endl;
			return;
		}
		NODE buff;
		while (!fileio.eof())
		{
			string sbuff;
			getline(fileio, sbuff);
			if (sbuff.at(0) == '[')
			{
				if (buff.label.size() != 0)
				{
					if (check_node(buff.n_name) != -1)
					{
						apd << "node name replace!" << endl;
					}
					else
						poll.push_back(buff);
				}
				buff.n_name = sbuff.substr(1, sbuff.size() - 2);
				buff.label.clear();
			}
			else{
				label xbuff;
				int equal = strfind(sbuff.data(), '=');
				if (equal == 0)
				{
					apd << "file format error!" << endl << "Error Place:" << sbuff << endl;
				}
				else{
					xbuff.name = sbuff.substr(0, equal);
					xbuff.data = sbuff.substr(equal + 1);
					buff.label.push_back(xbuff);
				}

			}
		}
	}
	string APD::get_label(string node, string lab)
	{
		if (check_node(node) == -1)
		{
			apd << "node not find!" << endl << "Node:" << node << endl;
			return "";
		}
		if (check_label(node, lab) == -1)
		{
			apd << "label not find!" << endl << "Lable:" << lab << endl;
			return"";
		}
		return poll.at(check_node(node)).label.at(check_label(node, lab)).data;
	}

	void APD::write_label(string node, string lab, string data)
	{
		if (check_node(node) == -1)
		{
			apd << "node not find!" << endl << "Node:" << node << endl;
			NODE buff;
			buff.n_name = node;
			poll.push_back(buff);
		}
		if (check_label(node, lab) == -1)
		{
			apd << "label not find!" << endl << "Lable:" << lab << endl;
			label temp;
			temp.name = lab;
			temp.data = data;
			poll.at(check_node(node)).label.push_back(temp);
			return;
		}
		poll.at(check_node(node)).label.at(check_label(node, lab)).data = data;
	}

	COUNT_TYPE APD::check_node(string node)
	{
		COUNT_TYPE ret = -1;
#pragma omp parallel for
		for (COUNT_TYPE n = 0; n < poll.size(); n++)
			if (streval(poll.at(n).n_name.data(), node.data()))
				ret = n;
		return ret;
	}

	COUNT_TYPE APD::check_label(string node, string lab)
	{
		COUNT_TYPE xnode = check_node(node);
		if (xnode == -1)
			return -1;
		COUNT_TYPE ret=-1;
#pragma omp parallel for
		for (COUNT_TYPE n = 0; n < poll.at(xnode).label.size(); n++)
			if (streval(poll.at(xnode).label.at(n).name.data(), lab.data()))
				ret = n;
		return ret;
	}

	void APD::node_for_each(for_each_api *api, for_each_check *check, bool omp )
	{
		COUNT_TYPE omp_size = omp_get_num_threads();
		if (omp)
			omp_set_num_threads(1);
#pragma omp parallel for
		for (COUNT_TYPE n = 0; n < poll.size(); n++)
		{
			if (check(poll.at(n).n_name))
				api(poll.at(n),n);
		}

	END:
		omp_set_num_threads(omp_size);
	}

	void APD::save()
	{
		fileio.close();
		string cmd;
#ifdef __LINUX__
		cmd+="rm ";
#else
		cmd+="del ";
#endif
		cmd+=filename;
		system(cmd.data());
		fileio.open(filename, ios::in | ios::out);
		if (!fileio.is_open()){
			apd << "clean file faild!" << endl<<"resume file stat!"<<endl;
			fileio.open(filename, ios::in | ios::out);
			if (!fileio.is_open())
			{
				apd << "File Resume Faild!" << endl;
				return;
			}
		}
		
		for (COUNT_TYPE n = 0; n < poll.size(); n++)
		{
			fileio << "[" << poll.at(n).n_name << "]" << endl;
			for (COUNT_TYPE x = 0; x < poll.at(n).label.size(); x++)
				fileio << poll.at(n).label.at(x).name << "=" << poll.at(n).label.at(x).data << endl;
		}
	}

	void APD::remove(string node)
	{
		COUNT_TYPE id = check_node(node);
		if (id == -1)
		{
			apd << "remove node not exist`1" << endl << " remove : " << node << endl;
			return;
		}
		poll.erase(poll.begin() + id);
	}

	void APD::remove(string node, string lab)
	{
		COUNT_TYPE id = check_node(node);
		if (id == -1)
		{
			apd << "remove node not exist!" << endl << " remove : " << node << endl;
			return;
		}
		COUNT_TYPE lid = check_label(node, lab);
		if (lid == -1)
		{
			apd << "remove label is not exit!" << endl << "remove:" << node << ">>" << lab << endl;
			return;
		}
		poll.at(id).label.erase(poll.at(id).label.begin() + lid);
	}

	void APD::import_bin(string bin_name)
	{
		filename = bin_name;
		fileio.open(bin_name);
		if (!fileio.is_open())
		{
			apd << "open file faild!" << endl;
			return;
		}
		char buff[sizeof(APD_BIN)] = { 0 };
		fileio.read(buff, sizeof(APD_BIN));
		if (cryptd)
			crypt(buff, sizeof(APD_BIN), p_password);
		APD_BIN head;
		memcpy(&head, buff, sizeof(APD_BIN));
		if (!streval(head.diff_str, PSTR))
		{
			apd << "file read faild! file format error or password not match!" << endl;
			return;
		}
		for (COUNT_TYPE n = 0; n < head.node_size; n++)
		{
			NODE temp;
			char nbuff[sizeof(NODE_INFO)] = { 0x00 };
			fileio.read(nbuff, sizeof(NODE_INFO));
			if (cryptd)
				crypt(nbuff, sizeof(NODE_INFO), p_password);
			NODE_INFO readed;
			memcpy(&readed, nbuff, sizeof(NODE_INFO));
			for (COUNT_TYPE g = 0; g < readed.label_size; g++)
			{
				char lbuff[sizeof(LABEL_INFO)] = { 0x00 };
				LABEL_INFO cache;
				fileio.read(lbuff, sizeof(LABEL_INFO));
				//char data_buff[MAX_BUFF_SIZE];
				//char name_buff[MAX_BUFF_SIZE];
				if (cryptd)
					crypt(lbuff, sizeof(LABEL_INFO), p_password);
				memcpy(&cache, lbuff, sizeof(LABEL_INFO));
				char *data_buff = (char*)malloc(cache.datalen);
				char *name_buff = (char*)malloc(cache.namelen);
				fileio.read(name_buff, cache.namelen);
				fileio.read(data_buff, cache.datalen);
				if (cryptd)
				{
					crypt(data_buff, cache.datalen, p_password);
					crypt(name_buff, cache.namelen, p_password);
				}
				label pushd;
				pushd.data = data_buff;
				pushd.name = name_buff;
				temp.label.push_back(pushd);
				free(data_buff);
				free(name_buff);
			}
			temp.n_name = readed.n_name;
			poll.push_back(temp);
		}
	}

	void APD::export_bin( )
	{
		fileio.close();
		fileio.open(filename, ios::in | ios::out | ios::trunc);
		if (!fileio.is_open()){
			apd << "clean file faild!" << endl << "resume file stat!" << endl;
			fileio.open(filename, ios::in | ios::out);
			if (!fileio.is_open())
			{
				apd << "File Resume Faild!" << endl;
				return;
			}
		}
		APD_BIN bin;
		bin.node_size = poll.size();
		strcpy(bin.diff_str, PSTR);
		if (cryptd)
			crypt((char*)&bin, sizeof(APD_BIN), p_password);
		fileio.write((char*)&bin, sizeof(APD_BIN));
		for (COUNT_TYPE n = 0; n < poll.size(); n++)
		{
			NODE_INFO cache;
			cache.label_size = poll.at(n).label.size();
			strcpy(cache.n_name, poll.at(n).n_name.data());
			if (cryptd)
				crypt((char*)&cache, sizeof(NODE_INFO), p_password);
			fileio.write((char*)&cache, sizeof(NODE_INFO));
			for (COUNT_TYPE x = 0; x < poll.at(n).label.size(); x++)
			{
				char *name_buff = (char*)malloc(poll.at(n).label.at(x).name.size());
				char *data_buff = (char*)malloc(poll.at(n).label.at(x).data.size());
				strcpy(name_buff, poll.at(n).label.at(x).name.data());
				strcpy(data_buff, poll.at(n).label.at(x).data.data());
				LABEL_INFO writed;
				writed.datalen = poll.at(n).label.at(x).data.size();
				writed.namelen = poll.at(n).label.at(x).name.size();
				if (cryptd)
				{
					crypt(name_buff, writed.namelen, p_password);
					crypt(data_buff, writed.datalen, p_password);
					crypt((char*)&writed, sizeof(LABEL_INFO), p_password);
				}
				fileio.write((char*)&writed, sizeof(LABEL_INFO));
				fileio.write(name_buff, writed.namelen);
				fileio.write(data_buff, writed.datalen);
				free(name_buff);
				free(data_buff);
			}
		}

	}
	/*
	void APD::enable_bin_mode() 
	{
		bin_mode = true; 
	}
	void APD::disable_bin_mode() 
	{ 
		bin_mode = false; 
	}

	void APD::enable_crypt(string password)
	{
		cryptd = true;
		p_password = password;
		return;
	}
	void APD::disable_crypt() 
	{ 
		cryptd = false;
		return;
	}
	*/
	void APD::set_bin_mode(bool stat)
	{
		bin_mode = stat;
	}
	void APD::set_crypt_mode(bool cpy)
	{
		cryptd = cpy;
	}
	void APD::set_password(string pwd)
	{
		char buff[MAX_BUFF_SIZE] = { 0x00 };
		crypt(buff, MAX_BUFF_SIZE, pwd);
		p_password = buff;


	}


}
