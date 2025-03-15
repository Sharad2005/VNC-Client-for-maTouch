SET(SOURCES screens/ui_Screen1.c
    screens/ui_Screen2.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_clock_png.c
    images/ui_img_arm_hour_png.c
    images/ui_img_arm_minute_png.c
    images/ui_img_arm_second_png.c
    images/ui_img_240x60_png.c
    images/ui_img_clock2_png.c)

add_library(ui ${SOURCES})
