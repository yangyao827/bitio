/*
 * Declaration for bitwise IO
 *
 * vim: ts=4 sw=4 cindent
 */
#ifndef __BITIO__
#define __BITIO__
 
#include <stdio.h>
 
//二进制文件结构体
typedef struct{
	FILE *fp; //输出文件指针
	unsigned char mask; //按位写入字节时的掩码
	int rack; //类似于缓存，每写完8位，将rack输出到文件中
}BITFILE;
 
BITFILE *OpenBitFileInput( char *filename);
BITFILE *OpenBitFileOutput( char *filename);
void CloseBitFileInput( BITFILE *bf);
void CloseBitFileOutput( BITFILE *bf);
int BitInput( BITFILE *bf);
unsigned long BitsInput( BITFILE *bf, int count);
void BitOutput( BITFILE *bf, int bit);
void BitsOutput( BITFILE *bf, unsigned long code, int count);
#endif	// __BITIO__
