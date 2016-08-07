#include "stl.h"
#include "files.h"
stl::stl(string filename)
{
	loadfile(filename);
}
uint64_t stl::VectorSize() const
{
	return data.size() * 4;	
}
uint64_t stl::TrianglesSize() const
{
	return data.size();
}
bool stl::WriteFile(const string filename)
{
	file io;
	io.open(filename, "w");
	if (!io.is_open())
		return false;
	uint32_t xsize = size();
	io.write((char*)head, sizeof(head));
	io.write((char*)&xsize, sizeof(uint32_t));
	for (uint64_t n = 0; n < xsize; n++)
	{
		io.write((char*)&at(n), 48);
		io.write(at(n).attr, 2);
	}
}
bool stl::ReadASCII(const string cfilename)
{
	file io;
	io.open(cfilename, "r");
	if (!io.is_open())
		return false;
	string head_info = io.getline();
	strcpy((char*)head, StrLimit(Splite(head_info, ' ', true)[1],80).data());
	debug << "STL Get Filename:" << head << endl;
	Triangles temp;
	while (!io.is_eof())
	{
		string line = io.getline();
		//string normal_v = io.getline();
		vector<string> normal_v = Splite(io.getline(), ' ', true);
		temp.normal.x = atolf(normal_v[3].data());
		temp.normal.y = atolf(normal_v[3].data());
		temp.normal.z = atolf(normal_v[3].data());
		io.getline();
		temp.a = StringToVectorT(io.getline());
		temp.b = StringToVectorT(io.getline());
		temp.c = StringToVectorT(io.getline());
		data.push_back(temp);
		data.clear();
	}
	return true;
}

bool stl::ReadBinary(const string cfilename)
{
	ifstream io;
	io.open(cfilename, ios::binary);
	if (!io.is_open())
		return false;
	uint32_t size;
	io.read((char*)head, sizeof(uint8_t) * 80);
	io.read((char*)&size, sizeof(size));
	debug << "Get Size Info:" << size << endl;
	display_dump((char*)&size,sizeof(size));
	for (uint32_t n = 0; n < size; n++)
	{
		if (n % 10 == 0)
			debug << "Alingine Szie:" << n << "//" << size << endl;
		Triangles temp;
		/*VECTOR_TYPE type[12];
		io.read((char*)type, sizeof(type));
		temp.normal.SetParam(type[0], type[1], type[2]);
		temp.a.SetParam(type[3], type[4], type[5]);
		temp.b.SetParam(type[6], type[7], type[8]);
		temp.c.SetParam(type[9], type[10], type[11]);
		io.read((char*)&temp.attr, sizeof(temp.attr));*/
		io.read((char*)&temp, 48);
		io.read(temp.attr, 2);
		data.push_back(temp);

	}
	return true;
}
bool stl::ReadSTLFile(const string cfilename)
{
	if (cfilename.empty())
		return false;

	ifstream in(cfilename, ios::in);
	if (!in)
		return false;
	char buff[80];
	in.read(buff, 80);

	if (strfind(buff,'\n')!=0)
		ReadASCII(cfilename);
	else
		ReadBinary(cfilename);

	return true;
}

void stl::SetHeaders(uint8_t tdata[80])
{
	memcpy(head, tdata,80*sizeof(uint8_t));
}

Triangles stl::operator=(uint64_t val)
{
	if (val < size())
		return data[val];
	return Triangles();
}

vector_t stl::StringToVectorT(string data)
{
	vector_t ret;
	vector<string> va = Splite(data, ' ', true);
	ret.x = atolf(va[0].data());
	ret.y = atolf(va[1].data());
	ret.z = atolf(va[2].data());
	return ret;
}
