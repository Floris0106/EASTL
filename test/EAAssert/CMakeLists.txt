#-------------------------------------------------------------------------------------------
# Copyright (C) Electronic Arts Inc.  All rights reserved.
#-------------------------------------------------------------------------------------------
file(GLOB EAASSERT_SOURCES "source/*.cpp")
add_library(EAAssert ${EAASSERT_SOURCES})

target_include_directories(EAAssert PUBLIC include)

target_link_libraries(EAAssert EABase)