<p align="center">
 <h1 align="center">YoroMCU-OTA</h1>
 <p align="center">一个极简的bootload和OTA协议栈，MCU被动部分实现</p>
</p>



<p align="center">
    <a href="https://github.com/anuraghazra/github-readme-stats/graphs/contributors"><img alt="C/C++" src="https://img.shields.io/badge/C/C++-00599C.svg?logo=c&logoColor=white" /></a>
    <a href="https://mit-license.org/"><img alt="Static Badge" src="https://img.shields.io/badge/license%20-%20MIT%203.0%20-%20blue" /></a>
</p>

<p align="center">
    <a href="https://wiki.yono233.cn/YoroMCU-OTA/zh_hans/">文档</a>
</p>

<p align="center">
    <a href="/README.md">简体中文</a>
    ·
    <a href="/README_EN.md">English</a>
</p>
## ✨特性

> 1. 🔢**轻量化**：相比其他OTA协议的最大优势，极简协议，ROM开销500字节左右，RAM开销可在560字节以下。
> 2. 🧂**硬件无关**：实现收发、擦写的接口绑定即可使用，硬件配置可以沿用项目应用。
> 3. 🙋‍♂️**多接口**：理论上只是一种串行协议支持任意接口，提供uart、CAN接口的无感接口。
> 4. 🕸️**内转发**：假设一个机器有两个MCU，其中一个不直接对外通信，本协议支持向内转发烧录。

## 📚功能计划

- [x] UART 单对单接口的协议及工程示例
- [ ] UART 总线接口的协议及工程示例
- [ ] CAN 接口的协议及工程示例
- [ ] 固件加密体系
- [ ] 差分APP BANK

## 👯‍♀️整体思路

例如一个 MCU 的 FLASH 范围是 0x08000000~0x08010000，每片大小是 0x2000，那么我们将第一片拿来作为 OTA 固件，而 0x08002000 开始作为 APP。

OTA 固件是一个完整工程具有完整程序，只具有 OTA 的升级功能。APP 则是实际产品功能。所以一共是两个工程。

这样做的最大好处是**稳定、简单、小资源**。比起 RAM 加载自擦写、双 APP 区域互载等看似高级的方案，无论如何 OTA 固件无需擦除不会改变，使得 MCU 不会脱离我们的掌控，永远可以通过通信端口 OTA。

而本 OTA 协议栈就是移植进 OTA 固件工程。

## 🤔如何使用

首先外设的初始化需要自行完成，本库无法接管这部分。另外为节省资源、boot 固件稳定，不使用函数绑定，需要自行修改 `port` 源码。

**依赖项目**

依赖项目的源码包同样包含在本项目中，便于调用。但使用者需要知道，这实际上是另一个完整项目。

- 负责统一函数返回码的纯头文件库 [stbanana/ReturnCodesC: A series of common C/C++ function return values.](https://github.com/stbanana/ReturnCodesC)

**按照模板填写port**

硬件平台不同，需要修改填写的部分会标记`(需填)`

假设是使用 uart，需要修改 Port/MCU 下的 `BootPort.c`，其中的三个函数实现芯片擦写、重启。

修改 Port/MCU 下的 `UartBootPort.c`，实现通信接口的收发。

修改 Port/MCU 下的 `BootConfig.h`，确认烧录起始地址和 APP 分区大小。

**初始化及周期调用**

假设是使用 uart。在 BootConfig.h 中将`BOOT_PORT_UART`宏置1开启 uart 的 OTA 功能。
在 OTA 固件部分。

```c
#include <YoroOTA.h>

int main(void)
{
    //...省略其他本库无关，例如串口和芯片初始化
    
    /* 初始化YoroOTA系统 */
    YoroOTAInit();
    
    while(1)
    {
        /* 以任意方式周期调用YoroOTA，传参为毫秒 */
        YoroOTARun(1); // YoroOTA升级程序处理
        delay_1ms(1);
    }
}
```

