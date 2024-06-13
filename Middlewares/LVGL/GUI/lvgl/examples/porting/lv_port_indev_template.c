

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev_template.h"
#include "../../lvgl.h"
/* 导入屏幕触摸驱动头文件 */
#include "../HARDWARE/TOUCH/touch.h"
#include "../HARDWARE/KEY/key.h"
#include "lcd.h"
/* 触摸屏 */
static void touchpad_init(void);
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool touchpad_is_pressed(void);
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y);
lv_indev_t * indev_touchpad; /* 触摸屏 */
/**
* @brief 初始化并注册输入设备
* @param 无
* @retval 无
*/
void lv_port_indev_init(void)
{
 /**
 * 
 * 在这里你可以找到 LittlevGL 支持的出入设备的实现示例:
 * - 触摸屏
 * - 鼠标 (支持光标)
 * - 键盘 (仅支持按键的 GUI 用法)
 * - 编码器 (支持的 GUI 用法仅包括: 左, 右, 按下)
 * - 按钮 (按下屏幕上指定点的外部按钮)
 *
 * 函数 `..._read()` 只是示例
 * 你需要根据具体的硬件来完成这些函数
 */
 static lv_indev_drv_t indev_drv;
 /*------------------
 * 触摸屏
 * -----------------*/
 /* 第一步 初始化触摸屏 */
 touchpad_init();
 /* 第二步 注册触摸屏输入设备 */
lv_indev_drv_init(&indev_drv);
/* 第三步 选择输入设备类型：触摸屏 */
 indev_drv.type = LV_INDEV_TYPE_POINTER;
 /* 第四步 设置触摸坐标读取回调函数 */
indev_drv.read_cb = touchpad_read;
/* 注册输入设备 */
 indev_touchpad = lv_indev_drv_register(&indev_drv);
}
/*------------------
* 触摸屏
* -----------------*/
/**
* @brief 初始化触摸屏
* @param 无
* @retval 无
*/
static void touchpad_init(void)
{
		 /*Your code comes here*/
		 tp_dev.init();
		 /* 电阻屏坐标矫正 */
		 if (KEY_Scan(0) == KEY0_PRES) /* KEY0 按下,则执行校准程序 */
		 {
			 LCD_Clear(WHITE); /* 清屏 */
			 TP_Adjust(); /* 屏幕校准 */
			 TP_Save_Adjdata();
		 }
}
/**
* @brief 图形库的触摸屏读取回调函数
* @param indev_drv : 触摸屏设备
* @arg data : 输入设备数据结构体
* @retval 无
*/
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
	 static lv_coord_t last_x = 0;
	 static lv_coord_t last_y = 0;
	 /* 保存按下的坐标和状态 */
	 if(touchpad_is_pressed())
	 {
		 touchpad_get_xy(&last_x, &last_y);
		 data->state = LV_INDEV_STATE_PR;
	 }
	 else
	 {
		 data->state = LV_INDEV_STATE_REL;
	 }
	 /* 设置最后按下的坐标 */
	 data->point.x = last_x;
	 data->point.y = last_y;
}
/**
* @brief 获取触摸屏设备的状态
* @param 无
* @retval 返回触摸屏设备是否被按下
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
* @brief 在触摸屏被按下的时候读取 x、y 坐标
* @param x : x 坐标的指针
* @arg y : y 坐标的指针
* @retval 无
*/
static void touchpad_get_xy(lv_coord_t * x, lv_coord_t * y)
{
 /*Your code comes here*/
 (*x) = tp_dev.x[0];
 (*y) = tp_dev.y[0];
}

