<p align="center">
 <h1 align="center">ReturnCodesC</h1>
 <p align="center">A Collection of Universal Function Return Codes</p>
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


## ✨Features

> 1. 🔢**Zero Overhead**: Macro-defined value library with zero overhead
> 2. 🐜**Resource-Constrained Friendly**: Common return codes are within 8 bits, facilitating functions with minimal resource overhead using uint8
> 3. 📄**Multi-Language Support**: Use the `_RERURNCC_LANGUAGE_` macro to define the language, making it easy to jump to your preferred language for semantic parsing
> 4. 🙋‍♂️**Wide Application**: Covers most application scenarios, providing a relatively unified return value system for the entire project

Compatible with the return enumeration meanings of the ST HAL library.

## 🤔示例

```c
/* First define your language version, or define it globally in the compiler/editor
 * 0: English
 * 1: Chinese
 * 2: Traditional Chinese
 */
#define _RERURNCC_LANGUAGE_ 1
#include <ReturnCodesC.h>

/* Simplest function example */
uint8_t IsValueEqual(uint32_t a, uint32_t b)
{
    if(a != b)
        return RETURN_ERROR;
    
    return RETURN_DEFAULT;
}

/* Structure or variable operation example, passing by pointer */
typedef struct
{                      
    uint8_t Buffer[20];
    uint8_t Head;
    uint8_t Tail;
} BufferDef;

uint32_t EnBuffer(BufferDef *pd, uint8_t data)
{
    /* Checkpoint-style checks */
    if(pd->Tail == ((pd->Head + 1) % 20)) 
        return RETURN_BUFFER_FULL;        // Buffer is full, return non-zero status
    // If any calling anomalies are found later, additional exception conditions can be added, for example
    /*
    if(pd == NULL)
        return RETURN_ERR_PARAM;
    */
    
    /* Actual functional logic */
    pd->Buffer[pd->Head] = data;
    pd->Head++;
    pd->Head %= 20;
    
    /* Normal return */
    return RETURN_DEFAULT;
}
```



## 👁️My Perspective

In C/C++ programs, functions are responsible for implementing functionality, and the implementation of functionality should have no side effects. A complete function should be able to handle all expected inputs and correspond to a unique output for each input.


Of course, achieving this completely in actual projects is quite challenging, so my view is that all functions must have a return value, and this return value should reflect the operational status of the function. This way, when calling the function, we can first determine that everything is fine with a completely normal return value and write the most basic project framework. Subsequently, adding exception generation and handling will be relatively easy.


In a project, many libraries may be integrated, each with their own API standard return values, which we cannot control or modify. However, in the business processes and middleware you write yourself, you can use this library to achieve a unified function status code system for the project.