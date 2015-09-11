#include "../stdafx.h"
#include "../modules.h"
#include <time.h>
#include "../feature_define.h"

DEVICE local;
bool local_init = false;

inline DEVICE_ID get_dev_id()
{
	DEVICE_ID ret=rand();
	int stop = rand();
	for (int n = 0; n < stop; n++)
		ret += rand();
	return ret;
}

vector<DEVICE>dev_poll;

void Hert_beat_server(struct DATA_FORMAT in, struct DATA_FORMAT &ret, SOCKET &conn)
{
	for (int n = 0; n < dev_poll.size();n++)
		if (dev_poll.at(n).id == in.dev.id)
		{
		strcpy(local.name, "test!");
		ret.dev.last = dev_poll.at(n).last;
		dev_poll.at(n).last = time(0);
		return;
		}
	cout << "Regist a new device:" << in.dev.id << " = " << in.dev.name << endl;
	DEVICE temp;
	memcpy(&temp, &in.dev, sizeof(DEVICE));
	temp.last = time(0);
	dev_poll.push_back(temp);
	ret.dev.last = 0;
}

void Hert_beat_client_to(struct DATA_FORMAT &data)
{
	if (!local_init)
	{
		local.id = get_dev_id();
		local_init = true;
	}
	memcpy(&data.dev, &local, sizeof(DEVICE));

}

void Hert_beat_client_ret(struct DATA_FORMAT &data)
{
	local.last = data.dev.last;
}

Modules Hert_beat_link(0xFFFF0001,"HertBeat", Hert_beat_server, Hert_beat_client_to, Hert_beat_client_ret);

DEVICE GETLOCAL()
{
	return local;
}