#include "apd.h"
#include "debug.h"
#include "string.h"
#include "crypt.h"
#define apd debug <<"[APD-Framework]"
#define HEAD_LEN	8
const char bin_head[HEAD_LEN] = { 'A','P','D','B',(char)0x80,(char)0x59,(char)0x6b,(char)0x7e };
APD::APD()
{
	apd << "Apd is init a empty Class" << endl;
}

APD::~APD()
{
	apd << "APD :" << filename << " was been clean" << endl;
	poll.clear();
	fileio.close();
}

APD::APD(string filename)
{
	if (filename.empty())
	{
		apd << "Warring:Use NONE_NULL_LINK to init a APD Class" << endl;
		abort();
	}
	load(filename);
}

void APD::load(string filename)
{
	this->filename = filename;
	if (filename.empty())
	{
		cout << "Load a NULL File Name!" << endl;
		abort();
	}
	fileio.open(filename.data(), ios::in | ios::out);
	if (!fileio.is_open())
	{
		apd << "open file faild!" << endl << "\tOpen:" << filename << endl;
		return;
	}
	char head[HEAD_LEN];
	fileio.read(head, HEAD_LEN);
	bin_mode = memequal(head, bin_head,HEAD_LEN);
	if (!bin_mode)
	{
		fileio.seekp(0);
		fileio.seekg(0);
		NODE buff;
		bool first = true;
		while (!fileio.eof())
		{
			string sbuff;
			getline(fileio, sbuff);
			apd << "ReadLine:" << sbuff << endl;
			if (sbuff.size() == 0)
				continue;
			if (sbuff[0] == '#')
				continue;
			if (sbuff.at(0) == '['&&sbuff.at(sbuff.size()-1))
			{
				if (first)
				{
					apd << "First Node Found!" << endl;
					first = false;
				}
				else {
					apd << "Begin Insert Check..." << endl;
					if (check_node(buff.n_name) != -1)
					{
						apd << "node name replace!" << endl;
					}
					else
					{
						apd << "insert:" << buff.n_name << endl;
						poll.push_back(buff);
					}
				}
				buff.n_name = sbuff.substr(1, sbuff.size() - 2);
				buff.label.clear();
			}
			else {
				label_t xbuff;
				int equal = strfind(sbuff.data(), '=', true);
				if (equal == 0)
				{
					apd << "file format error!" << endl << "Error Place:" << sbuff << endl;
				}
				else {
					xbuff.name = sbuff.substr(0, equal);
					xbuff.data = sbuff.substr(equal + 1);
					apd << "Pushd:" << buff.n_name << "//" << xbuff.name << " = " << xbuff.data << endl;
					buff.label.push_back(xbuff);
				}
			}
		}
		if (buff.label.size() != 0)
		{
			apd << "->Push:" << buff.n_name << endl;
			poll.push_back(buff);
		}
	} 
	else {
		fileio.seekg(HEAD_LEN);
		fileio.seekp(HEAD_LEN);
		uint64_t node_size;
		fileio.read((char*)&node_size, sizeof(uint64_t));
		for (uint64_t n = 0; n < node_size; n++)
		{
			NODE node;
			uint64_t lab_size;
			fileio.read((char*)&lab_size, sizeof(uint64_t));
			for (uint64_t x = 0; x < lab_size; x++)
			{
				uint64_t str_len;
				fileio.read((char*)&str_len, sizeof(uint64_t));
				emmx mem(str_len);
				fileio.read(mem.ptr, str_len);
				string name = mem.ptr;
				fileio.read((char*)&str_len, sizeof(uint64_t));
				emmx mem2(str_len);
				fileio.read(mem2.ptr, str_len);
				string val = mem2.ptr;
				label_t temp = { name,val };
				node.label.push_back(temp);
			}
			uint64_t str_len;
			fileio.read((char*)&str_len, sizeof(uint64_t));
			emmx mem(str_len);
			fileio.read(mem.ptr, str_len);
			node.n_name = mem.ptr;
			poll.push_back(node);
		}
	}
	fileio.close();
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

string APD::get_label(COUNT_TYPE node, string lab)
{
	if (check_label(node, lab) == -1)
	{
		apd << "label not find!" << endl << "Lable:" << lab << endl;
		return"";
	}
	return poll.at(node).label.at(check_label(node, lab)).data;
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
		label_t temp;
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
		{
			ret = n;
		}
	apd << "Find " << node << "=" << ret << endl;
	return ret;
}

COUNT_TYPE APD::check_label(string node, string lab)
{
	COUNT_TYPE xnode = check_node(node);
	if (xnode == -1)
	{
		apd << "CheckNode was not exist node!" << endl;
		return -1;
	}
	COUNT_TYPE ret = -1;
#pragma omp parallel for
	for (COUNT_TYPE n = 0; n < poll.at(xnode).label.size(); n++)
		if (streval(poll.at(xnode).label.at(n).name.data(), lab.data()))
			ret = n;
	return ret;
}

COUNT_TYPE APD::check_label(COUNT_TYPE node, string label)
{
	COUNT_TYPE ret = -1;
#pragma omp parallel for
	for (COUNT_TYPE n = 0; n < poll.at(node).label.size(); n++)
		if (streval(poll.at(node).label.at(n).name.data(), label.data()))
			ret = n;
	return ret;
}

string APD::GetNode(COUNT_TYPE id)
{
	return poll.at(id).n_name;
}



void APD::node_for_each(for_each_api *api, for_each_check *check, bool omp)
{
	if (omp)
#pragma omp parallel for
		for (COUNT_TYPE n = 0; n < poll.size(); n++)
		{
			if (check(poll.at(n).n_name))
				api(poll.at(n), n); 
		}
	else {
		for (COUNT_TYPE n = 0; n < poll.size(); n++)
			if (check(poll.at(n).n_name))
				api(poll.at(n), n);
	}
}

void APD::save()
{
	fileio.open(filename.data(), ios::in | ios::out|ios::trunc);
	if (!fileio.is_open()) {
		apd << "resume file stat!" << endl;
		apd << "Operating File:" << filename << endl;
		fileio.open(filename.data(), ios::in | ios::out);
		if (!fileio.is_open())
		{
			apd << "File Resume Faild!" << endl;
			return;
		}
	}
	apd << "File is Reenable!" << endl;
	if (bin_mode)
	{
		fileio.write(bin_head, HEAD_LEN);
		uint64_t len = poll.size();
		fileio.write((char*)&len, sizeof(uint64_t));
		for (uint64_t n = 0; n < poll.size(); n++)
		{
			uint64_t blen = poll.at(n).label.size();
			fileio.write((char*)&blen, sizeof(uint64_t));
			for (uint64_t x = 0; x < poll.at(n).label.size(); x++)
			{
				uint64_t slen=poll.at(n).label.at(x).name.size();
				fileio.write((char*)&slen, sizeof(uint64_t));
				fileio.write(poll.at(n).label.at(n).name.data(), poll.at(n).label.at(x).name.size());
				uint64_t slen2 = poll.at(n).label.at(x).data.size();
				fileio.write((char*)&slen2, sizeof(uint64_t));
				fileio.write(poll.at(n).label.at(n).data.data(), poll.at(n).label.at(x).data.size());

			}
			uint64_t slen = poll.at(n).n_name.size();
			fileio.write((char*)&slen, sizeof(uint64_t));
			fileio.write(poll.at(n).n_name.data(), poll.at(n).n_name.size());
		} 
	}
	else {
		for (COUNT_TYPE n = 0; n < poll.size(); n++)
		{
			fileio << "[" << poll.at(n).n_name << "]" << endl;
			for (COUNT_TYPE x = 0; x < poll.at(n).label.size(); x++)
				fileio << poll.at(n).label.at(x).name << "=" << poll.at(n).label.at(x).data << endl;
		}
	}
	fileio.close();
}

uint64_t APD::node_size()
{
	return poll.size();
}

uint64_t APD::lab_size(string node_name)
{
	COUNT_TYPE id = check_node(node_name);
	if (id == -1)
		return -1;
	return poll.at(id).label.size();

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
