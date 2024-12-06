cmake_minimum_required(VERSION 3.20)
project(wgslpack)

option(WGSLPACK_ENABLE_COMPRESSION "Enable ZSTD compression support" OFF)
option(WGSLPACK_ENABLE_VERIFICATION "Enable verification support" OFF)
option(WGSLPACK_ENABLE_TESTS "Enable tests" OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES 
    WgslMinifier.cpp 
    tint_headers.h 
    wgsl_test.h
)

include_directories(${PROJECT_SOURCE_DIR}/lib)

find_program(IWYU_PATH NAMES include-what-you-use iwyu)
if(IWYU_PATH)
    set_property(TARGET wgslpack
                PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PATH})
endif()

if(WGSLPACK_ENABLE_COMPRESSION)
    target_compile_definitions(wgslpack PRIVATE WGSLPACK_USE_COMPRESSION)
endif()

if(WGSLPACK_ENABLE_VERIFICATION)
    target_compile_definitions(wgslpack PRIVATE WGSLPACK_ENABLE_VERIFICATION)
endif()

if(WGSLPACK_ENABLE_TESTS)
    target_compile_definitions(wgslpack PRIVATE WGSLPACK_ENABLE_TESTS)
endif()

if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    add_executable(wgslpack ${SOURCES})
    
    target_link_options(wgslpack PRIVATE
        -sEXPORTED_RUNTIME_METHODS=["ccall","cwrap"]
        -sALLOW_MEMORY_GROWTH=1
        -sEXPORT_NAME="WGSLPack"
        -sEXPORT_ES6=1 -sMODULARIZE=1
        -lembind
    )
else()
    add_executable(wgslpack ${SOURCES})
endif()