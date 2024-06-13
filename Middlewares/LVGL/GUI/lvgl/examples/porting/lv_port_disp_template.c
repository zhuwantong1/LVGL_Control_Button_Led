

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
#define USE_SRAM 0 /* 使用外部 sram 为 1，否则为 0 */

#ifdef USE_SRAM

#include "../MALLOC/malloc.h"

#endif
/*此处高度和宽度改不改无所谓*/
#define MY_DISP_HOR_RES (320) /* 屏幕宽度 */
#define MY_DISP_VER_RES (240) /* 屏幕高度 */
 
 
/* 显示设备初始化函数 */
	static void disp_init(void);
/* 显示设备刷新函数 */
	static void disp_flush(lv_disp_drv_t * disp_drv,const lv_area_t * area,lv_color_t * color_p);
	
/**
* @brief 初始化并注册显示设备
* @param 无
* @retval 无
*/

void lv_port_disp_init(void)
{
	 /* 第一步 初始化显示设备 */
			disp_init();
	 /*-----------------------------
	 * 创建一个绘图缓冲区
	 *----------------------------*/
	 /**
	 * LVGL 需要一个缓冲区用来绘制小部件
	 * 随后，这个缓冲区的内容会通过显示设备的 flush_cb(显示设备刷新函数) 复制到显示设备上
	 * 这个缓冲区的大小需要大于显示设备一行的大小
	 *
	 * 这里有 3 中缓冲配置:
	 * 1. 单缓冲区:
	 * LVGL 会将显示设备的内容绘制到这里，并将他写入显示设备。
	 *
	 * 2. 双缓冲区:
	 * LVGL 会将显示设备的内容绘制到其中一个缓冲区，并将他写入显示设备。
	 * 需要使用 DMA 将要显示在显示设备的内容写入缓冲区。
	 * 当数据从第一个缓冲区发送时，它将使 LVGL 能够将屏幕的下一部分绘制到另一个缓冲区。
	 * 这样使得渲染和刷新可以并行执行。
		*
	 * 3. 全尺寸双缓冲区
	 * 设置两个屏幕大小的全尺寸缓冲区，并且设置 disp_drv.full_refresh = 1。
	 * LVGL 将始终以 'flush_cb' 的形式提供整个渲染屏幕，只需更改帧缓冲区的地址。
	 */
	 /* 单缓冲区示例) */
	
	static lv_disp_draw_buf_t draw_buf_dsc_1;
	
	#if USE_SRAM
	 /* 设置缓冲区的大小为屏幕的全尺寸大小 */
	 void * buf_1 = mymalloc(SRAMEX, MY_DISP_HOR_RES * MY_DISP_VER_RES);
	 /* 初始化显示缓冲区 */
	 lv_disp_draw_buf_init(&draw_buf_dsc_1,
	buf_1, NULL,
	MY_DISP_HOR_RES * MY_DISP_VER_RES);
	#else
	 /* 设置缓冲区的大小为 10 行屏幕的大小 */
			static lv_color_t buf_1[MY_DISP_HOR_RES * 10];
	 /* 第二步 初始化显示缓冲区 */
			lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10);
	#endif
	 /* 第三步 在 LVGL 中注册显示设备 */
			static lv_disp_drv_t disp_drv; /* 显示设备的描述符 */
			lv_disp_drv_init(&disp_drv); /* 初始化为默认值 */
	 /* 第四步 设置显示设备的分辨率
	 * 这里为了适配正点原子的多款屏幕，采用了动态获取的方式，
	 * 在实际项目中，通常所使用的屏幕大小是固定的，因此可以直接设置为屏幕的大小 */
			disp_drv.hor_res = lcddev.width;
			disp_drv.ver_res = lcddev.height;
	 /* 第五步 用来将缓冲区的内容复制到显示设备 */
			disp_drv.flush_cb = disp_flush;
	 /* 第六步 设置显示缓冲区 */
			disp_drv.draw_buf = &draw_buf_dsc_1;
	 /* 第七步 注册显示设备 */
			lv_disp_drv_register(&disp_drv);
}
/**
* @brief 初始化显示设备和必要的外围设备
* @param 无
* @retval 无
*/
static void disp_init(void)
{
	 /*You code here*/
	 LCD_Init(); /* 初始化 LCD */
	 LCD_Display_Dir(1); /* 设置横屏 */
}
/**
* @brief 将内部缓冲区的内容刷新到显示屏上的特定区域
* @note 可以使用 DMA 或者任何硬件在后台加速执行这个操作
* 但是，需要在刷新完成后调用函数 'lv_disp_flush_ready()'
* @param disp_drv : 显示设备
* @arg area : 要刷新的区域，包含了填充矩形的对角坐标
* @arg color_p : 颜色数组
*
* @retval 无
*/
static void disp_flush(lv_disp_drv_t * disp_drv,const lv_area_t * area,lv_color_t * color_p)
{
	 /* 重要！！！LCD 驱动函数，在指定区域内填充指定颜色块 */
	 LCD_Color_Fill(area->x1,area->y1,area->x2,area->y2,(uint16_t*)color_p);
	 /* 重要!!!* 通知图形库，已经刷新完毕了 */
	 lv_disp_flush_ready(disp_drv);
}
