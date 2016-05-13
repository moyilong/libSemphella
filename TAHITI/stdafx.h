#pragma once
#include "config.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include <string>
#include <string.h>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "net_base.h"

#include "multi_platform.h"

#include "inline.h"
#include "feature_define.h"
#include "modules.h"

#define DEBUG_LINE if (true)

#include "event.h"
#include "permission.h"


#ifndef __LINUX__
#include <Windows.h>
#endif

#include <libSemphella/libSemphella.h>
#include <libSemphella/string.h>
#include <libSemphella/debug.h>