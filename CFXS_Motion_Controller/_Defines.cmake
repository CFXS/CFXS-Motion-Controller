# Target
add_compile_definitions("PART_TM4C1294NCPDT")
add_compile_definitions("TARGET_IS_TM4C129_RA2")
add_compile_definitions("CFXS_PLATFORM_TM4C")
add_compile_definitions("CFXS_CPU_CLOCK_FREQUENCY=120000000")

# Project

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_compile_definitions("DEBUG")
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_compile_definitions("RELEASE")
    add_compile_definitions("NDEBUG")
endif()