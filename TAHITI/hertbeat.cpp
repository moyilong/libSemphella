#include "stdafx.h"
#include "feature_define.h"

#include <time.h>

DEVICE local;

bool local_init = false;


vector<ROUTING_INFO> rpoll;

inline DEVICE_ID get_dev_id()
{
	DEVICE_ID ret=rand();
	for (int n = 0; n < MAX_BUFF_SIZE; n++)
		ret += rand();
	return ret;
}

vector<DEVICE>dev_poll;

inline DEVICE GETLOCAL()
{
	if (!local_init)
	{
		if (kernel().deviId != 0)
			local.id = kernel().deviId;
		else
			local.id = get_dev_id();
		local_init = true;
		strcpy(local.name, kernel().device_name.data());
	}
	return local;
}

void Data_Routing_Server( DATA_FORMAT in, DATA_FORMAT &ret, SOCKET &conn)
{
	ROUTING_INFO info;
	memcpy(&info, in.buff, sizeof(ROUTING_INFO));
	rpoll.push_back(info);
}

void Hert_beat_server(DATA_FORMAT in, DATA_FORMAT &ret, SOCKET &conn)
{
	bool newd = true;
	for (int n = 0; n < dev_poll.size(); n++)
		if (dev_poll.at(n).id == in.dev.id)
		{
			cout << "Devices:" << in.dev.name << " was been update time out:" << clock() - dev_poll.at(n).last << endl;
			ret.dev.last = dev_poll.at(n).last;
			dev_poll.at(n).last = clock();
			newd = false;
			break;
		}
	if (newd)
	{
		cout << "Regist a new device:" << in.dev.id << " = " << in.dev.name << endl;

		DEVICE temp;
		memcpy(&temp, &in.dev, sizeof(DEVICE));
		temp.last = time(0);
		dev_poll.push_back(temp);
		ret.dev.last = 0;
	}
	cout << "Processing Routing Info.." << endl;
	for (int n = 0; n < rpoll.size(); n++)
	{
		cout << "Processing :" << rpoll.at(n).name << endl;
		if (!strcmp(rpoll.at(n).name, in.dev.name))
		{
			cout << "Routing Information Match!" << endl;
			ret.def = rpoll.at(n).def;
			memcpy(ret.buff, rpoll.at(n).data, DATA_LEN / 2);
			rpoll.erase(rpoll.begin() + n);
			break;
		}
	}
}

void Hert_beat_client_to( DATA_FORMAT &data)
{
	//memcpy(&data.dev, &local, sizeof(DEVICE));
	data.dev = GETLOCAL();

}

void Hert_beat_client_ret( DATA_FORMAT &data)
{
	local.last = data.dev.last;
	DEBUG_LINE cout << "last get time:" << local.last << endl;
}

int dev_size()
{
	return dev_poll.size();
}

int routing_size()
{
	return rpoll.size();
}

DEVICE GetDevice(int n)
{
	if (n > dev_size())
	{
		DEVICE err;
		err.id = -1;
		return err;
	}
	return dev_poll.at(n);
}

ROUTING_INFO GetRouting(int n)
{
	if (n > rpoll.size())
	{
		ROUTING_INFO err;
		err.def = -1;
		return err;
	}
	return rpoll.at(n);
}