#include "libMps.h"

libMps::libMps()
{
}

libMps::libMps(const uint64_t size)
{
	poll = vector<string>(size);
}

libMps::libMps(const vector<string> zpoll)
{
	poll = vector<string>(poll);
}

uint64_t libMps::poll_size() const
{
	return poll.size();
}

uint64_t libMps::cmd_size() const
{
	return cmds.size();
}


void libMps::clean()
{
	poll.clear();
}

int libMps::run(string cmds)
{
	debug << "Running Command:" << cmds << endl;
	if (dry_run)
	{
		cout << "DryRun:\"" << cmds << "\""<<endl;
	}
	else
	{
		return system(cmds.data());
	}
}

int libMps::execute(uint64_t cid,uint64_t pid)
{
	if (cid >= cmds.size())
		return -1;
	if (pid > poll.size())
		return -1;
	string recv = cmds.at(cid);
	recv=strreplace(recv, "##OMP", poll.at(pid));
	debug << "Running:" << cid << "@" << pid << " &= " << recv << endl;
	return run(recv);
}

bool libMps::execute(uint64_t id)
{
	if (id > cmds.size())
		return false;
	bool stat = true;
#pragma omp parallel for
	for (int64_t n = 0; n < poll.size(); n++)
			if (execute(id, n) != 0)
				stat = false;
}

bool libMps::execute(bool keep,bool mps)
{
	set_omp(mps);
	bool stat = true;
	for (int64_t n = 0; n < cmds.size() + cmds.size(); n++)
	{
		if (stat || keep)
		{
			if (execute((uint64_t)n) != 0)
				stat = false;
		}
	}
	return stat;
}

void libMps::add(const string cmds)
{
	poll.push_back(cmds);
}

void libMps::operator+=(const string cmds)
{
	add(cmds);
}

void libMps::operator=(const string cmds)
{
	clean();
	add(cmds);
}

libMps libMps::operator+(const string cmd)
{
	libMps ret(*this);
	ret.add(cmd);
	return ret;
}

const vector<string> libMps::get_poll() const
{
	return poll;
}

const vector<string> libMps::get_cmds() const
{
	return cmds;
}

void libMps::add_cmd(const string cmd)
{
	cmds.push_back(cmd);
}

void libMps::set_omp(bool mps)
{
	omp_set_num_threads(mpsize);
	if (!mps|| dry_run)
		omp_set_num_threads(1);
}


