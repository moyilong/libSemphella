#pragma once
#include "main.h"
#include "string.h"
#include "debug.h"
#include "apd.h"
#define VECTOR_TYPE	float
struct vector_t {
	VECTOR_TYPE x, y, z;
	inline void SetParam(VECTOR_TYPE x_t, VECTOR_TYPE y_t, VECTOR_TYPE z_t)
	{
		x = x_t;
		y = y_t;
		z = z_t;
	}
	inline void clear()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	inline string ToString(string formal = "%f %f %f")
	{
		char buff[MAX_BUFF_SIZE] = { 0x00 };
		sprintf(buff, formal.data(), x, y, z);
		return buff;
	}
};
struct Triangles {
	vector_t normal;
	vector_t a, b, c;
	//uint16_t attr;
	char attr[2];
	inline Triangles()
	{
		clear();
	}
	inline void clear()
	{
		a.clear();
		b.clear();
		c.clear();
		//attr = 0;
	}
	inline string ToString(string formal = "%s [%s][%s][%s]", string subformal = "%f %f %f")
	{
		//string ret = "["normal.ToString() + "][" + a.ToString() + "][" + b.ToString() + "][" + c.ToString + "]";
		char buff[MAX_BUFF_SIZE] = { 0x00 };
		sprintf(buff, formal.data(), normal.ToString(subformal).data(), a.ToString(subformal).data(), b.ToString(subformal).data(), c.ToString(subformal).data());
		return buff;
	}
};
class API stl
{
public:

	inline stl() {};
	stl(string filename);
	uint64_t VectorSize() const;
	uint64_t TrianglesSize() const;
	bool ReadSTLFile(const string cfilename);
	void SetHeaders(uint8_t tdata[80]);
	Triangles operator=(uint64_t val);
	inline Triangles at(uint64_t val)
	{
		return operator=(val);
	}
	inline void GetHead(uint8_t xhead[])const
	{
		memcpy(xhead, head, 80);
	}
	inline uint32_t size()
	{
		return data.size();
	}
	bool WriteFile(const string filename);
private:
#define loadfile ReadSTLFile
	uint8_t head[80];
	vector<Triangles> data;
	bool ReadASCII(const string cfilename);
	bool ReadBinary(const string cfilename);
	vector_t StringToVectorT(string data);
};
