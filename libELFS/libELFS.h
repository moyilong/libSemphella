#pragma once
#include <libSemphella/main.h>
#include <libSemphella/string.h>
#include <libSemphella/debug.h>
#include <libSemphella/crypt.h>

#ifndef __LINUX__

#ifdef LIBELFS_EXPORTS
#define EAPI __declspec(dllexport)
#else
#define EAPI __declspec(dllimport)
#endif

#else

#define API

#endif
#include "head_def.h"
class EAPI EFS {
public:
	EFS();
	EFS(string filename);
	EFS(string *filename, int matrix_size);
	EFS(string filename, uint64_t block_size);
	EFS(string *filename, int matrix_size, uint64_t block_size);
	bool read(char *buff,uint64_t address, uint64_t offset = -1);
	bool write(const char *buff, uint64_t address, uint64_t offset = -1);
	uint64_t GetBlockSize() const;
	uint64_t GetStorageSize() const;
	uint64_t GetBlockLen() const;


	attribute GetAttr(int attr_id) const;
	attribute GetAttr(string name) const;
	int GetAttrID(string name) const;
	void WriteAttr(const attribute attr);


private:
	uint64_t ptr = 0;
	efs_head head;

};
