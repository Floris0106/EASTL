#-------------------------------------------------------------------------------------------
# Copyright (C) Electronic Arts Inc.  All rights reserved.
#-------------------------------------------------------------------------------------------
file(GLOB EATEST_SOURCES "source/*.cpp")
add_library(EATest ${EATEST_SOURCES})

target_include_directories(EATest PUBLIC include)

target_link_libraries(EATest EABase)
target_link_libraries(EATest EAStdC)
target_link_libraries(EATest EASTL)
target_link_libraries(EATest EAMain)
target_link_libraries(EATest EAThread)