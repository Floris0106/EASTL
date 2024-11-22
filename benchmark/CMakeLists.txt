#-------------------------------------------------------------------------------------------
# Copyright (C) Electronic Arts Inc.  All rights reserved.
#-------------------------------------------------------------------------------------------
include(CTest)

include_directories(src)
include_directories(../test/src)

# Libstdc++ calls new internally, since DLLs have no weak symbols, runtime symbol resolution fails and EASTL's new is not called.
# Linking against static libstdc++ fixes this.
# See https://github.com/electronicarts/EASTL/issues/40 for more info.
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND MINGW)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -static-libstdc++")
    set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} -static-libstdc++")
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -static-libstdc++")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_BUILD_TYPE MATCHES "MinSizeRel" AND MINGW)
    message(FATAL_ERROR "FIXME: MinSizeRel on MingW-w64's Clang fails to link.")
endif()

if (EASTL_NO_CHAR8T_FLAG)
    add_compile_options(${EASTL_NO_CHAR8T_FLAG})
endif()

file(GLOB EASTLBENCHMARK_SOURCES "src/*.cpp")
file(GLOB EASTLBENCHMARK_HEADERS "src/*.h")
set(SOURCES ${EASTLBENCHMARK_SOURCES} ${EASTLBENCHMARK_HEADERS} "../test/src/EASTLTest.cpp")

add_definitions(-D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)

add_executable(EASTLBenchmarks ${SOURCES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(EASTLBenchmarks
    EAAssert
    EAMain
    EAThread
    EAStdC
    EASTL
    EATest
    Threads::Threads
)

add_test(EASTLBenchmarkRuns EASTLBenchmarks)
set_tests_properties(EASTLBenchmarkRuns PROPERTIES PASS_REGULAR_EXPRESSION "RETURNCODE=0")