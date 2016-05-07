// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 LIBERT_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// LIBERT_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#pragma once
#include "stdafx.h"
#include "headproto.h"

#include "dll.h"

namespace LIB_ERTLIB{

	enum RETURN_STAT {
		OK,
		FILE_IO_FAILD,
		ALG_FAILD,
		FID_FAILD,
		EXT_NOT_EXIST,
		HEAD_VERIFY_FAILD,
		FILE_VERIFY_FAILD,
		PASSWORD_FAILD,
		EXT_VERFY_FAILD,
	};
#define DEFAULT_ALG_ID	6
#define DEFAULT_FHL_ID	0
	LIBERT_API RETURN_STAT crypt_to_file(string in, string out, string password, int alg=DEFAULT_ALG_ID, int fid=DEFAULT_FHL_ID, string extfil="",int bs=MAX_BUFF_SIZE);
	LIBERT_API RETURN_STAT decrtpt_to_file(string in, string out, string password,int std_mode=false);
	LIBERT_API RETURN_STAT decrypt_to_std(string in, string out, string password);
	LIBERT_API RETURN_STAT get_ext_to_file(string in, string out,bool std_mode=false);
	LIBERT_API RETURN_STAT get_ext_to_std(string in);
	LIBERT_API HEAD get_head(string in);
	LIBERT_API void PerformanceTest();

	LIBERT_API int algor_max();
	LIBERT_API int fhand_max();

	LIBERT_API void get_alg_info(int stor_id, int &id, string &help);
	LIBERT_API int get_alg_id(int tid);

	LIBERT_API string get_api_ver();

}