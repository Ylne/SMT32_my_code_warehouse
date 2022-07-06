#ifndef __OLED_H
#define __OLED_H

#include "main.h"

void OLED_Init(void);
void OLED_Clear(void);
void OLED_Set_Pos(uint8_t Y, uint8_t X);
void OLED_ShowChar(uint8_t y, uint8_t x, uint8_t chr);
void OLED_ShowString(uint8_t y, uint8_t x, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, int32_t Num);
void OLED_ShowChinese(uint8_t Line, uint8_t Column, uint8_t num);
//void OLED_ShowPoint(uint8_t Line, uint8_t Column);

#endif
