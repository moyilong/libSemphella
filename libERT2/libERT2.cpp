#include "libERT2.h"
#include <libSemphella/debug.h>
#include <vector>
#include <libSemphella/crypt.h>
#include "head.h"
using namespace std;
using namespace ERT2;
vector<algorthim> algr_poll;
ERT2::algorthim::algorthim(const int _id, const string name, algorthim_type _algr, hash_type _hash, procd _pwd)
{
	algr_id = _id;
	algr_name = name;
	hash = _hash;
	pwd_init = _pwd;
	algr = _algr;
	for (int n = 0; n < algr_poll.size(); n++)
		if (algr_poll.at(n).algr_id == algr_id)
		{
			debug << "ID Correct:" << algr_id << "->" << name << "<-" << algr_poll.at(n).algr_name << endl;
			return;
		}
	algr_poll.push_back(*this);
}

void ERT2::configure::SetPaswsword(const string password)
{
	password_hash = algr_poll.at(algrid).hash(password.data(), password.size());
	padding_password = password;

}

void ERT2::configure::SetAlgrID(const int id)
{
	for (int n=0;n<algr_poll.size();n++)
		if (algr_poll.at(n).algr_id == id)
		{
			algrid = n;
			return;
		}
	debug << "Search Engine Faild:" << id << endl;
}


bool ERT2::CreatePackage(const vector<string> filelist, const string output, const configure cfg)
{
	ofstream fout;
	fout.open(output.data());
	if (!fout.is_open())
	{
		return false;
	}
	vector<E2FAT> fat;

	E2HEAD head;
	E2FOOT foot;
	foot.block_size = cfg.block_size;
	foot.crypt_filename = cfg.crypt_filename;
	foot.password_hash = cfg.password_hash;
	algorthim algr = algr_poll.at(cfg.algrid);
	algr.pwd_init(cfg.padding_password);
	uint64_t begin = fout.tellp();
	fout.write((char*)&head, sizeof(E2HEAD));
	char *buffers = (char *)malloc(foot.block_size);
	for (int64_t n = 0; n < filelist.size(); n++)
	{
		E2FAT fat;
		strcpy(fat.fname, filelist.at(n).data());
		ifstream fi;
		fi.open(filelist.at(n).data());
		if (!fi.is_open())
			debug << "Open File Faild:" << filelist.at(n) << endl;
		uint64_t len = fi.tellg();
		fi.seekg(ios_base::end);
		len = (uint64_t)fi.tellg() - len;
		fat.filelen = len;
		fat.filebegin = (uint64_t)fout.tellp() - begin;
		uint64_t block_number = fat.filelen % foot.block_size;
		uint64_t block_fix = fat.filelen - (block_number * foot.block_size);
		
		for (uint64_t n = 0; n < block_number; n++)
		{
			memset(buffers, 0, foot.block_size);
			fi.read(buffers, foot.block_size);
			fat.filehash += algr.hash(buffers, foot.block_size);
			algr.algr(buffers, foot.block_size, false);
			fout.write(buffers, foot.block_size);
		}
	}
	

}

bool ERT2::ExtrectFile(const uint64_t fid, const string fname)
{
	return false;
}
