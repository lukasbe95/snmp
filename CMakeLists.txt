cmake_minimum_required(VERSION 3.12)
project(snmp)

set(CMAKE_CXX_STANDARD 14)

include_directories(tests/lib/googletest/googletest/include tests/lib/googletest/googlemock/include)

set(SOURCE_FILES
        parser_mib/parser.cpp
        parser_mib/parser.h
        parser_mib/structures.h
        parser_mib/ObjectId.cpp
        parser_mib/ObjectId.h
        parser_mib/ObjectType.cpp
        parser_mib/ObjectType.h
        parser_mib/DataType.cpp
        parser_mib/DataType.h
        ber_coder/BerCoder.cpp
        ber_coder/BerCoder.h
        ber_coder/SimpleType.cpp
        ber_coder/SimpleType.h
        ber_coder/Type.h
        ber_coder/ConstructedType.cpp
        ber_coder/ConstructedType.h
        ber_decoder/BerEncoder.cpp
        ber_decoder/BerEncoder.h
        ber_decoder/Decoded.cpp
        ber_decoder/Decoded.h
        pdu/SnmpMessage.h
        pdu/SnmpMessage.cpp
        )

set(TEST_FILES
        tests/ber_decoder_tests/decoder.cpp
        tests/ber_decoder_tests/main_test.cpp)

add_subdirectory(tests/lib/googletest)
link_libraries(gmock)

add_executable(snmp_run ${SOURCE_FILES} main.cpp)

add_executable(snmp_test ${SOURCE_FILES} ${TEST_FILES})


#standard C++ 14
#set(CMAKE_CXX_STANDARD 14)
#
#include_directories(include googletest/googletest/include googletest/googlemock/include)
#
#set(SOURCE_FILES
#        src/matlab.cpp)
#
#set(TEST_SOURCES
#        test/main_test.cpp      #plik z main() dla testów
#        test/test_vector.cpp)
#
#add_subdirectory(googletest)
#link_libraries(gmock)
#
#add_executable(matlab_2_run ${SOURCE_FILES} matlab-2.cpp)
#
#add_executable(matlab2_test ${SOURCE_FILES} ${TEST_SOURCES})