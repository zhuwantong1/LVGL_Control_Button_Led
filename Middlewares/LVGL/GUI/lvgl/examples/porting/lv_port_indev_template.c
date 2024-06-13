

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev_template.h"
#include "../../lvgl.h"
/* ������Ļ��������ͷ�ļ� */
#include "../HARDWARE/TOUCH/touch.h"
#include "../HARDWARE/KEY/key.h"
#include "lcd.h"
/* ������ */
static void touchpad_init(void);
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool touchpad_is_pressed(void);
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y);
lv_indev_t * indev_touchpad; /* ������ */
/**
* @brief ��ʼ����ע�������豸
* @param ��
* @retval ��
*/
void lv_port_indev_init(void)
{
 /**
 * 
 * ������������ҵ� LittlevGL ֧�ֵĳ����豸��ʵ��ʾ��:
 * - ������
 * - ��� (֧�ֹ��)
 * - ���� (��֧�ְ����� GUI �÷�)
 * - ������ (֧�ֵ� GUI �÷�������: ��, ��, ����)
 * - ��ť (������Ļ��ָ������ⲿ��ť)
 *
 * ���� `..._read()` ֻ��ʾ��
 * ����Ҫ���ݾ����Ӳ���������Щ����
 */
 static lv_indev_drv_t indev_drv;
 /*------------------
 * ������
 * -----------------*/
 /* ��һ�� ��ʼ�������� */
 touchpad_init();
 /* �ڶ��� ע�ᴥ���������豸 */
lv_indev_drv_init(&indev_drv);
/* ������ ѡ�������豸���ͣ������� */
 indev_drv.type = LV_INDEV_TYPE_POINTER;
 /* ���Ĳ� ���ô��������ȡ�ص����� */
indev_drv.read_cb = touchpad_read;
/* ע�������豸 */
 indev_touchpad = lv_indev_drv_register(&indev_drv);
}
/*------------------
* ������
* -----------------*/
/**
* @brief ��ʼ��������
* @param ��
* @retval ��
*/
static void touchpad_init(void)
{
		 /*Your code comes here*/
		 tp_dev.init();
		 /* ������������� */
		 if (KEY_Scan(0) == KEY0_PRES) /* KEY0 ����,��ִ��У׼���� */
		 {
			 LCD_Clear(WHITE); /* ���� */
			 TP_Adjust(); /* ��ĻУ׼ */
			 TP_Save_Adjdata();
		 }
}
/**
* @brief ͼ�ο�Ĵ�������ȡ�ص�����
* @param indev_drv : �������豸
* @arg data : �����豸���ݽṹ��
* @retval ��
*/
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
	 static lv_coord_t last_x = 0;
	 static lv_coord_t last_y = 0;
	 /* ���水�µ������״̬ */
	 if(touchpad_is_pressed())
	 {
		 touchpad_get_xy(&last_x, &last_y);
		 data->state = LV_INDEV_STATE_PR;
	 }
	 else
	 {
		 data->state = LV_INDEV_STATE_REL;
	 }
	 /* ��������µ����� */
	 data->point.x = last_x;
	 data->point.y = last_y;
}
/**
* @brief ��ȡ�������豸��״̬
* @param ��
* @retval ���ش������豸�Ƿ񱻰���
*/
static bool touchpad_is_pressed(void)
{
 /*Your code comes here*/
 tp_dev.scan(0);
 if (tp_dev.sta & TP_PRES_DOWN)
 {
 return true;
 }
 return false;
}
/**
* @brief �ڴ����������µ�ʱ���ȡ x��y ����
* @param x : x �����ָ��
* @arg y : y �����ָ��
* @retval ��
*/
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{
 /*Your code comes here*/
 (*x) = tp_dev.x[0];
 (*y) = tp_dev.y[0];
}

