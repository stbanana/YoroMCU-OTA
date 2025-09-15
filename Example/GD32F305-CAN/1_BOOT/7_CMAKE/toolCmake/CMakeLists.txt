cmake_minimum_required(VERSION 3.22)

set(CMAKE_PROJECT_NAME user_src) 

project(${CMAKE_PROJECT_NAME})
add_library(${CMAKE_PROJECT_NAME} INTERFACE)

# cmake FUNC
include(${CMAKE_CURRENT_LIST_DIR}/cmake_func/functions.cmake)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(${CMAKE_PROJECT_NAME} INTERFACE 
    USE_STDPERIPH_DRIVER
    # GD32F310
    GD32F30X_CL
    __TARGET_FPU_VFP
    ARM_MATH_CM4
    $<$<CONFIG:Debug>:DEBUG>
)

# 递归包含头文件
include_sub_directories_recursively(${CMAKE_CURRENT_LIST_DIR}/../../1_App)
include_sub_directories_recursively(${CMAKE_CURRENT_LIST_DIR}/../../4_Driver)

target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE
    ../../6_RTOS/Include
    ../../5_PhysicalChip/Cpu/Include
    ../../5_PhysicalChip/GD32F30x/CMSIS
    ../../5_PhysicalChip/GD32F30x/CMSIS/GD/GD32F30x/Include
    ../../5_PhysicalChip/GD32F30x/GD32F30x_standard_peripheral/Include
)

# 递归查找所有源码文件
file(GLOB_RECURSE 1_APP_SRC ${CMAKE_CURRENT_LIST_DIR}/../../1_App/*.c)
file(GLOB_RECURSE 4_DRIVER_SRC ${CMAKE_CURRENT_LIST_DIR}/../../4_Driver/*.c)
file(GLOB_RECURSE 5_DRIVER_SRC ${CMAKE_CURRENT_LIST_DIR}/../../5_PhysicalChip/GD32F30x/GD32F30x_standard_peripheral/Source/*.c)

target_sources(${CMAKE_PROJECT_NAME} PUBLIC
    ../../main.c
    ${1_APP_SRC}
    ${4_DRIVER_SRC}
    ${5_DRIVER_SRC}
    # CPU部分    
    ../../5_PhysicalChip/Cpu/GNU/syscalls.c
    ../../5_PhysicalChip/Cpu/Source/GD32F30x_it.c
    ../../5_PhysicalChip/GD32F30x/CMSIS/GD/GD32F30x/Source/system_GD32F30x.c    
    ../../5_PhysicalChip/GD32F30x/CMSIS/GD/GD32F30x/Source/GCC/startup_GD32F30x.s
)

target_link_directories(${CMAKE_PROJECT_NAME} INTERFACE

)

target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE 
    ${CMAKE_CURRENT_LIST_DIR}/../../5_PhysicalChip/GD32F30x/CMSIS/GD/arm_cortexM4lf_math.lib
)

if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


