#pragma once
#include "main.h"

enum EMMX_EXCEPTION {
	MEM_OVERLOAD,
	MEM_REPLACE,
};

class API emmx {
public:
	inline emmx() {};
	emmx(uint64_t length);
	~emmx();
	void alloc(uint64_t length);
	void free();
	char operator[] (uint64_t off) const;
	bool operator==(const emmx mmx) const;
	void reallow();
	uint64_t size() const;
	void reset();
	void *value();
private:
	char *ptr;
	bool inited = false;
	uint64_t xsize = 0;
};


API const uint64_t GetGlobalAlloc();