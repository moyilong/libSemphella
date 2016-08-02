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

#include "event.h"
#include "permission.h"

#ifndef __LINUX__
#include <Windows.h>
#endif
#include <libSemphella/main.h>
#include <libSemphella/libSemphella.h>
#include <libSemphella/string.h>
#include <libSemphella/debug.h>