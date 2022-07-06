#include "i2c.h"
#include "oled.h"
#include "oled_font.h"

uint8_t CMD_Data[] = //初始化命令(数组形式)
{ 
	0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,
						
	0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,
						
	0xD8, 0x30, 0x8D, 0x14, 0xAF
};


//向OLED写命令
void OLED_WR_CMD(uint8_t CMD)
{
	HAL_I2C_Mem_Write(&hi2c1, 0X78, 0X00, I2C_MEMADD_SIZE_8BIT, &CMD, 1, 0X100);
	//IIC通信传输命令
}


//向OLED写数据
void OLED_WR_DATA(uint8_t DATA)
{
	HAL_I2C_Mem_Write(&hi2c1, 0X78, 0X40, I2C_MEMADD_SIZE_8BIT, &DATA, 1, 0X100);
	//IIC通信传输数据
}

//OLED初始化
void OLED_Init(void)
{
	uint8_t i = 0;
	
	HAL_Delay(200);
	for(i=0; i<27; i++){
		HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,CMD_Data+i,1,0x100);
		//初始化所有项目
	}
	
}

//清屏
void OLED_Clear(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_CMD(0XB0 + i);//0XB0:设置页地址(B0-B7)
		OLED_WR_CMD(0X00);//设置显示地址低4位
		OLED_WR_CMD(0X10);//设置显示地址高4位
		
		for (n = 0; n < 128; n ++)
		{
			OLED_WR_DATA(0);//n递增达到短暂持续写0操作(?)
		}
	}
}

//开启OLED显示
void OLED_Display_On(void)
{
	OLED_WR_CMD(0XBD);//电荷泵命令设置
	OLED_WR_CMD(0X14);//开启电荷泵
	OLED_WR_CMD(0XAF);//开启显示
}

//关闭OLED显示
void OLED_Display_Off(void)
{
	OLED_WR_CMD(0XBD);//电荷泵命令设置
	OLED_WR_CMD(0X10);//关闭电荷泵
	OLED_WR_CMD(0XAF);//关闭显示
}

//设置光标起始位置
//Y 以左上角为原点，向下方向的坐标 0~7
//X 以左上角为原点，向右方向的坐标 0~127
void OLED_Set_Pos(uint8_t Y, uint8_t X)
{
	OLED_WR_CMD(0XB0 | Y);//设置Y
	OLED_WR_CMD(((X & 0xF0) >> 4) | 0x10);//设置X低四位
	OLED_WR_CMD((X & 0x0F) | 0x00);//设置X高四位
}

/**
* @brief  显示一个字母
  * @param  Line 起始行坐标 1~4
  * @param 	Column 起始列坐标 1~16
  * @param	chr 显示的字母
  * @param	size 字体大小 16
  * @retval 无
  */

void OLED_ShowChar(uint8_t Line, uint8_t Column, uint8_t chr)
{
	unsigned char c = 0, i = 0;
	
	c = chr - ' ';//得到字符的ASCLL码
	
	OLED_Set_Pos((Line - 1) * 2, (Column - 1) * 8);//处理光标
	
	for (i = 0; i < 8; i++)
	{
		OLED_WR_DATA(OLED_F8x16[c][i]);//输出上半部分
	}
	
	OLED_Set_Pos((Line - 1) * 2 + 1, (Column - 1) * 8);//重新处理光标
	
	for (i = 0; i < 8; i++)
	{
		OLED_WR_DATA(OLED_F8x16[c][i + 8]);//输出下半部分
	}
	
}

/**
  * @brief  显示字符串
  * @param  Line 起始行坐标 1~4
  * @param 	Column 起始列坐标 1~16
  * @param  String 要显示的字符串
  * @retval 无
  */

void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  显示数字（十进制, 正负均可）
  * @param  Line 起始行位置 1~4
  * @param  Column 起始列位置 1~16
  * @param  Num 显示的数字
  * @param  显示数字的长度 1~10
  * @retval 无
  */

void OLED_ShowNum(uint8_t Line, uint8_t Column, int32_t Num)
{
	uint8_t i, Len, temp, num[10];
	int32_t Num2;
	
	Len = 0;
	
	if (Num > 0)
	{
		Num2 = Num;
	}
	else
	{
		Num2 = -Num;
	}
	
	for (i = 0; Num2 != 0; i++)
	{
		temp = Num2 % 10;
		Num2 = Num2 / 10;
		Len ++;
		num[i] = temp;
	}
	
	if (Num == 0)
	{
		OLED_ShowChar(Line, Column, '0');
	}
	else if (Num > 0)
	{
		for (i = 0; i < Len; i++)
		{
			OLED_ShowChar(Line, Column + i, num[Len - i - 1] + '0');
		}
	}
	else if (Num < 0)
	{
		Num = -Num;
		OLED_ShowChar(Line, Column, '-');
		
		for (i = 1; i <= Len; i++)
		{
			OLED_ShowChar(Line, Column + i, num[Len - i] + '0');
		}
	}
}


/**
  * @brief  显示汉字（自行取模，改数组名）
  * @param  Line 起始行位置 1~4
  * @param  Column 起始列位置 1~16
  * @param  Num 汉字代号
  * @retval 无
  */

void OLED_ShowChinese(uint8_t Line, uint8_t Column, uint8_t num)
{
	unsigned char i = 0;
		
	OLED_Set_Pos(Line - 1, (Column - 1) * 16);//处理光标
	
	for (i = 0; i < 16; i++)
	{
		OLED_WR_DATA(hhh[num][i]);
	}
}


//void OLED_ShowPoint(uint8_t Line, uint8_t Column)
//{
//	
//	OLED_Set_Pos(Line, Column);

//	OLED_WR_DATA(0x30);
//	
//	
//}

