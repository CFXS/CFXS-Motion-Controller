# CFXS-Base
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/CFXS-Base")
target_link_libraries(${EXE_NAME} PUBLIC CFXS_Base)

# CFXS-HW
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/CFXS-HW")
target_include_directories(CFXS_HW PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/CFXS-Base/include")
target_include_directories(CFXS_HW PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/tm4c-driverlib")
target_link_libraries(${EXE_NAME} PUBLIC CFXS_HW)

# CFXS-CNC
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/CFXS-CNC")
target_include_directories(CFXS_CNC PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/CFXS-Base/include")
target_link_libraries(${EXE_NAME} PUBLIC CFXS_CNC)

# tm4c-driverlib
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/tm4c-driverlib")
target_link_libraries(${EXE_NAME} PUBLIC tm4c_driverlib)

# SeggerRTT
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/SeggerRTT-printf")
target_link_libraries(${EXE_NAME} PUBLIC printf_impl_SeggerRTT)