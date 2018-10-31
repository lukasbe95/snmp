cmake_minimum_required(VERSION 3.12)
project(snmp)

set(CMAKE_CXX_STANDARD 14)

add_executable(snmp main.cpp parser.cpp parser.h structures.h ObjectId.cpp ObjectId.h ObjectType.cpp ObjectType.h DataType.cpp DataType.h)