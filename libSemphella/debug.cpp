
#include <iostream>
#include <string>
#include <fstream>
#include <omp.h>
using namespace std;
#include "debug.h"
#include "ctime"
#include "string.h"
libDebug::libDebug()
{

}

libDebug::~libDebug()
{
if (log_to_file&&filelink.is_open())
    filelink.close();
}


libDebug::libDebug(string _name)
{
setname(_name);
}

libDebug::libDebug(string _name, string _filename)
{
    setname(_name);
    link_to_file(_filename);
}

void libDebug::setname(string _name)
{
    name=_name;
}

bool libDebug::link_to_file(string filename)
{
    if (filelink.is_open())
        return false;
    filelink.open(filename.data());
    if (!filelink.is_open())
        return false;
    display_log("Init Debug Library Log File");
    return true;
}

void libDebug::display_log(string info)
{
       buff+=info;
       if (strfind(buff.data(),'\n')>0)
       {
           string stime="[";
           stime+=name;
           stime+="]"+ buff;
          
		   cout << stime;
		   if (log_to_file)
			   filelink << stime;
		   buff = "";
       }
}

libDebug& libDebug::operator<<(string info)
{
	display_log(info);
	return *this;
}

libDebug & libDebug::operator<<(int info)
{
	char buff[MAX_BUFF_SIZE] = { 0 };
//	_itoa(info, buff, 10);
	sprintf(buff,"%d",info);
	*this << buff;
	return *this;
}

libDebug & libDebug::operator<<(char * info)
{
	display_log(info);
	return *this;
}

