#include "key.h"
#include "stm32f4xx_gpio.h"
#include "delay.h" 
#include "lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//按键输入驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//按键初始化函数
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE); //使能GPIOA,GPIOE时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4; //KEY0 KEY1 KEY2对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;						//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;						//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);								//初始化GPIOE2,3,4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	   //WK_UP对应引脚PA0
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);		   //初始化GPIOA0
	/****4行输出****/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	/**** 4列输入 ****/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，WKUP按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志u
	//u8 runstop=1;
	
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return 0;
		else if(KEY1==0)return 2;
		else if(KEY2==0)return 3;
		else if(WK_UP==1)return 4;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)key_up=1; 	    
 	return 1;// 无按键按下
}

/************************************
按键表盘为:  1  2  3  a
             4  5  6  b
			 7  8  9  c 
			 *  0  #  d 
// ************************************/
int MartrixKeyScan(void)
{
	GPIO_WriteBit(GPIOD, GPIO_Pin_6, Bit_SET);
	GPIO_WriteBit(GPIOG, GPIO_Pin_9, Bit_SET);
	GPIO_WriteBit(GPIOG, GPIO_Pin_11, Bit_SET);
	GPIO_WriteBit(GPIOG, GPIO_Pin_13, Bit_SET);
	if ((LINE0 | LINE1 | LINE2 | LINE3) == 0) //如果PG4、PB4、PB6、PB8全为零则没有按键按下
	{
		return 20;
	}
	GPIO_WriteBit(GPIOD, GPIO_Pin_6, Bit_SET);
	GPIO_WriteBit(GPIOG, GPIO_Pin_9, Bit_RESET);
	GPIO_WriteBit(GPIOG, GPIO_Pin_11, Bit_RESET);
	GPIO_WriteBit(GPIOG, GPIO_Pin_13, Bit_RESET);
	if (LINE0)
	{
		return 1;
	}
	else if (LINE1)
	{
		return 2;
	}
	else if (LINE2)
	{
		return 3;
	}
	else if (LINE3)
	{
		return 10;
	}

	while ((LINE0 | LINE1 | LINE2 | LINE3) > 0) //等待按键释放，同理，不赘述
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_6, Bit_RESET);
		GPIO_WriteBit(GPIOG, GPIO_Pin_9, Bit_RESET);
		GPIO_WriteBit(GPIOG, GPIO_Pin_11, Bit_RESET);
		GPIO_WriteBit(GPIOG, GPIO_Pin_13, Bit_RESET);
	};

	GPIO_WriteBit(GPIOG, GPIO_Pin_9, Bit_SET);
	if (LINE0)
	{
		return 4;
	}
	else if (LINE1)
	{
		return 5;
	}
	else if (LINE2)
	{
		return 6;
	}
	else if (LINE3)
	{
		return 11;
	}

	while ((LINE0 | LINE1 | LINE2 | LINE3) > 0)					//等待按键释放，同理，不赘述
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_6, Bit_RESET);
		GPIO_WriteBit(GPIOG, GPIO_Pin_9, Bit_RESET);
		GPIO_WriteBit(GPIOG, GPIO_Pin_11, Bit_RESET);
		GPIO_WriteBit(GPIOG, GPIO_Pin_13, Bit_RESET);
	};								//重新让PC0到PC3全部输出低。

	GPIO_WriteBit(GPIOG, GPIO_Pin_11, Bit_SET);
	if (LINE0)
	{
		return 7;
	}
	else if (LINE1)
	{
		return 8;
	}
	else if (LINE2)
	{
		return 9;
	}
	else if (LINE3)
	{
		return 12;
	}

	while ((LINE0 | LINE1 | LINE2 | LINE3) > 0) //等待按键释放，同理，不赘述
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_6, Bit_RESET);
		GPIO_WriteBit(GPIOG, GPIO_Pin_9, Bit_RESET);
		GPIO_WriteBit(GPIOG, GPIO_Pin_11, Bit_RESET);
		GPIO_WriteBit(GPIOG, GPIO_Pin_13, Bit_RESET);
	}; //重新让PC0到PC3全部输出低。

	GPIO_WriteBit(GPIOG, GPIO_Pin_13, Bit_SET);
	if (LINE0)
	{
		return 13;
	}
	else if (LINE1)
	{
		return 14;
	}
	else if (LINE2)
	{
		return 15;
	}
	else if (LINE3)
	{
		return 16;
	}
	while ((LINE0 | LINE1 | LINE2 | LINE3) > 0) //等待按键释放，同理，不赘述
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_6, Bit_RESET);
		GPIO_WriteBit(GPIOG, GPIO_Pin_9, Bit_RESET);
		GPIO_WriteBit(GPIOG, GPIO_Pin_11, Bit_RESET);
		GPIO_WriteBit(GPIOG, GPIO_Pin_13, Bit_RESET);
	}; //重新让PC0到PC3全部输出低。
	return 0;
}
