#include <iostream>
#include <unistd.h>

#include "lvgl/lvgl.h"
#include "lv_drv_conf.h"

#include "lv_drivers/sdl/sdl.h"

#include "Common/HAL/HAL.h"
#include "App.h"

extern "C" {
    LV_IMG_DECLARE(mouse_cursor_icon)
}

static void hal_init();
static void hal_deinit();
static void* tick_thread(void *data);

static pthread_t thr_tick;    /* thread */
static bool end_tick = false; /* flag to terminate thread */

int main(int argc,char **argv)
{
    lv_init();

    hal_init();

    HAL::HAL_Init();

    App_Init();

    while (1)
    {
        lv_timer_handler();
        HAL::HAL_Update();
        usleep( 5 * 1000);
    }
    
    hal_deinit();
    App_Uninit();
    return 0;
}

static void hal_init(void)
{

#if USE_SDL
  sdl_init();
#else
  // todo
#endif
    /* mouse input device */
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1);
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;
    // mouse_init();
    indev_drv_1.read_cb = sdl_mouse_read;

    /* keyboard input device */
    static lv_indev_drv_t indev_drv_2;
    lv_indev_drv_init(&indev_drv_2);
    indev_drv_2.type = LV_INDEV_TYPE_KEYPAD;
    // keyboard_init();
    indev_drv_2.read_cb = sdl_keyboard_read;

    /* mouse scroll wheel input device */
    static lv_indev_drv_t indev_drv_3;
    lv_indev_drv_init(&indev_drv_3);
    indev_drv_3.type = LV_INDEV_TYPE_ENCODER;
    // mousewheel_init();
    indev_drv_3.read_cb = sdl_mousewheel_read;

    auto group = lv_group_create();
    lv_group_set_default(group);

    lv_disp_t *disp = NULL;

    /*Create a display buffer*/
    static lv_disp_draw_buf_t disp_buf1;
    static lv_color_t buf1_1[MONITOR_HOR_RES * 100];
    static lv_color_t buf1_2[MONITOR_HOR_RES * 100];
    lv_disp_draw_buf_init(&disp_buf1, buf1_1, buf1_2, MONITOR_HOR_RES * 100);

    /*Create a display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/
    disp_drv.draw_buf = &disp_buf1;
    disp_drv.flush_cb = sdl_display_flush;
    disp_drv.hor_res = MONITOR_HOR_RES;
    disp_drv.ver_res = MONITOR_VER_RES;
    disp_drv.antialiasing = 1;

    disp = lv_disp_drv_register(&disp_drv);    

    /* Tick init */
    end_tick = false;
    pthread_create(&thr_tick, NULL, tick_thread, NULL);

    /* register input devices */
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);
    lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_2);
    lv_indev_t *enc_indev = lv_indev_drv_register(&indev_drv_3);
    lv_indev_set_group(kb_indev, group);
    lv_indev_set_group(enc_indev, group);

    lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor*/
    lv_img_set_src(cursor_obj, &mouse_cursor_icon);      /*Set the image source*/
    lv_indev_set_cursor(mouse_indev, cursor_obj);        /*Connect the image  object to the driver*/

}

static void hal_deinit(void)
{
  end_tick = true;
  pthread_join(thr_tick, NULL);
}

static void* tick_thread(void *data) {
  (void)data;

  while(!end_tick) {
    usleep(5000);
    lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
  }
  return NULL;
}
