#-------------------------------------------------------------------------------------------
# Copyright (C) Electronic Arts Inc.  All rights reserved.
#-------------------------------------------------------------------------------------------
file(GLOB EATHREAD_SOURCES "src/*.cpp")
add_library(EAThread ${EATHREAD_SOURCES})

target_include_directories(EAThread PUBLIC include)

target_link_libraries(EAThread EABase)