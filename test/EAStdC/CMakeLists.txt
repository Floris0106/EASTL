#-------------------------------------------------------------------------------------------
# Copyright (C) Electronic Arts Inc.  All rights reserved.
#-------------------------------------------------------------------------------------------
file(GLOB EASTDC_SOURCES "source/*.cpp")
add_library(EAStdC ${EASTDC_SOURCES})

target_include_directories(EAStdC PUBLIC include)

target_link_libraries(EAStdC EABase)
target_link_libraries(EAStdC EAAssert)
target_link_libraries(EAStdC EAThread)