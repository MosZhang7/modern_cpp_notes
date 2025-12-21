cmake_minimum_required(VERSION 3.10.0)
project(cn VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp")

list(FILTER CPP_FILES EXCLUDE REGEX
        ".*/duplicates.cpp$|.*/name_duplicates.cpp$"
    )

foreach(cpp_file ${CPP_FILES})
    # 3. 从文件名中提取可执行文件名（不带路径、不带后缀）
    get_filename_component(target_name ${cpp_file} NAME_WE)

    # 4. 生成可执行文件
    add_executable(${target_name} ${cpp_file})
endforeach()

include(CTest)
enable_testing()

