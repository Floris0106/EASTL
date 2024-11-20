#-------------------------------------------------------------------------------------------
# Copyright (C) Electronic Arts Inc.  All rights reserved.
#-------------------------------------------------------------------------------------------
include(CTest)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-pointer-bool-conversion -Wno-unknown-warning-option")
endif()

if (EASTL_NO_CHAR8T_FLAG)
    add_compile_options(${EASTL_NO_CHAR8T_FLAG})
endif()

file(GLOB EASTLTEST_SOURCES "src/*.cpp")
file(GLOB EASTLTEST_HEADERS "src/*.inl" "src/*.h")
set(SOURCES ${EASTLTEST_SOURCES} ${EASTLTEST_HEADERS})

if (EASTL_CHAR8T_FLAG)
    message(STATUS "Building with char8_t support in tests.")
    set_source_files_properties("src/TestString.cpp" "src/TestStringView.cpp" PROPERTIES
        COMPILE_FLAGS ${EASTL_CHAR8T_FLAG}
        COMPILE_DEFINITIONS "EASTL_EXPECT_CHAR8T_SUPPORT"
    )
endif()

add_executable(EASTLTest ${SOURCES})

target_include_directories(EASTLTest PUBLIC include)

add_subdirectory(EABase)
add_subdirectory(EAAssert)
add_subdirectory(EAStdC)
add_subdirectory(EAMain)
add_subdirectory(EATest)
add_subdirectory(EAThread)

target_link_libraries(EASTLTest EABase)
target_link_libraries(EASTLTest EAAssert)
target_link_libraries(EASTLTest EAStdC)
target_link_libraries(EASTLTest EAMain)
target_link_libraries(EASTLTest EATest)
target_link_libraries(EASTLTest EAThread)
target_link_libraries(EASTLTest EASTL)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

if((NOT APPLE) AND (NOT WIN32))
    target_link_libraries(EASTLTest ${EASTLTest_Libraries} Threads::Threads rt)
else()
    target_link_libraries(EASTLTest ${EASTLTest_Libraries} Threads::Threads)
endif()

add_test(EASTLTestRuns EASTLTest)
set_tests_properties(EASTLTestRuns PROPERTIES PASS_REGULAR_EXPRESSION "RETURNCODE=0")