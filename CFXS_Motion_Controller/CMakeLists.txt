include("${PROJECT_SOURCE_DIR}/CMake/Target/CortexM4F.cmake")
include("_Sources.cmake")

set(EXE_NAME CFXS_Motion_Controller)

set(LINKER_SCRIPT "\"${CMAKE_CURRENT_SOURCE_DIR}/LinkerScript.ld\"")
Target_CortexM4F(${EXE_NAME})

set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/${EXE_NAME}/vendor)
 
message(${CPU_OPTIONS})

add_compile_options(
    ${CPU_OPTIONS}
    $<$<COMPILE_LANGUAGE:CXX>:-std=c++17>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-Wsuggest-override>
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-old-style-cast>

    -fsingle-precision-constant # 2.3/2.3f are both floats
    
    -Wl,--gc-sections # remove unused sections
    -Wl,--print-gc-sections

    #-Wstack-usage=x
    #-fstack-usage

    -Wall
    -Wextra
    -Wcast-align
    -Wsign-conversion
    -Wshadow
    -Wlogical-op
    -Wsuggest-final-types
    -Wsuggest-final-methods
    -Wno-unused-parameter
    -Wno-unused-function
    -Wno-sign-conversion
    -Wno-narrowing
    -Wno-sign-compare
    -Wno-shift-negative-value
    -Wno-cast-align
    -Wno-write-strings
    -Wno-tautological-compare

    $<$<COMPILE_LANGUAGE:CXX>:-Wno-literal-suffix>
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-register>
    $<$<COMPILE_LANGUAGE:CXX>:-fpermissive>
    
    -fdebug-prefix-map="${CMAKE_SOURCE_DIR}"=.
)

add_executable(${EXE_NAME} ${sources} ${headers})

include("_IncludeDirectories.cmake")
include("_Defines.cmake")
include("_Modules.cmake")

target_precompile_headers(
    ${EXE_NAME}
    PUBLIC
    <stddef.h>
    <stdint.h>
    <stdio.h>
    <stdlib.h>
    <stdbool.h>
    <string.h>
    <new>
)

target_link_options(
    ${EXE_NAME}
    PRIVATE
    ${CPU_OPTIONS}
    SHELL:-T${LINKER_SCRIPT}
    -Wl,-Map=output.map,--cref
)

set_target_properties(${EXE_NAME} PROPERTIES SUFFIX ".elf")

add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    COMMAND ${OBJCOPY} -S -O binary ${CMAKE_CURRENT_SOURCE_DIR}/../build/${EXE_NAME}/${EXE_NAME}.elf ${CMAKE_CURRENT_SOURCE_DIR}/../build/${EXE_NAME}.bin)
add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    COMMAND ${NM} -C -l --print-size --size-sort --format=bsd --radix=x ${CMAKE_CURRENT_SOURCE_DIR}/../build/${EXE_NAME}/${EXE_NAME}.elf > ${CMAKE_CURRENT_SOURCE_DIR}/../build/${EXE_NAME}_Symbols.txt)