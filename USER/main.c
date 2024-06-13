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
 ALIENTEK ��ӢSTM32F103������ ʵ��34
 �ڴ����ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣� http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/  

int main(void)
{	
	
  HAL_Init();                    	 	//��ʼ��HAL��    
  Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);               		//��ʼ����ʱ����
	uart_init(115200);					//��ʼ������	
	LED_Init();							//��ʼ��LED	
	KEY_Init();							//��ʼ������
 	LCD_Init();           				//��ʼ��LCD FSMC�ӿ� 
	SRAM_Init();						//��ʼ���ⲿSRAM  
	my_mem_init(SRAMIN);				//��ʼ���ڲ��ڴ��
	 
	
	/* ϵͳ����Ӳ����ʼ����Ϣ��������� */
 TIM3_Init(100-1,7200-1); /* �����Լ��Ŀ����� MCU ���嶨ʱ������Ϊ 1ms */
 lv_init(); /* lvgl ϵͳ��ʼ�� */
 lv_port_disp_init(); /* lvgl ��ʾ�ӿڳ�ʼ��,���� lv_init()�ĺ��� */
 lv_port_indev_init(); /* lvgl ����ӿڳ�ʼ��,���� lv_init()�ĺ��� */
 lv_obj_t *label = lv_label_create(lv_scr_act());
 lv_obj_t *label2 = lv_label_create(lv_scr_act());
 lv_label_set_text(label,"Hello Alientek!!!");
 lv_label_set_text(label2,"nihao");
 lv_obj_center(label);
 while(1)
 {
	 lv_timer_handler(); /* LVGL �������൱�� RTOS ����������Ⱥ��� */
	 delay_ms(500);
	 LED0=!LED0;
	 //printf("nihao\r\n");
 }
}

