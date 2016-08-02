#include "../stdafx.h"

#ifdef __LINUX__
#define TRANS_LATE_DIR	"/etc/TAHITI/translate"
#else
#define TRANS_LATE_DIR	"./translate"
#endif

string translate(string origin);
void dump_bin(string filename);
void load_bin(string filename);
void load_asci(string filename);