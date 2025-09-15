/**
 * 这是工程的 APP 可 OTA 项目
 * 关键点如下
 * 1.需要有一个触发跳转至 BOOT 的关键帧，这里使用 OTA 的 ping 帧触发，实际项目中依据需要调整
 * 2.跳转前做好环境清理，确保 BOOT 的硬件初始化正常运作
 * 3.BOOT 程序占用 FLASH 的第一片，APP 程序应该从第二片作为起始，同时注意调整中断向量表的偏移(SysInit中做到这一点)
 * 4.编译后占满了 RAM 是正常现象，通过链接器脚本将所有剩余空间作为了芯片堆栈使用
 */

#include <Includes.h>

/* 进入boot的延时计数，需要做好回复和一切准备工作再进入boot */
uint32_t RunBootTick = 0;

/* 一些符合协议的ping帧 */
/* 对机柜广播、模块广播、模块类型1的ping帧 */
const _CAN_MSG_FRAME PingExtra0 = {
    .ID = 0x05FA0400, .Data = {0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};
/* 对机柜广播、模块1、模块类型1的ping帧 */
const _CAN_MSG_FRAME PingExtra1 = {
    .ID = 0x05FA0401, .Data = {0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};
/* 对机柜1、模块1、模块类型1的ping帧 */
const _CAN_MSG_FRAME PingExtra2 = {
    .ID = 0x05FA4141, .Data = {0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

/* 符合协议的回复帧 */
const _CAN_MSG_FRAME PingReply = {
    .ID = 0x00FA8041, .Data = {0x00, 0x01, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00},
         .MsgLen = 8
};

/* LED分频 */
static uint16_t LEDTime = 0;

/**
 * @brief 主程序入口
 */
int main(void)
{
    SysInit( );

    /* 预设发送帧配置 */
    CAN1TxMessage.tx_efid = 0x00;
    CAN1TxMessage.tx_ft   = CAN_FT_DATA;
    CAN1TxMessage.tx_ff   = CAN_FF_EXTENDED;
    CAN1TxMessage.tx_dlen = 8;

    while(1)
    {
        /* ping帧监测 */
        _CAN_MSG_FRAME getMeg;
        while(Queue_CanMsgPop(&CAN1MsgQueue, &getMeg) == 1)
        {
            if(memcmp(getMeg.Data, PingExtra0.Data, sizeof(PingExtra0.Data)) == 0 || //
               memcmp(getMeg.Data, PingExtra1.Data, sizeof(PingExtra1.Data)) == 0 || //
               memcmp(getMeg.Data, PingExtra2.Data, sizeof(PingExtra2.Data)) == 0)
            {
                CAN1TxMessage.tx_efid = PingReply.ID;
                CAN1TxMessage.tx_dlen = PingReply.MsgLen;
                memcpy(CAN1TxMessage.tx_data, PingReply.Data, PingReply.MsgLen);
                can_message_transmit(CAN1, &CAN1TxMessage);

                RunBootTick = 1; // 收到ping帧，准备进入boot
            }
        }

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
        if(LEDTime++ > 1000) // APP中的LED慢速闪烁
        {
            gpio_bit_write(GPIOC, GPIO_PIN_4, !gpio_input_bit_get(GPIOC, GPIO_PIN_4));
            LEDTime = 0;
        }

        delay_1ms(1);
    }
}
