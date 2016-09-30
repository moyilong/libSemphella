#include "compress.h"
#include "crypt.h"

#define BLOCK_FILE	".blk"
#define INDEX_FILE	".idx"

const configure_t def_cfg = {
	128,
};

Compress::Compress(string filename, vector<string> collect, const configure_t cfg)
{
	Import(filename, true,cfg);
	AddFile(collect);
}

Compress::Compress(string filename, bool create, const configure_t cfg)
{
	Import(filename, create,cfg);
}

Compress::Compress()
{
}

Compress::~Compress()
{
}

vector<string> Compress::FileList()
{
	vector<string> ret;
	for (uint64_t n = 0; n < fat.size(); n++)
		ret.push_back(fat.at(n).infile.name);
	return ret;
}

void Compress::Import(string filename, bool create, const configure_t cfg)
{
	basic_target_name = filename;
	InitFileIO("rw");
	if (create)
	{
		head.block_len = cfg.block_len;
		UpdateIDFile();
	}
	else {
		index.read(&head, 1);
		index.read(&keep, 1);
		for (int n = 0; n < head.filesize; n++)
		{
			file_t f;
			index.read(&f.infile, 1);

			for (int x = 0; x < f.infile.block_size; x++)
			{
				uint64_t id;
				index.read(&id, 1);
				f.bcc.push_back(id);
			}
			fat.push_back(f);
		}
		for (int n = 0; n < head.blocksize; n++)
		{
			block_f b;
			index.read(&b, 1);
			bat.push_back(b);
		}
	}
}

void Compress::ReadBuffer(string filename, uint64_t begin, uint64_t length, char * buff)
{

}

void Compress::AddFile(string filename)
{
	file fi;
	fi.open(filename,"r");
	uint64_t len = fi.tell_len();
	uint64_t block_size = fi.tell_len() / head.block_len;
	uint64_t fix = len - (block_size*head.block_len);
	file_t fio;
	for (int n = 0; n < filename.size(); n++)
		fio.infile.name[n] = filename.at(n);

	if (fix > head.block_len)
	{
		fix = fix - head.block_len;
		block_size++;
	}
	char *buff = (char*)malloc(head.block_len);
	for (int n = 0; n < block_size; n++)
	{
		fi.read(buff, head.block_len);
		fio.bcc.push_back(AddBuffers(buff));
	}
	free(buff);
	UpdateIDFile();
}

void Compress::AddFile(vector<string> filename)
{
	for (uint64_t n = 0; n < filename.size(); n++)
		AddFile(filename.at(n));
}

uint64_t Compress::AddBuffers(const char * buff)
{
	uint64_t ret = getsumV2(buff, head.block_len);
	if (Search(ret) == -1)
	{
		head.blocksize++;
		block_f blk;
		blk.id = bat.size();
		blk.hash = ret;
		bat.push_back(blk);
		index.seekp(SEEK_END);
		index.write(&blk, 1);
		block.seekp(SEEK_END);
		block.write(buff, head.block_len);
		index.seekp(0);
		index.write(&head, 1);
	}
	return ret;
}

void Compress::InitFileIO(char * mode)
{
	index.open(basic_target_name + INDEX_FILE, mode);
	if (!index.is_open())
		return;
	block.open(basic_target_name + BLOCK_FILE, mode);
	if (!block.is_open())
		return;
	inited = true;
}

void Compress::UpdateIDFile()
{
	head.filesize = fat.size();
	head.blocksize = bat.size();
	index.seekp(0);
	index.write(&head,1);
	index.write(&keep,1);
	for (uint64_t n = 0; n < fat.size(); n++)
	{
		index.write(&fat.at(n).infile,1);
		for (int b = 0; b < fat.at(n).infile.block_size; b++)
			index.write(&fat.at(n).bcc.at(b), 1);
	}
	for (uint64_t b = 0; b < bat.size(); b++)
		index.write(&bat.at(b), 1);
	index.flush();
}

uint64_t Compress::Search(uint64_t id)
{
	for (uint64_t n = 0; n < bat.size(); n++)
		if (id == bat.at(n).hash)
			return n;
	return -1;
}

