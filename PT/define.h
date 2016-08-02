#pragma once
#include "stdafx.h"
#include "storage.h"
#define AREA_MAX	1024
#define STEPS		16
#define STEP_DIVID	1
#define DGST_STEP	4
#define LOOP_ADD	4
extern char **chess;
extern uint64_t cc_count;
extern uint64_t xc_count;
#define for_each	 for (int x=0;x<AREA_MAX;x++) for (int y=0;y<AREA_MAX;y++)

extern int iops;

bool inPoint(XPOINT point);
void init();
void Run();

#define MT7620_PERFORMANCE	7.5

extern uint64_t tout;
void Run2();