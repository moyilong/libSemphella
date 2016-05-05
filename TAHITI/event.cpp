#include "stdafx.h"

EVENT epoll[MAX_EVENT_SIZE];
int epoll_size=0;

int online_event_size=0;

EVENT::~EVENT()
{
	fun = NULL;
	online_event_size--;
}

EVENT::EVENT(EVENT_TYPE ctype, EVENT_API capi, unsigned char clevel,bool conline)
{
	type = ctype;
	level = clevel;
	fun = capi;
	if (epoll_size>MAX_EVENT_SIZE)
	{
		cout << "error: event registed size is out of "<<MAX_EVENT_SIZE<<"\nPlease Check yout Code or allow more event reg"<<endl;
	}
#ifndef UNLOAD_EVENT
	epoll[epoll_size] = *this;
	epoll_size++;
#endif
	online=conline;
	if (conline)
		online_event_size++;

}

void EVENT::trigger(ENV_DATA data)
{
#ifndef DISABLE_EVENT_TRIGGER
	if (fun != NULL)
		fun(data);
	call_count++;
#endif
}

int EVENT::get_call_count()
{
	return call_count;	
}

void EVENT::get_online()
{
	online=true;
}

void EVENT::get_offline()
{
	online=false;
}

bool EVENT::get_stat()
{
	return online;
}



struct LIST_BLOCK{
	int level;
	int pos;
};

void trigger(EVENT_TYPE type, ENV_DATA data)
{
	vector<LIST_BLOCK>lpoll;
	for (int n = 0; n < epoll_size;n++)
		if (epoll[n].type == type&&epoll[n].get_stat())
		{
		LIST_BLOCK blk;
		blk.level = epoll[n].level;
		blk.pos = n;
		lpoll.push_back(blk);
		}
	for (unsigned char n = 0; n < LEVEL_MAX; n++)
	{
#ifdef ALLOW_PARALLEL_LEVEL_EVENT_PARALLEL_EXEC
#pragma omp parallel for
#endif
		for (int c = 0; c < lpoll.size(); c++)
			if (lpoll.at(c).level == n)
				epoll[lpoll.at(c).pos].trigger(data);
	}
	
}
