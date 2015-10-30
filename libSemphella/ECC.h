
#define DEFAULT_ECC_SIZE	256
/**
* caculate_ecc - [NAND Interface] Calculate 3-byte ECC for 256/512-byte
*			 block
* @buf:	input buffer with raw data
* @eccsize:	data bytes per ECC step (256 or 512)
* @code:	output buffer with ECC
*/
API void caculate_ecc(const unsigned char *buf, unsigned char *code, unsigned int eccsize=512);

/**
* correct_data - [NAND Interface] Detect and correct bit error(s)
* @buf:	raw data read from the chip
* @read_ecc:	ECC from the chip
* @calc_ecc:	the ECC calculated from raw data
* @eccsize:	data bytes per ECC step (256 or 512)
*
* Detect and correct a 1 bit error for eccsize byte block
*/
API int correct_data(unsigned char *buf,
	unsigned char *read_ecc, unsigned char *calc_ecc,
	unsigned int eccsize = 512);

inline int get_eccdata_size(int len)
{
	return 8 + len / 16;
}