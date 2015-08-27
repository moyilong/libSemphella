#pragma once
#include "libDragonEDGE.h"
bool diff_check(IO_BUFF data);
void write_check(IO_BUFF &data);
void call_modules(IO_BUFF &io, IO_BUFF get);
vector<modules> &getpoll();