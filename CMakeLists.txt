cmake_minimum_required(VERSION 3.16)

project(unicpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gather all .cpp source files
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/unicpp/*.cpp"
)

# Create the static library
add_library(unicpp STATIC ${SRC_FILES})

target_include_directories(unicpp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Common compile definitions
target_compile_definitions(unicpp PRIVATE
    PROG_THD_CNT_MULTI
    UCPP_MAIN_TYPE_STD
)

# Conditional per build type
target_compile_definitions(unicpp PRIVATE
    $<$<CONFIG:Release>:PROG_BUILD_TYPE_RELEASE>
    $<$<CONFIG:Debug>:PROG_BUILD_TYPE_DBG>
)

# Platform-specific settings
if(WIN32)
    message(STATUS "Configuring for Windows")
elseif(UNIX)
    message(STATUS "Configuring for Linux / Unix")
    target_compile_options(unicpp PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()
