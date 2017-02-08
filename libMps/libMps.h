#pragma once
#include <libSemphella/main.h>
#include <libSemphella/string.h>
#include <libSemphella/debug.h>

#ifdef MPS_EXPORT;
class __declspec(dllexport) libMps {
#else
class __declspec(dllimport) libMps {
#endif
public:
#define exe execute
	libMps();
	libMps(const uint64_t size);
	libMps(const vector<string> zpoll);
	uint64_t cmd_size() const;
	uint64_t poll_size() const;
	void clean();
	int execute(uint64_t cid, uint64_t pid);
	bool execute(uint64_t id);
	bool execute(bool keep=true,bool mps=true);
	void add(const string val);
	void operator +=(const string val);
	void operator = (const string val);
	libMps operator + (const string val);
	const vector<string> get_poll() const;
	const vector<string> get_cmds() const;
	void add_cmd(const string cmd);
public:
	int mpsize = omp_get_thread_num();
	bool dry_run = false;
private:
	vector<string> poll;
	vector<string> cmds;
	void set_omp(bool mps=true);
	int run(string cmds);
};