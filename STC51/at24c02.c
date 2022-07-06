#include "at24c02.h"

#include "12c.h"

#define DEV_ADDR 0xA0

unsigned char at24_write_byte(unsigned char addr,unsigned char byte)
{
	unsigned char bytes_to_send[1];
	unsigned char try_count=10;
	
	bytes_to_send[0] = byte;
	
	while (i2c_men_write(DEV_ADDR,addr,bytes_to_send,1) && try_count){	
		try_count--;
	}
	if (try_count > 0){
	 return 0;	//sucess
	}else{
		return 1;
	}
}

unsigned char at24_read_byte(unsigned char addr,unsigned char *recv_byte)
{
	
	unsigned char try_count=10; 
	
	while (i2c_men_read(DEV_ADDR,addr,recv_byte, 1) && try_count){	
		try_count--;
	}
	if (try_count > 0){
	 return 0;//success
	}else{
		return 1;//fail
	}
}