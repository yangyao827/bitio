/*
 * Definitions for bitwise IO
 *
 * vim: ts=4 sw=4 cindent
 */
 
#include <stdlib.h>
#include <stdio.h>
#include "bitio.h"
 
//打开要输入的文件
BITFILE *OpenBitFileInput( char *filename){
	BITFILE *bf;
	bf = (BITFILE *)malloc( sizeof(BITFILE));
	if(NULL == bf) 
		return NULL;
	if(NULL == filename)	
		bf->fp = stdin;
	else 
		bf->fp = fopen( filename, "rb");
	if( NULL == bf->fp) 
		return NULL;
	bf->mask = 0x80;
	bf->rack = 0;
	return bf;
}
 
//以二进制方式打开要输出的文件
BITFILE *OpenBitFileOutput( char *filename){
	BITFILE *bf;
	bf = (BITFILE *)malloc( sizeof(BITFILE));
	if(NULL == bf) 
		return NULL;
	if( NULL == filename)	
		bf->fp = stdout;
	else 
		bf->fp = fopen( filename, "wb"); //以二进制只写的方式打开文件
	if(NULL == bf->fp) 
		return NULL;
	bf->mask = 0x80; //初始化掩码为1000 0000
	bf->rack = 0;  //初始化rack为0
	return bf;
}
 
//关闭读入文件比特流
void CloseBitFileInput( BITFILE *bf){
	fclose(bf->fp);
	free(bf);
}
 
//输出剩下未输出的二进制数字，关闭二进制输出文件
void CloseBitFileOutput( BITFILE *bf){
	// Output the remaining bits
	if( 0x80 != bf->mask) 
		fputc( bf->rack, bf->fp);
	fclose(bf->fp);
	free(bf);
}
 
//得到输出文件中下一位二进制数（从右向左）
int BitInput( BITFILE *bf){
	int value;
 
	if( 0x80 == bf->mask){
		bf->rack = fgetc( bf->fp);
		if( EOF == bf->rack){
			fprintf(stderr, "Read after the end of file reached\n");
			exit( -1);
		}
	}
	value = bf->mask & bf->rack;
	bf->mask >>= 1;
	if( 0 == bf->mask) 
		bf->mask = 0x80;
	return( (0==value)?0:1);
}
 
//从输入文件中得到数据
unsigned long BitsInput( BITFILE *bf, int count){
	unsigned long mask;
	unsigned long value;
	mask = 1L << (count-1);
	value = 0L;
	while( 0!=mask){
		if( 1 == BitInput( bf))
			value |= mask;
		mask >>= 1;
	}
	return value;
}
 
 
void BitOutput( BITFILE *bf, int bit){
	//如果bit为1，则在rack下一个未写码位置由0变为1。如果bit为0，则rack不作处理，mask直接向右移位，代表下一个未写码位置为0
	if( 0 != bit) 
		bf->rack |= bf->mask;
	bf->mask >>= 1;
	//每次mask移位后，都要判断mask是否溢出为0。若溢出，则代表成功累计写入八位，即该字节已写满。
	//直接输出rack后，rack初始化为0，mask初始化为1000 0000。
	if( 0 == bf->mask){	// eight bits in rack
		fputc( bf->rack, bf->fp); 
		bf->rack = 0;
		bf->mask = 0x80;
	}
}
 
//按位输出数据到输出文件中
void BitsOutput( BITFILE *bf, unsigned long code, int count){
	unsigned long mask;
 
	mask = 1L << (count-1);
	while( 0 != mask){  //mask为0时，说明code共count位数字输出完毕
		BitOutput( bf, (int)(0==(code&mask)?0:1)); //按位输出code
		mask >>= 1; //掩码向右移位
	}
}
#if 0
#endif
