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
		block.read(&blk_keep, 1);
		if (!VerifyFileHead())
			return;
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

void Compress::ReadBuffer(string filename, uint64_t begin, uint64_t length, char * buffc)
{
	uint64_t begin_block = begin / head.block_len;
	uint64_t begin_offset = begin - (head.block_len*begin_block);
	uint64_t end_block =  (length-begin_offset) / head.block_len;
	uint64_t end_offset = (length - begin_offset) - (head.block_len*begin_block);
	char *buff = (char*)malloc(head.block_len);
	uint64_t id = -1;
	for (uint64_t n = 0; n < fat.size();n++)
		if (strcmp(fat.at(n).infile.name, filename.data()))
		{
			id = n;
			break;
		}
	if (id == -1)
	{
		memset(buff, 0, length);
		free(buff);
		return;
	}
	for (uint64_t n = begin_block+1; n < end_block; n++)
	{
		ReadBuff(fat.at(id).bcc.at(n + begin_block),buffc+begin_offset+head.block_len*n);
	}
	ReadBuff(fat.at(begin_block).bcc.at(begin_block),buff);
	memcpy(buffc, buff + (head.block_len - begin_offset), head.block_len - begin_offset);

	ReadBuff(fat.at(id).bcc.at(end_block + begin_block), buff);
	memcpy(buffc + end_offset, buff, end_offset);
	free(buff);
	
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
#include "string.h"
bool Compress::VerifyFileHead()
{
	if (!streval(head._define, PREDEF_HEAD))
		return false;
	if (!streval(blk_keep._define, PREDEF_HEAD))
		return false;
	if (!BlkFormatCompact(blk_keep._ver))
		return false;
	if (!ExtFormatCompact(keep._ver))
		return false;
	if (!ProFormatCompact(head._ver))
		return false;
	return true;
}

bool Compress::Verify(VERIFY_MODE mode,int mpsie)
{
	if (mode == LINK_AND_DATA)
	{
		if (Verify(LINK_TAG, mpsie) && Verify(BLK_DATA, mpsie))
			return true;
		else
			return false;
	}
	switch (mode)
	{
	case LINK_TAG:
		return VerifyLink(mpsie);
	case BLK_DATA:
		return VerifyBlock(mpsie);
	}
	return false;
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
	block.seekp(0);
	block.write(&blk_keep, 1);
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

void Compress::IDReadBuff(uint64_t id, char * ptr)
{
	if (id == -1)
	{
		memset(ptr, 0, head.block_len);
	}
	uint64_t addr = sizeof(block_keep) + head.block_len*id;
	index.seekp(addr);
	index.read(ptr, head.block_len);
}
bool Compress::VerifyLink(int mpsize)
{
	if (mpsize == -1)
		mpsize = omp_get_num_procs();
	for (uint64_t n = 0; n < fat.size(); n+=mpsize)
	{
		bool stat = true;
#pragma omp parallel for
		for (int x = 0; x < mpsize; x++)
			if (stat)
				for (uint64_t b = 0; b < fat.at(n + x).bcc.size(); b++)
					if (stat)
						if (Search(fat.at(n + x).bcc.at(b) == -1))
							stat = false;
		if (!stat)
			return false;
	}
	return true;
}
bool Compress::VerifyBlock(int mpsize)
{
	if (mpsize == -1)
		mpsize = omp_get_num_procs();
	char **temp;
	temp = (char**)malloc(sizeof(char*)*mpsize);
	for (int n = 0; n < mpsize; n++)
		temp[n] = (char*)malloc(head.block_len);
	for (uint64_t n = 0; n < bat.size(); n++)
	{
		bool stat = true;
		for (int x = 0; x < mpsize; x++)
			IDReadBuff(n + x, temp[x]);
#pragma omp parallel for
		for (int x = 0; x < mpsize; x++)
			if (stat)
				if (!getsumV2(temp[n + x], head.block_len))
					stat = false;
		if (!stat)
			return false;
	}
	return true;
}
API extern const int ext_format[] = { 0xA0,0xFF };
API extern const int pro_format[] = { 0xBF,0xFF };
API extern const int blk_format[] = { 0xA1,0xFF };
__inline bool CompactTest(const char ver,const int *match_tree)
{
	int offset = 0;
	while (true)
	{
		if (match_tree[offset] == 0xFF)
			return false;
		if (ver == match_tree[offset])
			return true;
		offset++;
	}
	return false;
}

API bool ExtFormatCompact(int ver)
{
	return CompactTest(ver, ext_format);
}

API bool BlkFormatCompact(int ver)
{
	return CompactTest(ver, blk_format);
}

API bool ProFormatCompact(char ver)
{
	return CompactTest(ver, pro_format);
}
