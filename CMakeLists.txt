cmake_minimum_required (VERSION 3.16.5)
project (Quar)

include_directories (include)
file (GLOB SOURCES "src/*.cpp")

add_executable (quar ${SOURCES})