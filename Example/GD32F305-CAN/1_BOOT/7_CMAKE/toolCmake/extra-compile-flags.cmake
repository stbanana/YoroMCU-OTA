# 递归查找6_Rtos和7_Exlib文件夹下所有源码文件
# file(GLOB_RECURSE RTOS_SRC ${CMAKE_CURRENT_LIST_DIR}/../../6_Rtos/*.c)
# file(GLOB_RECURSE EXLIB_SRC ${CMAKE_CURRENT_LIST_DIR}/../../7_Exlib/*.c)
# get_target_property(src_list ${CMAKE_PROJECT_NAME} SOURCES)

# # 调用函数设置编译标志
# foreach(src_file IN LISTS RTOS_SRC)
#     set_source_files_properties(${src_file} PROPERTIES COMPILE_FLAGS "-w")
# endforeach()
# foreach(src_file IN LISTS EXLIB_SRC)
#     set_source_files_properties(${src_file} PROPERTIES COMPILE_FLAGS "-w")
# endforeach()

include(${CMAKE_CURRENT_LIST_DIR}/cmake_func/functions.cmake)

# set_compile_flags_for_matching_files(${CMAKE_PROJECT_NAME} "6_Rtos|7_Exlib" "-w")
set_compile_flags_for_matching_files(user_src "DFlowUser.c" "-O0")
# set_compile_flags_for_matching_files(user_src "5_PhysicalChip" "-O1 -g0")
# set_compile_flags_for_matching_files(user_src "1_App" "-O1 -g0")
