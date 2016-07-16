#include "main.h"
#include "emmx.h"

uint64_t global_alloc=0;


emmx::emmx(uint64_t length)
{
	alloc(length);
}

emmx::~emmx()
{
	try {
		free();
	}
	catch (...)
	{

	}
}

void emmx::alloc(uint64_t length)
{
	if (inited || xsize != 0)
		throw MEM_REPLACE;
	inited = true;
	ptr = (char*)malloc(length);
	xsize = length;
	global_alloc += xsize;
	reset();
}

void emmx::reset()
{
	if (!inited || xsize == 0)
		throw MEM_OVERLOAD;
	memset(ptr, 0, xsize);
}

void emmx::free()
{
	if (!inited)
		return;
	global_alloc -= xsize;
	xsize = 0;
	delete[] ptr;
	inited = false;
}

char emmx::operator[](uint64_t off) const
{
	if (!inited)
		throw MEM_OVERLOAD;
	if (off >= xsize)
		throw MEM_OVERLOAD;
	return ptr[off];
}

bool emmx::operator==(const emmx mmx) const
{
	if (mmx.size() != xsize)
		return false;
	for (uint64_t n = 0; n < xsize; n++)
		if (mmx[n] != ptr[n])
			return false;
}

void emmx::reallow()
{
	uint64_t size = xsize;
	free();
	alloc(size);
}

uint64_t emmx::size() const
{
	return xsize;
}

API const uint64_t GetGlobalAlloc()
{
	return global_alloc;
}


void* emmx::value()
{
	return ptr;
}