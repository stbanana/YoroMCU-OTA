<p align="center">
 <h1 align="center">ReturnCodesC</h1>
 <p align="center">一个通用的函数返回代码值的集合</p>
</p>
[stbanana/ReturnCodesC: A series of common C/C++ function return values.](https://github.com/stbanana/ReturnCodesC)

<p align="center">
    <a href="https://github.com/anuraghazra/github-readme-stats/graphs/contributors"><img alt="C/C++" src="https://img.shields.io/badge/C/C++-00599C.svg?logo=c&logoColor=white" /></a>
    <a href="https://mit-license.org/"><img alt="Static Badge" src="https://img.shields.io/badge/license%20-%20MIT%203.0%20-%20blue" /></a>
</p>
<p align="center">
    <a href="/README.md">简体中文</a>
    ·
    <a href="/README_EN.md">English</a>
</p>


## ✨特性

> 1. 🔢**0开销**：宏定义数值库，完全的0开销
> 2. 🐜**资源受限友好**：常见的返回代码值都在8位以内，便于 uint8 最小资源开销的函数编写
> 3. 📄**多语言**：以 `_RERURNCC_LANGUAGE_`  宏定义语言，便于语义解析直接跳转到你习惯的语言
> 4. 🙋‍♂️**应用广**：覆盖大多数的应用场景，使得整个工程都拥有相对统一的返回值

对 ST hal 库的返回枚举含义兼容。

## 🤔示例

```c
/* 先定义自己的语言版本，或者在编译器/编辑器全局定义
 * 0: English
 * 1: 中文
 * 2: 正體中文
 */
#define _RERURNCC_LANGUAGE_ 1
#include <ReturnCodesC.h>

/* 最简函数示例 */
uint8_t IsValueEqual(uint32_t a, uint32_t b)
{
    if(a != b)
        return RETURN_ERROR;
    
    return RETURN_DEFAULT;
}

/* 结构体或变量操作示例，以指针传参 */
typedef struct
{                      
    uint8_t Buffer[20];
    uint8_t Head;
    uint8_t Tail;
} BufferDef;

uint32_t  EnBuffer(BufferDef *pd, uint8_t data)
{
    /* 闯关式检查 */
    if(pd->Tail == ((pd->Head + 1) % 20)) 
        return RETURN_BUFFER_FULL;        // buffer 已满，返回非0状态
    // 后续发现有调用异常，可以再添加其他异常条件，例如
    /*
    if(pd == NULL)
    	return RETURN_ERR_PARAM;
    */
    
    /* 实际功能逻辑 */
    pd->Buffer[pd->Head] = data;
    pd->Head ++;
    pd->Head %= 20;
    
    /* 常态返回 */
    return RETURN_DEFAULT;
}
```



## 👁️我的观点

在 C/C++ 程序中，函数负责实现功能，而功能的实现应该无副作用。一个完备的函数应该能处理所有预期的输入，并与输入对应的唯一输出。

当然在实际项目中完全地做到这一点相当困难，所以我的观点是所有的函数必须有返回值，而且这个返回值反映了函数运转的状态。如此在调用这个函数时，我们可以先以完全正常的返回值判断出一切顺利的情况，并编写最基础的项目框架，后续填充异常产生和异常处理都会相对容易。 

在一个项目中可能整合很多的库，他们都会有自己的 API 标准返回值，这些我们无法把控也无法修改。但是在自己编写的业务流程和中间件中，可以使用本库实现项目统一的函数状态码体系。