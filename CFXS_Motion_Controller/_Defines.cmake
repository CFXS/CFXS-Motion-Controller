# Target
add_compile_definitions("PART_TM4C1294NCPDT")
add_compile_definitions("TARGET_IS_TM4C129_RA2")
add_compile_definitions("CFXS_PLATFORM_TM4C")
add_compile_definitions("CFXS_DEVICE_CLASS_TM4C129X")
add_compile_definitions("CFXS_CPU_CLOCK_FREQUENCY=120000000")

# Project

add_compile_definitions("CFXS_PROJECT_NAME=\"CFXS Motion Controller\"")
add_compile_definitions("CFXS_PROJECT_VERSION_STRING=\"1.0.0-dev\"")

add_compile_definitions("SEGGER_DONT_REDEFINE_PRINTF") # custom printf lib, dont let segger redirect directly

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_compile_definitions("DEBUG")
    add_compile_definitions("CFXS_DEBUG")
    add_compile_definitions("CFXS_DEBUG_ASSERT")
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_compile_definitions("RELEASE")
    add_compile_definitions("NDEBUG")
endif()