#include "12c.h"

#include <STC89C5xRC.H>
#include <intrins.h>

#define SDA P20
#define SCL P21

#define ACK 0 
#define NACK 1 

void delay()
{
	_nop_();
}

void start()
{
	SDA = 1;
	SCL = 1;	
	delay();
	SDA = 0;
	delay();
	SCL = 0;
}
void stop()
{
	SDA = 0;
	delay();
	SCL = 1;
	delay();
	SDA = 1;
}

void send_byte(unsigned char byte)
{
	unsigned char i = 0;
	for (i = 0;i < 8;i++)
	{
		SDA = (byte >> (7 - i)) & 0x01;
		delay();
		SCL = 1;
		delay();
		SCL = 0;
		delay();
	}
}

unsigned char read_byte()
{
	unsigned char i = 0;
	unsigned char byte = 0x00;
	
	SDA = 1;
	for (i = 0; i<8 ; i++)
	{
		SCL = 1;
		delay();
		byte = byte << 1;
		byte |= SDA;
		SCL=0;
		delay();
		}
	return byte;
}

unsigned char read_ack()
{
	unsigned char is_ack;
	SDA = 1;
	delay();
	
	SCL = 1;
	delay();
	is_ack = SDA;
	SCL = 0;
	return is_ack;
}

void send_ack(unsigned char ack)
{
	SDA = ack;
	delay();
	SCL = 1;
	delay();
	SDA = 0;
}

unsigned char i2c_men_write(unsigned char dev_addr, unsigned char reg_addr,
														unsigned char bytes[],unsigned char count)
{
	unsigned char i = 0;
	// start condition 
	start();
	//device address
	send_byte(dev_addr); // 7-bit + R/W
	// read ACK
	if(read_ack() == NACK)
	{
		stop();
		return 1;
	}
	send_byte(reg_addr);
	read_ack();
	//send data
	for (i = 0; i < count ;i++)
	{
		send_byte(bytes[i]);
		read_ack();
	}
	//Stop condition
	stop();
	return 0;
}

unsigned char i2c_men_read(unsigned char dev_addr,
														unsigned char reg_addr,
															unsigned char bytes[],
																unsigned char count)
{
		unsigned char i = 0;
	//start condition
	start();
	//send device address +R/W
	send_byte(dev_addr | 0x01);
	// Read ACK
	if (read_ack() == NACK)
	{
		stop();
		return 1;
	}
	//send register address
	send_byte(reg_addr);
	read_ack();
	//Rrpeated start
	start();
	// send device adress + R
	send_byte(dev_addr | 0x01);
	read_ack();
	// Read bytes
	for(i = 0;i<count-1	 ;i++)
	{
		bytes[i] = read_byte();
		send_ack(ACK);
	}
	bytes[count - 1] = read_byte();
		send_ack(NACK);
	//Stop condition
	stop();
	return 0;
}