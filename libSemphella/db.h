#pragma once
#include "apd.h"
#include "crypt.h"
#include "time.h"
#include "utils.h"
#include "libSemphella.h"

struct db_t {
	string tag;
	string name; 
	bool finded = false;
};

class API db {
public:
	db();
	db(string dir);
	void open(string dir);
	db_t GetDB(string name);
	void GetData(const db_t db, char *ptr, uint64_t &data_size);
	vector<db_t> SearchTags(string tags);
	void WriteData(const db_t db, const char *data, const uint64_t data_size);
	bool exist(const db_t db);
	void Remove(string name);
	void Remove(uint64_t id);
	void Remove(db_t db);
private:
	string path;
	APD io;
protected:
	string GetProc(const db_t db);
};