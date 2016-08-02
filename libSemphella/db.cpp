#include "db.h"

#define HEAD_FILE "head.file"


db::db()
{
}

db::db(string dir)
{
	open(dir);
}

void db::open(string dir)
{
	io.load(dir + "//" + HEAD_FILE);
}

db_t db::GetDB(string name)
{
	db_t ret;
	COUNT_TYPE id=io.check_node(name);
	if (id == -1)
		return ret;
	ret.name = name;
	ret.finded = true;
	ret.tag = io.get_label(name, "tags");
	return ret;
}
#include "files.h"
void db::GetData(const db_t db, char * ptr, uint64_t &data_size)
{
	file fp;
	string fname = path+"//"+GetProc(db);
	fp.open(fname, "r");
	if (fp.is_open())
	{
		ptr = NULL;
		data_size = -1;
		return;
	}
	data_size = fp.tell_len();
	ptr = (char*)malloc(data_size);
	fp.read(ptr, data_size);
	fp.close();

}

vector<db_t> db::SearchTags(string tags)
{
	vector<db_t> ret;
	for (COUNT_TYPE id = 0; id < io.node_size(); id++)
	{
		if (streval(tags.data(), io.get_label(id, "tags").data()))
		{
			//db_t temp = { io.GetNode(id),tags };
			db_t temp;
			temp.name = io.GetNode(id);
			temp.tag = tags;
			ret.push_back(temp);
		}
	}
	return ret;
}

void db::WriteData(const db_t db, const char * data, const uint64_t data_size)
{
	io.write_label(db.name, "tags", db.tag);
	io.save();
	file fp;
	string fname = path + "//" + GetProc(db);
	fp.open(fname, "w");
	fp.write(data, data_size);
	fp.close();
}

bool db::exist(const db_t db)
{
	if (io.check_node(db.name) == -1)
		return false;
	else
		return true;
}
void db::Remove(db_t db)
{
	Remove(db.name);
}
void db::Remove(string name)
{
	db_t temp;
	temp.name = name;
	string path = path + "//" + GetProc(temp);
	io.remove_node(name);
	io.save();
#ifdef __LINUX__
	path = "rm -rf " + path;
#else
	path = "del " + path;
#endif
	system(path.data());
}

void db::Remove(uint64_t id)
{
	Remove(io.GetNode(id));
}

string db::GetProc(const db_t db)
{
	return  "EL_SP_DB_" + ull2s(getsumV2(db.name.data(), db.name.size())) + ".data";
}


