/**
 * 这是工程的 BOOT 固件项目
 * 对 YoroMCU-OTA 的修改如下
 * 1.YoroMCU-OTA/Port/MCU/BootConfig.h 中的各个定义，将 BOOT_PORT_UART 置 0 裁剪掉 UART OTA 功能。 
 * 2.YoroMCU-OTA/Port/MCU/BootPort.c 中的每个(需填)项修改为符合芯片
 * 3.YoroMCU-OTA/Port/MCU/UartBootPort.c 中的每个(需填)项修改为符合平台
 */

#include <Includes.h>

/**
 * @brief 主程序入口
 */
int main(void)
{
    LoadApp( );   // 检查是否有APP，如果有，那么加载APP并运行
    UpdataApp( ); // 否则进入 OTA 态等待烧写
}
