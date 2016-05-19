#pragma once



#define ERR_CODE_FAILD	-10
#define ERR_CORRECT_ER	-11

API int RefPointCorrect(char *data, uint64_t len, const char *code);
API void CreateRefPoint(const char *data, uint64_t len, uint64_t &wback, char *back);
API bool TrustyTestPerFrame(uint64_t seek, uint64_t length);
API uint64_t GetLength(uint64_t len);