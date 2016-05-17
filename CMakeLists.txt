cmake_minimum_required (VERSION 2.6)
project (io.java)
add_executable(io.java Main.cpp )

# The verson number.
set (IO_JAVA_VERSION_MAJOR 1)
set (IO_JAVA_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code

configure_file (
    "${PROJECT_SOURCE_DIR}/IO_JAVAConfig.h.in"
	"${PROJECT_BINARY_DIR}/IO_JAVAConfig.h"
)
include_directories("/usr/local/include")
# add the binary tree to the search path for include files
# so that we will find IO_JAVAConfig.h

include_directories("${PROJECT_BINARY_DIR}" "/System/Library/Frameworks/JavaVM.framework/Versions/Current/Headers")

link_directories("/usr/local/lib" "/System/Library/Frameworks/JavaVM.framework/Versions/CurrentJDK/Libraries")

FIND_LIBRARY(event libevent.a /usr/local/lib)
#FIND_LIBRARY(jvm libjvm.dylib /System/Library/Frameworks/JavaVM.framework/Versions/CurrentJDK/Libraries)

add_library(libiojava Env.h Env.cpp  io_java_lang_System.h)

target_link_libraries(io.java event)
