#-------------------------------------------------------------------------------------------
# Copyright (C) Electronic Arts Inc.  All rights reserved.
#-------------------------------------------------------------------------------------------
file(GLOB EAMAIN_SOURCES "src/*.cpp" "src/internal/*.cpp")
add_library(EAMain ${EAMAIN_SOURCES})

add_definitions(-DEAMAIN_NETWORK_CHANNEL_PORT=8080)

target_include_directories(EAMain PUBLIC include)
target_include_directories(EAMain PUBLIC src)

target_link_libraries(EAMain EABase)
target_link_libraries(EAMain EAAssert)
target_link_libraries(EAMain EAStdC)