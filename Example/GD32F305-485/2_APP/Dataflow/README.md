<p align="center">
 <h1 align="center">Dataflow</h1>
 <p align="center">一个流式数据的驱动中间件</p>
</p>

<p align="center">
    <a href="https://github.com/anuraghazra/github-readme-stats/graphs/contributors"><img alt="C" src="https://img.shields.io/badge/C-00599C.svg?logo=c&logoColor=white" /></a>
    <a href="https://www.gnu.org/licenses/gpl-3.0.en.html"><img alt="Static Badge" src="https://img.shields.io/badge/license%20-%20GPL%203.0%20-%20blue" /></a>
</p>

<p align="center">
    <a href="https://wiki.yono233.cn/Dataflow/zh_hans/">文档</a>
</p>

<p align="center">
    <a href="/README.md">简体中文</a>
    ·
    <a href="/Example/README.DATA/README_EN.md">English</a>
</p>

# 💉作用
期望帮助那些管理不好以下两项的小白，符合任何一项都可以使用本库解决。

>传输流的 DMA。
>
>传输流的半双工用法。

我相信不使用 DMA 且工作在全双工通道，任何人都能简单编写运行良好的驱动。所以本库不建议在这种场景使用，未来也不会增加配置项以裁剪适合这种场景。

**如果你是高手，请不要使用。**

一个典型的应用场景是，串口的 RS485 通信。

# 🩼副作用
>**延迟**: 会带来收发数据 1~2ms 的额外延迟，主要是确保切换半双工方向的稳定性。
>
>**资源**: 会加倍 RAM 空间的消耗。

对于小白而言，这不算什么。

# 🧺思考方向

或者仅参考思路来优化自己的程序。本库仅是以下思路的一个实践，或许带有作者的个人偏见。

### DMA 的问题
DMA 接收是非常简单且易用的，但是发送的设计却比较复杂，因为在实际应用中，发送数据的来源通常是杂乱的，在程序各个部分都可能调用发送API。

而调用时发送 DMA 通道还没有结束，就会比较尴尬。大多数菜鸟的用法是阻塞等待，那就完全丧失 DMA 的优越性了。

我早期的做法是，采用一个环形 buffer 来缓存发送数据，并定期将其搬运到一个线性 buffer 来开启发送 DMA(省略具体条件)，这样当然是非常稳健的。

如果有过使用 DMA 接收采样数据并统一处理的实践经验，会了解到 AB buffer 的用法。有 AB 两个buffer，同一时间只有一个会接收数据，而另一个此时装满了数据，可以进行统一的处理。这将会比环形 buffer 节省许多搬运。

或许更多人叫乒乓 buffer，我只是不喜欢这个叫法。

另外如果是带有 D-Cache 的 MCU 需要特别注意缓存一致性的问题，偶尔可能需要自己解决一下，可以参考例程中的方法。本库已经尽量使用 volatile 进行修饰，但是 hal 库对于 DMA 以及 Cache 仍然没有很好的处理。

那么发送是否也可以使用这样的做法呢？反正同时只能有一个 buffer 挂在 DMA 通道上。

### 半双工的问题
通常半双工通道需要**数十纳秒**到**数微秒**切换收发方向，1毫秒的延迟是必然稳健的。

半双工的通信与 DMA 其实是有相似性的，都需要将杂乱而来的数据集合起来，建立一段通道统一进行传输。所以将通信分两步进行是合适的。

而我认为使用半双工的场景。对于通信延迟或许就不在意了，重要的是稳健可靠的信息。例如我使用的 AMD2587 IC 发送机切换需要 2.5 μs，且在跑 modbusRTU ，这种使用场景下使用定时器进行绝对的电平控制是没有必要的，所以本库将建立一个收发数据 1ms 延迟切换的机制。

# 🤔如何使用

建议直接参考 [在stm32的示例](/Example/stm32H743/h7_main.c)

首先给库的对象变量预分配足够的内存空间。注意，这个内存空间提供给发送以及接收，而且需要双份。

```c
// 建议4字节对齐，否则 DMA 可能存在问题
#define T_LEN_MAX 256
#define R_LEN_MAX 256
/* 预分配内存区 */
uint8_t MEM[T_LEN_MAX * 2 + R_LEN_MAX * 2] __ALIGNED(32);
```

创建库对象变量，以及其使用的回调绑定变量。在这里，如果 .SendBefor 以及 .SendOver 绑定为 **NULL** ，那么将没有半双工通道切换的支持，没有延迟状态机的判断，仅有 DMA 的逻辑，也会稍微增强性能。

```c
/* Dataflow对象*/
_DFlow                  DFlow;
_DFLOW_COMMON_FUNCTION  DFlowFunc = {
    .Transmit         = Transmit485, 
    .Receive          = Receive485,
    .TransmitGetState = TransmitGetState485,
    .SendBefor        = SendBefor485,
    .SendOver         = SendOver485};
```

程序中进行对象的初始化

```c
    DFlow_Init(&DFlow, MEM, T_LEN_MAX, R_LEN_MAX, &DFlowFunc);
```

周期性地调用库 tick ，针对某个对象，如果有多个对象请一一调用。

```c
while(1)
{
    DFlow_Ticks(&DFlow);
    HAL_Delay(1);
}
```
在合适的中断处理，引向合适的库处理。详见示例。

>**发送结束中断**:  DFlow_Interrupt_TC(&DFlow);
>
>**空闲中断 & 接收结束中断 &原教旨DMA FTF中断(非hal)**: DFlow_Interrupt_IDLE_RC_FTF(&DFlow, Size);

然后就可以在程序任何地方随意地调用发送和接收啦

```c
//发送
DFlow_Write(&DFlow, "Hello3\n", sizeof("Hello3\n"));
//接收
while(DFlow_Getc(&DFlow, &data) == DFLOW_API_RETURN_DEFAULT)
{

}
```



# 各设计

buffer模型

![](https://cloudflare-imgbed-6qt.pages.dev/file/1735001474791_buffer%20%E6%A8%A1%E5%9E%8B.png)

中断处理

![](https://cloudflare-imgbed-6qt.pages.dev/file/1735001486438_%E4%B8%AD%E6%96%AD%E5%A4%84%E7%90%86.png)

轮询处理

![](https://cloudflare-imgbed-6qt.pages.dev/file/1735001486588_%E8%BD%AE%E8%AF%A2%E5%A4%84%E7%90%86.png)

