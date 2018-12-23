cmake_minimum_required(VERSION 3.12)
project(snmp)

set(CMAKE_CXX_STANDARD 14)

add_executable(snmp main.cpp parser_mib/parser.cpp parser_mib/parser.h parser_mib/structures.h parser_mib/ObjectId.cpp parser_mib/ObjectId.h parser_mib/ObjectType.cpp parser_mib/ObjectType.h parser_mib/DataType.cpp parser_mib/DataType.h ber_coder/BerCoder.cpp ber_coder/BerCoder.h ber_coder/SimpleType.cpp ber_coder/SimpleType.h ber_coder/Type.h ber_coder/ConstructedType.cpp ber_coder/ConstructedType.h)