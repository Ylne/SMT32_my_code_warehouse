#ifndef __AT24C2_H
#define __AT24C2_H

unsigned char at24_write_byte(unsigned char addr ,unsigned char byte);
unsigned char at24_read_byte(unsigned char addr,unsigned char *recv_byte);

#endif