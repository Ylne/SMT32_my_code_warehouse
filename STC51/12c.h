#ifndef _12C_H
#define _12C_H

unsigned char i2c_men_read(unsigned char dev_addr,
														unsigned char reg_addr,
															unsigned char bytes[],
																unsigned char count);
unsigned char i2c_men_write(unsigned char dev_addr, unsigned char reg_addr,
														unsigned char bytes[],unsigned char count());

#endif 