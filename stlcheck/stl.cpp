#include <libSemphella/main.h>
#include <libSemphella/stl.h>
#include <libSemphella/argment.h>
#include <libSemphella/math.h>
argment args;
string filename;
string cvs;
bool end_pause = false;
string stl_file;
void proc(string arg, string val)
{
	switch (arg[0])
	{
	case 'f':
		filename = val;
		break;
	case 'c':
		cvs = val;
		break;
	case 'p':
		end_pause = true;
		break;
	case 's':
		stl_file = val;
		break;
	case 't':
		ShortXTest(16);
		exit(0);
		break;
	case 'r':
		random_test();
		break;
	}
}
#include <libSemphella/files.h>
int main(int argc, char *argv[])
{
	args.load(argc, argv);
	args.for_each(proc);
	stl temp;
	temp.ReadSTLFile(filename.data());
	ofstream io;
	if (!cvs.empty())
		io.open(cvs, ios::binary);
	for (uint64_t n = 0; n < temp.TrianglesSize(); n++)
	{
		//printf("Triangles:%f %f %f [%f %f %f] [%f %f %f]")
		if (!cvs.empty())
			io<<temp.at(n).ToString("%s,%s,%s", "%f,%f,%f") << "\n";
		else
			cout << "Triangles:" << temp.at(n).ToString() << endl;
	}
	cout << "Triangles Size:" << temp.TrianglesSize() << endl;
	io.close();
	debug << "Sizeof Float=" << sizeof(float) << endl;
	debug << "Sizoef Unit:" << sizeof(Triangles) << endl;
	debug << "Sizeof Vector:" << sizeof(vector_t) << endl;
	if (!stl_file.empty())
	{
		debug << "Exporting STL" << endl;
		ofstream fo;
		fo.open(stl_file, ios::binary);
		uint8_t head[80];
		temp.GetHead(head);
		fo.write((char*)head, 80);
		uint32_t size = temp.size();
		fo.write((char*)&size, sizeof(uint32_t));
		for (uint64_t n = 0; n < size; n++)
		{
			fo.write((char*)&temp.at(n), 48);
			fo.write(temp.at(n).attr, 2);
		}
		
		

	}
	if (end_pause)
		esleep(30000);
}