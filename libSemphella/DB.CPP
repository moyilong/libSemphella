#include "..\buildhost\feature.h"
#include <iostream>
using namespace std;
#define pre_alloc_size	512
db::db(string filename)
{
	load(filename);
}
void db::load(string filename)
{
	string m_file = filename + ".exb";
	string l_file = filename + ".logd";
	log_file.open(l_file, "ab");
	main_file.open(m_file, "rb");
	if (log_file.is_open() || main_file.is_open())
	{
		cout << "Open File Faild!" << endl;
		return;
	}
	for (uint64_t n = 0; n < log_file.tell_len() / sizeof(build_log); n++)
	{
		build_log log;
		log_file.read(&log, 1);
		logdb.push_back(log);
		if (logdb.size() > log_keep_size)
			logdb.erase(logdb.begin() + 0);
	}
	main_file.read(&data, 1);

}
build_log db::add_log(char project[NAME_LEN], char message[MESSAGE_LEN])
{
	build_log ret;
	memcpy(ret.project, project,NAME_LEN);
	memcpy(ret.message, message, MESSAGE_LEN);
	ret.date = time(0);
	CreateUID(ret.uid);
	ret.id = add_log(ret);
	return ret;
}
uint64_t db::add_log(build_log logs)
{
	build_log xdata = logs;
	data.build_id += 1;
	xdata.id = data.build_id;
	logdb.push_back(xdata);
	if (log_file.is_open())
		log_file.write((char*)&xdata, sizeof(build_log));
	save_main_data();
	if (logdb.size() > log_keep_size)
		logdb.erase(logdb.begin() + 0);
	return data.build_id;
}

void db::save_main_data()
{
	main_file.seekp(0);
	main_file.write(&data, 1);
}
