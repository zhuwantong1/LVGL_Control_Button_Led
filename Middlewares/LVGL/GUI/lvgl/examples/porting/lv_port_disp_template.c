

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp_template.h"
#include "../../lvgl.h"
#include <stdbool.h>
#include "../HARDWARE/LCD/lcd.h"

/*********************
 *      DEFINES
 *********************/
#define USE_SRAM 0 /* ʹ���ⲿ sram Ϊ 1������Ϊ 0 */

#ifdef USE_SRAM

#include "../MALLOC/malloc.h"

#endif
/*�˴��߶ȺͿ�ȸĲ�������ν*/
#define MY_DISP_HOR_RES (320) /* ��Ļ��� */
#define MY_DISP_VER_RES (240) /* ��Ļ�߶� */
 
 
/* ��ʾ�豸��ʼ������ */
	static void disp_init(void);
/* ��ʾ�豸ˢ�º��� */
	static void disp_flush(lv_disp_drv_t * disp_drv,const lv_area_t * area,lv_color_t * color_p);
	
/**
* @brief ��ʼ����ע����ʾ�豸
* @param ��
* @retval ��
*/

void lv_port_disp_init(void)
{
	 /* ��һ�� ��ʼ����ʾ�豸 */
			disp_init();
	 /*-----------------------------
	 * ����һ����ͼ������
	 *----------------------------*/
	 /**
	 * LVGL ��Ҫһ����������������С����
	 * �����������������ݻ�ͨ����ʾ�豸�� flush_cb(��ʾ�豸ˢ�º���) ���Ƶ���ʾ�豸��
	 * ����������Ĵ�С��Ҫ������ʾ�豸һ�еĴ�С
	 *
	 * ������ 3 �л�������:
	 * 1. ��������:
	 * LVGL �Ὣ��ʾ�豸�����ݻ��Ƶ����������д����ʾ�豸��
	 *
	 * 2. ˫������:
	 * LVGL �Ὣ��ʾ�豸�����ݻ��Ƶ�����һ����������������д����ʾ�豸��
	 * ��Ҫʹ�� DMA ��Ҫ��ʾ����ʾ�豸������д�뻺������
	 * �����ݴӵ�һ������������ʱ������ʹ LVGL �ܹ�����Ļ����һ���ֻ��Ƶ���һ����������
	 * ����ʹ����Ⱦ��ˢ�¿��Բ���ִ�С�
		*
	 * 3. ȫ�ߴ�˫������
	 * ����������Ļ��С��ȫ�ߴ绺�������������� disp_drv.full_refresh = 1��
	 * LVGL ��ʼ���� 'flush_cb' ����ʽ�ṩ������Ⱦ��Ļ��ֻ�����֡�������ĵ�ַ��
	 */
	 /* ��������ʾ��) */
	
	static lv_disp_draw_buf_t draw_buf_dsc_1;
	
	#if USE_SRAM
	 /* ���û������Ĵ�СΪ��Ļ��ȫ�ߴ��С */
	 void * buf_1 = mymalloc(SRAMEX, MY_DISP_HOR_RES * MY_DISP_VER_RES);
	 /* ��ʼ����ʾ������ */
	 lv_disp_draw_buf_init(&draw_buf_dsc_1,
	buf_1, NULL,
	MY_DISP_HOR_RES * MY_DISP_VER_RES);
	#else
	 /* ���û������Ĵ�СΪ 10 ����Ļ�Ĵ�С */
			static lv_color_t buf_1[MY_DISP_HOR_RES * 10];
	 /* �ڶ��� ��ʼ����ʾ������ */
			lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10);
	#endif
	 /* ������ �� LVGL ��ע����ʾ�豸 */
			static lv_disp_drv_t disp_drv; /* ��ʾ�豸�������� */
			lv_disp_drv_init(&disp_drv); /* ��ʼ��ΪĬ��ֵ */
	 /* ���Ĳ� ������ʾ�豸�ķֱ���
	 * ����Ϊ����������ԭ�ӵĶ����Ļ�������˶�̬��ȡ�ķ�ʽ��
	 * ��ʵ����Ŀ�У�ͨ����ʹ�õ���Ļ��С�ǹ̶��ģ���˿���ֱ������Ϊ��Ļ�Ĵ�С */
			disp_drv.hor_res = lcddev.width;
			disp_drv.ver_res = lcddev.height;
	 /* ���岽 �����������������ݸ��Ƶ���ʾ�豸 */
			disp_drv.flush_cb = disp_flush;
	 /* ������ ������ʾ������ */
			disp_drv.draw_buf = &draw_buf_dsc_1;
	 /* ���߲� ע����ʾ�豸 */
			lv_disp_drv_register(&disp_drv);
}
/**
* @brief ��ʼ����ʾ�豸�ͱ�Ҫ����Χ�豸
* @param ��
* @retval ��
*/
static void disp_init(void)
{
	 /*You code here*/
	 LCD_Init(); /* ��ʼ�� LCD */
	 LCD_Display_Dir(1); /* ���ú��� */
}
/**
* @brief ���ڲ�������������ˢ�µ���ʾ���ϵ��ض�����
* @note ����ʹ�� DMA �����κ�Ӳ���ں�̨����ִ���������
* ���ǣ���Ҫ��ˢ����ɺ���ú��� 'lv_disp_flush_ready()'
* @param disp_drv : ��ʾ�豸
* @arg area : Ҫˢ�µ����򣬰����������εĶԽ�����
* @arg color_p : ��ɫ����
*
* @retval ��
*/
static void disp_flush(lv_disp_drv_t * disp_drv,const lv_area_t * area,lv_color_t * color_p)
{
	 /* ��Ҫ������LCD ������������ָ�����������ָ����ɫ�� */
	 LCD_Color_Fill(area->x1,area->y1,area->x2,area->y2,(uint16_t*)color_p);
	 /* ��Ҫ!!!* ֪ͨͼ�ο⣬�Ѿ�ˢ������� */
	 lv_disp_flush_ready(disp_drv);
}
