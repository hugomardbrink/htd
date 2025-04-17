cmake_minimum_required(VERSION 3.21)

project(htd C)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_C_EXTENSIONS NO)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    
include(GNUInstallDirs) 

file(GLOB HTD_SOURCES
    src/data_structure/*.c
)

add_library(htd STATIC ${HTD_SOURCES})
target_include_directories(htd PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_options(htd PRIVATE -Wall -Wextra -Werror)

install(TARGETS htd
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/htd
    FILES_MATCHING PATTERN "*.h"
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
)
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

set(CMAKE_CTEST_VERBOSE TRUE)
enable_testing()

file(GLOB TEST_SOURCES tests/test_*.c)

foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} PRIVATE htd)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
