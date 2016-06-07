cmake_minimum_required (VERSION 3.4)
project (io.java)
add_executable(io.java Main.cpp )

# The verson number.
set (IO_JAVA_VERSION_MAJOR 1)
set (IO_JAVA_VERSION_MINOR 0)
set (UV_HEADER_PATH "/usr/local/include/"
    CACHE PATH "The path to libuv framework")

# configure a header file to pass some of the CMake settings
# to the source code

configure_file (
    "${PROJECT_SOURCE_DIR}/IO_JAVAConfig.h.in"
	"${PROJECT_BINARY_DIR}/IO_JAVAConfig.h"
)
# add the binary tree to the search path for include files
# so that we will find IO_JAVAConfig.h

include_directories(SYSTE ${UV_HEADER_PATH})
IF(APPLE)
include_directories("${PROJECT_BINARY_DIR}" "/System/Library/Frameworks/JavaVM.framework/Versions/Current/Headers")


FIND_LIBRARY(jvm libjvm.dylib /System/Library/Frameworks/JavaVM.framework/Versions/CurrentJDK/Libraries)

ADD_LIBRARY(libuv STATIC IMPORTED)
SET_TARGET_PROPERTIES(libuv PROPERTIES IMPORTED_LOCATION /usr/local/lib/libuv.a)

ADD_LIBRARY(jvm SHARED IMPORTED)
SET_TARGET_PROPERTIES(jvm PROPERTIES IMPORTED_LOCATION /System/Library/Frameworks/JavaVM.framework/Versions/Current/JavaVM) 

ENDIF (APPLE)

add_library(javaio SHARED Env.cpp io_java_io_File.cpp io_java_io_Net.cpp)
target_link_libraries(javaio libuv jvm)

target_link_libraries(io.java libuv jvm javaio)
