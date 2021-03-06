#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sys.h"
#include "delay.h"

#include "usart.h"
#include "hc05.h" 	 
#include "usart3.h" 
#include "bcomponent.h"
#include "exti.h"

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		// 设置系统中断优先级分组2
	delay_init(168);				// 延时初始化 
	
	uart_init(9600);				// 初始化串口
	BCOMPONENT_Init();				// 初始化自定义引脚
	EXTIX_Init();					// 外部中断初始化
	
	u8 contrastState = 0;			// 对比度状态位	1-往左转 2-往右转
	u8 lightSource = 0;				// 光源状态位 	1-往左转 2-往右转
	
	while ( HC05_Init() ) 			// 初始化HC05蓝牙模块
	{
		delay_ms(100);
		printf("蓝牙初始化中...\r\n");
	}
	
	/*
	 M1:
	 GPIO_SetBits(GPIOB, GPIO_Pin_0);		// PB0 = 1
	 GPIO_ResetBits(GPIOE, GPIO_Pin_8);		// PE8 = 0
	 GPIO_SetBits(GPIOE, GPIO_Pin_9);		// PE9 = 1

	 GPIO_SetBits(GPIOB, GPIO_Pin_0);		// PB0 = 1
	 GPIO_SetBits(GPIOE, GPIO_Pin_8);		// PE8 = 1
	 GPIO_ResetBits(GPIOE, GPIO_Pin_9);		// PE9 = 0
		
	 GPIO_ResetBits(GPIOB, GPIO_Pin_0);		// PB0 = 0
	
	
	 M2:
	 GPIO_SetBits(GPIOE, GPIO_Pin_7);		// PE7 = 1
	 GPIO_ResetBits(GPIOE, GPIO_Pin_10);	// PE10 = 0
	 GPIO_SetBits(GPIOE, GPIO_Pin_11);		// PE11 = 1
	 
	 GPIO_SetBits(GPIOE, GPIO_Pin_7);		// PE7 = 1
	 GPIO_SetBits(GPIOE, GPIO_Pin_10);		// PE10 = 1
	 GPIO_ResetBits(GPIOE, GPIO_Pin_11);	// PE11 = 0
	 
	 GPIO_ResetBits(GPIOE, GPIO_Pin_7);		// PE7 = 0
	 
	 
	 M3:
	 GPIO_SetBits(GPIOE, GPIO_Pin_14);		// PE14 = 1
	 GPIO_ResetBits(GPIOE, GPIO_Pin_13);	// PE13 = 0
	 GPIO_SetBits(GPIOE, GPIO_Pin_12);		// PE12 = 1

	 GPIO_SetBits(GPIOE, GPIO_Pin_14);		// PE14 = 1
	 GPIO_SetBits(GPIOE, GPIO_Pin_13);		// PE13 = 1
	 GPIO_ResetBits(GPIOE, GPIO_Pin_12);	// PE12 = 0
	 
	 GPIO_ResetBits(GPIOE, GPIO_Pin_14);	// PE14 = 0
	 
	 
	 M4:
	 GPIO_SetBits(GPIOD, GPIO_Pin_8);		// PD8 = 1
	 GPIO_ResetBits(GPIOA, GPIO_Pin_7);		// PA7 = 0
	 GPIO_SetBits(GPIOC, GPIO_Pin_6);		// PC6 = 1
	 
	 GPIO_SetBits(GPIOD, GPIO_Pin_8);		// PD8 = 1
	 GPIO_SetBits(GPIOA, GPIO_Pin_7);		// PA7 = 1
	 GPIO_ResetBits(GPIOC, GPIO_Pin_6);		// PC6 = 0
		
	 GPIO_ResetBits(GPIOD, GPIO_Pin_8);		// PD8 = 0
	 
	 
	 M5:
	 GPIO_SetBits(GPIOG, GPIO_Pin_6);		// PG6 = 1
	 GPIO_ResetBits(GPIOB, GPIO_Pin_14);	// PB14 = 0
	 GPIO_SetBits(GPIOC, GPIO_Pin_7);		// PC7 = 1
	 
	 GPIO_SetBits(GPIOG, GPIO_Pin_6);		// PG6 = 1
	 GPIO_SetBits(GPIOB, GPIO_Pin_14);		// PB14 = 1
	 GPIO_ResetBits(GPIOC, GPIO_Pin_7);		// PC7 = 0
	 
	 GPIO_ResetBits(GPIOG, GPIO_Pin_6);		// PG6 = 0
	 
	 
	 M6:
	 GPIO_SetBits(GPIOC, GPIO_Pin_8);		// PC8 = 1
	 GPIO_ResetBits(GPIOC, GPIO_Pin_8);		// PC8 = 0
	 
	 
	 蓝牙输出
	 USART3_RX_BUF[len] = 0;									// 加结束符
	 printf("蓝牙发送的消息为: \r%s\r\n\r\n", USART3_RX_BUF);	// 发送数据到上位机
	 
	 
	 串口输出
	 printf("您发送的消息为: \r");
	 for ( u8 t = 0; t < len; ++t )
	 {
		 USART_SendData(USART1, USART_RX_BUF[t]);         		// 向串口1发送数据
		 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);	// 等待发送结束
	 }
	 printf("\r\n\r\n");	// 插入换行
	 */
	
	while(1)
	{
		// 消抖
		delay_ms(10);
		
		// 调焦判断限位
		if ( LIMIT0 != 0 || LIMIT1 != 0 ) {
			GPIO_ResetBits(GPIOB, GPIO_Pin_0);	// PB0 = 0
		}
		
		// 变倍判断限位
		if ( LIMIT2 != 0 || LIMIT3 != 0 ) {
			GPIO_ResetBits(GPIOE, GPIO_Pin_7);	// PE7 = 0
		}
		
		// 对比度判断左限位
		if ( LIMIT4 != 0 && contrastState == 1 ) {
			GPIO_ResetBits(GPIOD, GPIO_Pin_8);	// PD8 = 0
		}
		
		// 对比度判断右限位
		if ( LIMIT5 != 0 && contrastState == 2 ) {
			GPIO_ResetBits(GPIOD, GPIO_Pin_8);	// PD8 = 0
		}
		
		// 双光源判断左限位
		if ( LIMIT6 != 0 && lightSource == 1 ) {
			GPIO_ResetBits(GPIOG, GPIO_Pin_6);	// PG6 = 0
		}
		
		// 双光源判断右限位
		if ( LIMIT7 != 0 && lightSource == 2 ) {
			GPIO_ResetBits(GPIOG, GPIO_Pin_6);	// PG6 = 0
		}
		

		// 蓝牙
		if( USART3_RX_STA & 0X8000 )
		{
			u8 len = USART3_RX_STA & 0X7FFF;
			
			// 调焦左旋钮 - 按下
			if ( USART3_RX_BUF[0] == 'a' ) {
				if ( LIMIT0 != 0 ) {
					u3_printf("FL\r\n");					// 调焦左限位已触发
				} else {
					GPIO_SetBits(GPIOB, GPIO_Pin_0);		// PB0 = 1
					GPIO_ResetBits(GPIOE, GPIO_Pin_8);		// PE8 = 0
					GPIO_SetBits(GPIOE, GPIO_Pin_9);		// PE9 = 1
					
					if ( LIMIT1 != 0 ) {
						// 右限位触发的情况下，先转一会确保离开限位
						delay_ms(500);
					}
				}
			}
			
			// 调焦左旋钮 - 抬起
			if ( USART3_RX_BUF[0] == 'b' ) {
				GPIO_ResetBits(GPIOB, GPIO_Pin_0);			// PB0 = 0
			}
				
			// 调焦右旋钮 - 按下
			if ( USART3_RX_BUF[0] == 'c' ) {
				if ( LIMIT1 != 0 ) {
					u3_printf("FR\r\n");					// 调焦右限位已触发
				} else {
					GPIO_SetBits(GPIOB, GPIO_Pin_0);		// PB0 = 1
					GPIO_SetBits(GPIOE, GPIO_Pin_8);		// PE8 = 1
					GPIO_ResetBits(GPIOE, GPIO_Pin_9);		// PE9 = 0
					
					if ( LIMIT0 != 0 ) {
						// 左限位触发的情况下，先转一会确保离开限位
						delay_ms(500);
					}
				}
			}
			
			// 调焦右旋钮 - 抬起
			if ( USART3_RX_BUF[0] == 'd' ) {
				GPIO_ResetBits(GPIOB, GPIO_Pin_0);			// PB0 = 0
			}
			
			//------------------------------------------------------------------------------
			
			// 变倍左旋钮 - 按下
			if ( USART3_RX_BUF[0] == 'e' ) {
				if ( LIMIT2 != 0 ) {
					u3_printf("ZL\r\n");					// 变倍左限位已触发
				} else {
					/*
					// 点动模式
					GPIO_ResetBits(GPIOE, GPIO_Pin_10);		// PE10 = 0
					GPIO_SetBits(GPIOE, GPIO_Pin_11);		// PE11 = 1
					
					int count = 1;
					while( LIMIT2 == 0 )
					{
						if ( count == 20 ) {
							break;
						}
						
						GPIO_SetBits(GPIOE, GPIO_Pin_7);	// PE7 = 1
						delay_ms(2);
						GPIO_ResetBits(GPIOE, GPIO_Pin_7);	// PE7 = 0
						delay_ms(5);
						
						count++;
					}
					*/
					
					GPIO_SetBits(GPIOE, GPIO_Pin_7);		// PE7 = 1
					GPIO_ResetBits(GPIOE, GPIO_Pin_10);		// PE10 = 0
					GPIO_SetBits(GPIOE, GPIO_Pin_11);		// PE11 = 1
					
					if ( LIMIT3 != 0 ) {
						// 右限位触发的情况下，先转一会确保离开限位
						delay_ms(100);
					}
				}
			}
			
			// 变倍左旋钮 - 抬起
			if ( USART3_RX_BUF[0] == 'f' ) {
				GPIO_ResetBits(GPIOE, GPIO_Pin_7);			// PE7 = 0
			}
				
			//  变倍右旋钮 - 按下
			if ( USART3_RX_BUF[0] == 'g' ) {
				if ( LIMIT3 != 0 ) {
					u3_printf("ZR\r\n");					// 变倍右限位已触发
				} else {
					/*
					// 点动模式
					GPIO_SetBits(GPIOE, GPIO_Pin_10);		// PE10 = 1
					GPIO_ResetBits(GPIOE, GPIO_Pin_11);		// PE11 = 0
					
					int count = 1;
					while( LIMIT3 == 0 )
					{
						if ( count == 20 ) {
							break;
						}
						
						GPIO_SetBits(GPIOE, GPIO_Pin_7);	// PE7 = 1
						delay_ms(2);
						GPIO_ResetBits(GPIOE, GPIO_Pin_7);	// PE7 = 0
						delay_ms(5);
						
						count++;
					}
					*/
					
					GPIO_SetBits(GPIOE, GPIO_Pin_7);		// PE7 = 1
					GPIO_SetBits(GPIOE, GPIO_Pin_10);		// PE10 = 1
					GPIO_ResetBits(GPIOE, GPIO_Pin_11);		// PE11 = 0
					
					if ( LIMIT2 != 0 ) {
						// 左限位触发的情况下，先转一会确保离开限位
						delay_ms(100);
					}
				}
			}
			
			//  变倍右旋钮 - 抬起
			if ( USART3_RX_BUF[0] == 'h' ) {
				GPIO_ResetBits(GPIOE, GPIO_Pin_7);			// PE7 = 0
			}

			//------------------------------------------------------------------------------
			
			// 明亮度左旋钮 - 按下
			if ( USART3_RX_BUF[0] == 'i' ) {
				GPIO_SetBits(GPIOE, GPIO_Pin_14);			// PE14 = 1
				GPIO_ResetBits(GPIOE, GPIO_Pin_13);			// PE13 = 0
				GPIO_SetBits(GPIOE, GPIO_Pin_12);			// PE12 = 1
			}
			
			// 明亮度左旋钮 - 抬起
			if ( USART3_RX_BUF[0] == 'j' ) {
				GPIO_ResetBits(GPIOE, GPIO_Pin_14);			// PE14 = 0
			}
			
			// 明亮度右旋钮 - 按下
			if ( USART3_RX_BUF[0] == 'k' ) {
				GPIO_SetBits(GPIOE, GPIO_Pin_14);			// PE14 = 1
				GPIO_SetBits(GPIOE, GPIO_Pin_13);			// PE13 = 1
				GPIO_ResetBits(GPIOE, GPIO_Pin_12);			// PE12 = 0
			}
			
			// 明亮度右旋钮 - 抬起
			if ( USART3_RX_BUF[0] == 'l' ) {
				GPIO_ResetBits(GPIOE, GPIO_Pin_14);			// PE14 = 0
			}
			
			//------------------------------------------------------------------------------
			
			// 对比度左旋钮 - 按下
			if ( USART3_RX_BUF[0] == 'm' )
			{
				if ( LIMIT4 != 0 && contrastState == 1 ) {	
					u3_printf("CL\r\n");					// 对比度左限位已触发
				} else {
					GPIO_SetBits(GPIOD, GPIO_Pin_8);		// PD8 = 1
					GPIO_ResetBits(GPIOA, GPIO_Pin_7);		// PA7 = 0
					GPIO_SetBits(GPIOC, GPIO_Pin_6);		// PC6 = 1
					contrastState = 1;						// 往左转
				}
			}
			
			// 对比度右旋钮 - 按下
			if ( USART3_RX_BUF[0] == 'n' )
			{
				if ( LIMIT5 != 0 && contrastState == 2 ) {
					u3_printf("CR\r\n");					// 对比度右限位已触发
				} else {
					GPIO_SetBits(GPIOD, GPIO_Pin_8);		// PD8 = 1
					GPIO_SetBits(GPIOA, GPIO_Pin_7);		// PA7 = 1
					GPIO_ResetBits(GPIOC, GPIO_Pin_6);		// PC6 = 0
					contrastState = 2;						// 往右转
				}
			}
			
			//------------------------------------------------------------------------------
			
			// 双光源左旋钮 - 按下
			if ( USART3_RX_BUF[0] == 'o' )
			{
				if ( LIMIT6 != 0 && lightSource == 1 ) {	
					u3_printf("LL\r\n");					// 双光源左限位已触发
				} else {
					GPIO_SetBits(GPIOG, GPIO_Pin_6);		// PG6 = 1
					GPIO_ResetBits(GPIOB, GPIO_Pin_14);		// PB14 = 0
					GPIO_SetBits(GPIOC, GPIO_Pin_7);		// PC7 = 1
					lightSource = 1;						// 往左转	
				}
			}
			
			// 双光源右旋钮 - 按下
			if ( USART3_RX_BUF[0] == 'p' )
			{
				if ( LIMIT7 != 0 && lightSource == 2 ) {
					u3_printf("LR\r\n");					// 双光源右限位已触发
				} else {
					GPIO_SetBits(GPIOG, GPIO_Pin_6);		// PG6 = 1
					GPIO_SetBits(GPIOB, GPIO_Pin_14);		// PB14 = 1
					GPIO_ResetBits(GPIOC, GPIO_Pin_7);		// PC7 = 0
					lightSource = 2;						// 往右转
				}
			}
			
			//------------------------------------------------------------------------------
			
			// 对准
			if ( USART3_RX_BUF[0] == 'q' ) {
				// 发送信号给上位机，打开对准相机
				printf("8");
			}
			
			// 测量
			if ( USART3_RX_BUF[0] == 'r' ) {
				// 发送信号给上位机，启动测量
				printf("9");
			}
			
			USART3_RX_STA = 0;
		}
		
		// 串口
		if ( USART_RX_STA & 0x8000 )
		{
			u8 len = USART_RX_STA & 0x3fff;	// 得到此次接收到的数据长度
			
			// 调焦 M1
			if ( USART_RX_BUF[0] == 'a' ) {
				if ( LIMIT0 != 0 ) {
					printf("FL");							// 调焦左限位已触发
				} else {
					GPIO_SetBits(GPIOB, GPIO_Pin_0);		// PB0 = 1
					GPIO_ResetBits(GPIOE, GPIO_Pin_8);		// PE8 = 0
					GPIO_SetBits(GPIOE, GPIO_Pin_9);		// PE9 = 1
					
					if ( LIMIT1 != 0 ) {
						// 右限位触发的情况下，先转一会确保离开限位
						delay_ms(500);
					}
				}
			}
			
			if ( USART_RX_BUF[0] == 'b' ) {
				GPIO_ResetBits(GPIOB, GPIO_Pin_0);			// PB0 = 0
			}
			
			if ( USART_RX_BUF[0] == 'c' ) {
				if ( LIMIT1 != 0 ) {
					printf("FR");							// 调焦右限位已触发
				} else {
					GPIO_SetBits(GPIOB, GPIO_Pin_0);		// PB0 = 1
					GPIO_SetBits(GPIOE, GPIO_Pin_8);		// PE8 = 1
					GPIO_ResetBits(GPIOE, GPIO_Pin_9);		// PE9 = 0
					
					if ( LIMIT0 != 0 ) {
						// 左限位触发的情况下，先转一会确保离开限位
						delay_ms(500);
					}
				}
			}
			
			if ( USART_RX_BUF[0] == 'd' ) {
				GPIO_ResetBits(GPIOB, GPIO_Pin_0);			// PB0 = 0
			}
			
			//------------------------------------------------------------------------------
			
			// 变倍 M2
			if ( USART_RX_BUF[0] == 'e' ) {
				if ( LIMIT2 != 0 ) {
					printf("ZL");							// 变倍左限位已触发
				} else {
					/*
					// 点动模式
					GPIO_ResetBits(GPIOE, GPIO_Pin_10);		// PE10 = 0
					GPIO_SetBits(GPIOE, GPIO_Pin_11);		// PE11 = 1
					
					int count = 1;
					while( LIMIT2 == 0 )
					{
						if ( count == 20 ) {
							break;
						}
						
						GPIO_SetBits(GPIOE, GPIO_Pin_7);	// PE7 = 1
						delay_ms(2);
						GPIO_ResetBits(GPIOE, GPIO_Pin_7);	// PE7 = 0
						delay_ms(5);
						
						count++;
					}
					*/
					
					GPIO_SetBits(GPIOE, GPIO_Pin_7);		// PE7 = 1
					GPIO_ResetBits(GPIOE, GPIO_Pin_10);		// PE10 = 0
					GPIO_SetBits(GPIOE, GPIO_Pin_11);		// PE11 = 1
					
					if ( LIMIT3 != 0 ) {
						// 右限位触发的情况下，先转一会确保离开限位
						delay_ms(100);
					}
				}
			}
			
			if ( USART_RX_BUF[0] == 'f' ) {
				GPIO_ResetBits(GPIOE, GPIO_Pin_7);			// PE7 = 0
			}
			
			if ( USART_RX_BUF[0] == 'g' ) {
				if ( LIMIT3 != 0 ) {
					printf("ZR");							// 变倍右限位已触发
				} else {
					/*
					// 点动模式
					GPIO_SetBits(GPIOE, GPIO_Pin_10);		// PE10 = 1
					GPIO_ResetBits(GPIOE, GPIO_Pin_11);		// PE11 = 0
					
					int count = 1;
					while( LIMIT3 == 0 )
					{
						if ( count == 20 ) {
							break;
						}
						
						GPIO_SetBits(GPIOE, GPIO_Pin_7);	// PE7 = 1
						delay_ms(2);
						GPIO_ResetBits(GPIOE, GPIO_Pin_7);	// PE7 = 0
						delay_ms(5);
						
						count++;
					}
					*/
					
					GPIO_SetBits(GPIOE, GPIO_Pin_7);		// PE7 = 1
					GPIO_SetBits(GPIOE, GPIO_Pin_10);		// PE10 = 1
					GPIO_ResetBits(GPIOE, GPIO_Pin_11);		// PE11 = 0
					
					if ( LIMIT2 != 0 ) {
						// 左限位触发的情况下，先转一会确保离开限位
						delay_ms(100);
					}
				}
			}
			
			if ( USART_RX_BUF[0] == 'h' ) {
				GPIO_ResetBits(GPIOE, GPIO_Pin_7);			// PE7 = 0
			}
			
			//------------------------------------------------------------------------------
			
			// 明亮度 M3
			if ( USART_RX_BUF[0] == 'i' ) {
				GPIO_SetBits(GPIOE, GPIO_Pin_14);			// PE14 = 1
				GPIO_ResetBits(GPIOE, GPIO_Pin_13);			// PE13 = 0
				GPIO_SetBits(GPIOE, GPIO_Pin_12);			// PE12 = 1
			}
			
			if ( USART_RX_BUF[0] == 'j' ) {
				GPIO_ResetBits(GPIOE, GPIO_Pin_14);			// PE14 = 0
			}
			
			if ( USART_RX_BUF[0] == 'k' ) {
				GPIO_SetBits(GPIOE, GPIO_Pin_14);			// PE14 = 1
				GPIO_SetBits(GPIOE, GPIO_Pin_13);			// PE13 = 1
				GPIO_ResetBits(GPIOE, GPIO_Pin_12);			// PE12 = 0
				
			}
			
			if ( USART_RX_BUF[0] == 'l' ) {
				GPIO_ResetBits(GPIOE, GPIO_Pin_14);			// PE14 = 0
			}
			
			//------------------------------------------------------------------------------
			
			// 对比度 M4
			if ( USART_RX_BUF[0] == 'm' )
			{
				if ( LIMIT4 != 0 && contrastState == 1 ) {	
					printf("CL");							// 对比度左限位已触发
				} else {
					GPIO_SetBits(GPIOD, GPIO_Pin_8);		// PD8 = 1
					GPIO_ResetBits(GPIOA, GPIO_Pin_7);		// PA7 = 0
					GPIO_SetBits(GPIOC, GPIO_Pin_6);		// PC6 = 1
					contrastState = 1;						// 往左转
				}
			}
			
			if ( USART_RX_BUF[0] == 'n' )
			{
				if ( LIMIT5 != 0 && contrastState == 2 ) {
					printf("CR");							// 对比度右限位已触发
				} else {
					GPIO_SetBits(GPIOD, GPIO_Pin_8);		// PD8 = 1
					GPIO_SetBits(GPIOA, GPIO_Pin_7);		// PA7 = 1
					GPIO_ResetBits(GPIOC, GPIO_Pin_6);		// PC6 = 0
					contrastState = 2;						// 往右转
				}
			}
			
			//------------------------------------------------------------------------------
			
			// 双光源 M5
			if ( USART_RX_BUF[0] == 'o' )
			{
				if ( LIMIT6 != 0 && lightSource == 1 ) {	
					printf("LL");							// 双光源左限位已触发
				} else {
					GPIO_SetBits(GPIOG, GPIO_Pin_6);		// PG6 = 1
					GPIO_ResetBits(GPIOB, GPIO_Pin_14);		// PB14 = 0
					GPIO_SetBits(GPIOC, GPIO_Pin_7);		// PC7 = 1
					lightSource = 1;						// 往左转
				}
			}
			
			if ( USART_RX_BUF[0] == 'p' )
			{
				if ( LIMIT7 != 0 && lightSource == 2 ) {
					printf("LR");							// 双光源右限位已触发
				} else {
					GPIO_SetBits(GPIOG, GPIO_Pin_6);		// PG6 = 1
					GPIO_SetBits(GPIOB, GPIO_Pin_14);		// PB14 = 1
					GPIO_ResetBits(GPIOC, GPIO_Pin_7);		// PC7 = 0
					lightSource = 2;						// 往右转	
				}
			}
			
			//------------------------------------------------------------------------------
			
			// 明亮度 - 变亮
			if ( USART_RX_BUF[0] == '1' )
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_13);			// PE13 = 1
				GPIO_ResetBits(GPIOE, GPIO_Pin_12);			// PE12 = 0
				
				GPIO_SetBits(GPIOE, GPIO_Pin_14);			// PE14 = 1
				delay_ms(15);
				GPIO_ResetBits(GPIOE, GPIO_Pin_14);			// PE14 = 0
			}
			
			if ( USART_RX_BUF[0] == '2' )
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_13);			// PE13 = 1
				GPIO_ResetBits(GPIOE, GPIO_Pin_12);			// PE12 = 0
				
				GPIO_SetBits(GPIOE, GPIO_Pin_14);			// PE14 = 1
				delay_ms(30);
				GPIO_ResetBits(GPIOE, GPIO_Pin_14);			// PE14 = 0
			}
			
			if ( USART_RX_BUF[0] == '3' )
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_13);			// PE13 = 1
				GPIO_ResetBits(GPIOE, GPIO_Pin_12);			// PE12 = 0
				
				GPIO_SetBits(GPIOE, GPIO_Pin_14);			// PE14 = 1
				delay_ms(45);
				GPIO_ResetBits(GPIOE, GPIO_Pin_14);			// PE14 = 0
			}
			
			// 明亮度 - 变暗
			if ( USART_RX_BUF[0] == '4' )
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_13);			// PE13 = 0
				GPIO_SetBits(GPIOE, GPIO_Pin_12);			// PE12 = 1
				
				GPIO_SetBits(GPIOE, GPIO_Pin_14);			// PE14 = 1
				delay_ms(15);
				GPIO_ResetBits(GPIOE, GPIO_Pin_14);			// PE14 = 0
			}
			
			if ( USART_RX_BUF[0] == '5' )
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_13);			// PE13 = 0
				GPIO_SetBits(GPIOE, GPIO_Pin_12);			// PE12 = 1
				
				GPIO_SetBits(GPIOE, GPIO_Pin_14);			// PE14 = 1
				delay_ms(30);
				GPIO_ResetBits(GPIOE, GPIO_Pin_14);			// PE14 = 0
			}
			
			if ( USART_RX_BUF[0] == '6' )
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_13);			// PE13 = 0
				GPIO_SetBits(GPIOE, GPIO_Pin_12);			// PE12 = 1
				
				GPIO_SetBits(GPIOE, GPIO_Pin_14);			// PE14 = 1
				delay_ms(45);
				GPIO_ResetBits(GPIOE, GPIO_Pin_14);			// PE14 = 0
			}

			USART_RX_STA = 0;
		}
	}
}
