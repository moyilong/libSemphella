#include "algorthim.h"

#define STD_DBU	debug<<"[Alghrthim]"

ALGHRTHIM::ALGHRTHIM(password_algrthom p, crypt_algrthom c, sum_algrthom s, int xid,get_password_checksum pc)
{
	id = xid;
	pa = p;
	ca = c;
	sa = s;
	STD_DBU << "Insert Alghrthim " << id << " @ " << APOLL.size() << endl;
	APOLL.push_back(*this);
}


char _cached_trans_from = -1;
char _cached_trans_to = -1;

uint64_t password_type(string passwd)
{
	return getsumV2(passwd.data(), passwd.size());
}

int trans_id(int id)
{
	if (_cached_trans_from == id)
		return _cached_trans_to;
	int ret = -1;
	for (int n = 0; n < APOLL.size(); n++)
		if (APOLL.at(n).id == id)
		{
			ret = id;
			debug << "Trans ID " << id << " => " << ret << endl;
			_cached_trans_from = id;
			_cached_trans_to = ret;
			return ret;
		}

	return -1;
}
