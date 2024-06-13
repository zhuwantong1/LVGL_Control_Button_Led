#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sram.h"
#include "malloc.h" 
#include "lvgl.h"
#include "lv_port_indev_template.h"
#include "lv_port_disp_template.h"
#include "timer.h"
/************************************************
 ALIENTEK 精英STM32F103开发板 实验34
 内存管理实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺： http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/  

int main(void)
{	
	
  HAL_Init();                    	 	//初始化HAL库    
  Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
	uart_init(115200);					//初始化串口	
	LED_Init();							//初始化LED	
	KEY_Init();							//初始化按键
 	LCD_Init();           				//初始化LCD FSMC接口 
	SRAM_Init();						//初始化外部SRAM  
	my_mem_init(SRAMIN);				//初始化内部内存池
	 
	
	/* 系统其他硬件初始化信息，这里忽略 */
 TIM3_Init(100-1,7200-1); /* 根据自己的开发板 MCU 定义定时器周期为 1ms */
 lv_init(); /* lvgl 系统初始化 */
 lv_port_disp_init(); /* lvgl 显示接口初始化,放在 lv_init()的后面 */
 lv_port_indev_init(); /* lvgl 输入接口初始化,放在 lv_init()的后面 */
 lv_obj_t *label = lv_label_create(lv_scr_act());
 lv_obj_t *label2 = lv_label_create(lv_scr_act());
 lv_label_set_text(label,"Hello Alientek!!!");
 lv_label_set_text(label2,"nihao");
 lv_obj_center(label);
 while(1)
 {
	 lv_timer_handler(); /* LVGL 管理函数相当于 RTOS 触发任务调度函数 */
	 delay_ms(500);
	 LED0=!LED0;
	 //printf("nihao\r\n");
 }
}

