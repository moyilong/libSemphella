#ifndef DEBUG_H
#define DEBUG_H

#include "libSemphella.h"
#include <time.h>

#define endl "\n"

class CAPI libDebug
{
public:
    libDebug();
    libDebug(string _name);
    libDebug(string _name,string _filename);
    ~libDebug();
    void setname(string _name);
    bool link_to_file(string filename);
    void display_log(string info);
	libDebug& operator<<(string info);
	libDebug& operator<<(int info);
	libDebug& operator<<(char* info);
private:
    string name;
    unsigned long long outcount=0;
    string buff;
    bool log_to_file=false;
    ofstream filelink;
protected:
};

API void display_dump(const char *data, long long len, int x = 5);

#endif // DEBUG_H
