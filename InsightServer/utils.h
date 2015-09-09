#pragma once
#include "feature.h"
void init();
CONN_HEAD dePackage(char *buff,bool &stat);
void Package(const CONN_HEAD head, char *buff);

void server_main();

void client_main();