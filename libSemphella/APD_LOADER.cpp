#include "apd.h"
#include "debug.h"
#include "string.h"
#include "crypt.h"
#define apd debug <<"[APD-Framework]"

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
		else {
			label xbuff;
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
		poll.push_back(buff);
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

void APD::node_for_each(for_each_api *api, for_each_check *check, bool omp)
{
	COUNT_TYPE omp_size = omp_get_num_threads();
	if (omp)
		omp_set_num_threads(1);
#pragma omp parallel for
	for (COUNT_TYPE n = 0; n < poll.size(); n++)
	{
		if (check(poll.at(n).n_name))
			api(poll.at(n), n);
	}

	omp_set_num_threads(omp_size);
}

void APD::save()
{
	fileio.close();
	string cmd;
#ifdef __LINUX__
	cmd += "rm ";
#else
	cmd += "del ";
#endif
	cmd += filename;
	system(cmd.data());
	fileio.open(filename, ios::in | ios::out);
	if (!fileio.is_open()) {
		apd << "clean file faild!" << endl << "resume file stat!" << endl;
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