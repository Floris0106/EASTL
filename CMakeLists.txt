#-------------------------------------------------------------------------------------------
# Copyright (C) Electronic Arts Inc.  All rights reserved.
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)

project(EASTL CXX)

option(EASTL_STD_ITERATOR_CATEGORY_ENABLED "Enable compatibility with std::iterator categories" OFF)

include(CheckCXXCompilerFlag)

check_cxx_compiler_flag("-fchar8_t" EASTL_HAS_FCHAR8T_FLAG)
check_cxx_compiler_flag("/Zc:char8_t" EASTL_HAS_ZCCHAR8T_FLAG)

if(EASTL_HAS_FCHAR8T_FLAG)
    set(EASTL_CHAR8T_FLAG "-fchar8_t")
    set(EASTL_NO_CHAR8T_FLAG "-fno-char8_t")
elseif(EASTL_HAS_ZCCHAR8T_FLAG)
    set(EASTL_CHAR8T_FLAG "/Zc:char8_t")
    set(EASTL_NO_CHAR8T_FLAG "/Zc:char8_t-")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive-")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
endif()

file(GLOB EASTL_SOURCES "src/*.cpp")
file(GLOB_RECURSE EASTL_HEADERS "include/**.h")
add_library(EASTL ${EASTL_SOURCES} ${EASTL_HEADERS})

add_definitions(-D_CHAR16T)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_SCL_SECURE_NO_WARNINGS)
if (EASTL_STD_ITERATOR_CATEGORY_ENABLED)
    add_definitions(-DEASTL_STD_ITERATOR_CATEGORY_ENABLED=1)
endif()

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(benchmark)
    add_subdirectory(test)
endif()

target_include_directories(EASTL PUBLIC include)