/**
 * 这是工程的 APP 可 OTA 项目
 * 关键点如下
 * 1.需要有一个触发跳转至 BOOT 的关键帧，这里使用 OTA 的 ping 帧触发，实际项目中依据需要调整
 * 2.跳转前做好环境清理，确保 BOOT 的硬件初始化正常运作
 * 3.BOOT 程序占用 FLASH 的第一片，APP 程序应该从第二片作为起始，同时注意调整中断向量表的偏移(SysInit中做到这一点)
 * 4.这里使用到作者的另一个项目帮助485的流控 https://github.com/stbanana/Dataflow/tree/main
 * 5.编译后占满了 RAM 是正常现象，通过链接器脚本将所有剩余空间作为了芯片堆栈使用
 */

#include <Includes.h>

/* 方便管理485的 UART 包装*/
extern _DFlow DFlow;
/* 作为协议解析的buffer，简单示例 */
uint8_t  RxBuffer[1048];
uint16_t RxLen = 0;
/* 进入boot的延时计数，需要做好回复和一切准备工作再进入boot */
uint32_t RunBootTick = 0;

/* 一些符合协议的ping帧 */
/* 对机柜广播、模块广播、模块类型1的ping帧 */
const uint8_t PingExtra0[] = {0xFE, 0xEF, 0x00, 0x04, 0xFA, 0x05, 0x08, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF3};
/* 对机柜广播、模块1、模块类型1的ping帧 */
const uint8_t PingExtra1[] = {0xFE, 0xEF, 0x01, 0x04, 0xFA, 0x05, 0x08, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF2};
/* 对机柜1、模块1、模块类型1的ping帧 */
const uint8_t PingExtra2[] = {0xFE, 0xEF, 0x41, 0x04, 0xFA, 0x05, 0x08, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB2};

/* 符合协议的回复帧 */
const uint8_t PingReply[] = {0xFE, 0xEF, 0x41, 0x80, 0xFA, 0x00, 0x08, 0x00, 0x00, 0x01, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x35};

/* LED分频 */
static uint16_t LEDTime = 0;

/* 总线空闲态手动监测 */
extern int8_t U2IDLE_Delay;
extern size_t U2RxBufferIndex;

/**
 * @brief 主程序入口
 */
int main(void)
{
    SysInit( );
    DFlow_User_Init( );

    /* 切换RS485收发芯片为接收模式 */
    gpio_bit_write(GPIOB, GPIO_PIN_1, RESET);
    while(1)
    {
        /* ping帧监测 */
        RxLen = 0;
        while(DFlow_Getc(&DFlow, &RxBuffer[RxLen]) == DFLOW_API_RETURN_DEFAULT)
        {
            RxLen++;
            if(RxLen >= sizeof(PingExtra0)) // 接收了符合帧长
            {
                if(memcmp(RxBuffer, PingExtra0, sizeof(PingExtra0)) == 0 || //
                   memcmp(RxBuffer, PingExtra1, sizeof(PingExtra1)) == 0 || //
                   memcmp(RxBuffer, PingExtra2, sizeof(PingExtra2)) == 0)
                {
                    /* 三种符合规则的ping，触发进入boot */
                    if(RunBootTick == 0)
                    {
                        RunBootTick = 1;
                    }
                    /* 进行回复 */
                    DFlow_Write(&DFlow, (uint8_t *)PingReply, sizeof(PingReply));
                }
            }
        }
        RxLen = 0;

        /* 延时重启，供回复帧发出以及其他的准备工作 */
        if(RunBootTick)
        {
            RunBootTick++;
            if(RunBootTick >= 500) // 1秒后进入boot
            {
                /* 关闭中断，清理环境 */
                InitNVIC( );
                /* 特殊接口，固化在 BOOT 的中断向量表中，对应 BOOT 的 SVC_Handler */
                (*((void (*)(void))(*(uint32_t *)0x2c)))( );
                while(1)
                    ;
            }
        }

        /* app的LED闪烁表示 */
        if(LEDTime++ > 500) // APP中的LED慢速闪烁
        {
            gpio_bit_write(GPIOC, GPIO_PIN_4, !gpio_input_bit_get(GPIOC, GPIO_PIN_4));
            LEDTime = 0;

            // usart_data_transmit(USART0, 0XAA);
            // usart_data_transmit(USART2, 0XAA);
            // usart_data_transmit(UART3, 0XAA);
        }

        /* 空闲中断处理手动触发 */
        if(U2IDLE_Delay > 0)
        {
            U2IDLE_Delay--;
            if(U2IDLE_Delay == 0)
            {
                DFlow_Interrupt_IDLE_RC_FTF(&DFlow, U2RxBufferIndex);
                U2RxBufferIndex = 0;
            }
        }
        DFlow_Ticks(&DFlow); // 协助485管理
        delay_1ms(1);
    }
}
