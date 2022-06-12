cmake_minimum_required(VERSION 3.18)
project(scanner)

set(CMAKE_CXX_STANDARD 17)

add_executable(scanner main.cpp lib/utils.cpp lib/utils.h lib/scanner.cpp lib/scanner.h)
