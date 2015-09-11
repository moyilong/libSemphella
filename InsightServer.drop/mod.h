#pragma once
#include <libSemphella/libSemphella.h>
#include <libSemphella/debug.h>
#include "feature.h"
typedef bool(*modules_api)(const CONN_HEAD,CONN_HEAD&);

class mod{
public:
	mod(long long call_type, modules_api api);
};

modules_api get_modapi(long long reg_type);