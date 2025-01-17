#ifndef __WAVEFORM_H
#define __WAVEFORM_H	

#include "lcd.h"

#define tableSize 1024

void sin_Generation(void);
void sawtooth_Generation(void);
void triangle_Generation(void);
void rectangle_Generation(void);
void dtmf_Generation(u16 num);
void MYDAC_Init(void);
void clear_point(u16 num);                                                                                                                       //更新显示屏当前列
void Set_BackGround(void);                                                                                                                       //设置背景
void Lcd_DrawNetwork(void);                                                                                                                      //画网格
float get_vpp(u16 *buf);                                                                                                                         //获取峰峰值
void DrawOscillogram(u16 *buff, u16 len, signed int offset1, signed int offset2); //画波形图
#endif
