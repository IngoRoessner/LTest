cmake_minimum_required(VERSION 2.8.4)
project(LTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y  -pthread")

set(ExampleSource
    examples/async.h
    examples/ignores.h
    examples/leap_year.h
    examples/main.cpp
    examples/minimal.h
    examples/parametertest.h
    examples/resultset.h
    examples/call_private.h
    src/OutputFormat/rapidxml-1.13/rapidxml.hpp
    src/OutputFormat/rapidxml-1.13/rapidxml_iterators.hpp
    src/OutputFormat/rapidxml-1.13/rapidxml_print.hpp
    src/OutputFormat/rapidxml-1.13/rapidxml_utils.hpp
    src/OutputFormat/Format.h
    src/OutputFormat/OutputFormat.h
    src/OutputFormat/OutputFormatBase.h
    src/OutputFormat/TextOutput.h
    src/OutputFormat/TextTable.h
    src/OutputFormat/XmlOutput.h
    src/AsyncTaskExecuter.h
    src/FunctionPattern.h
    src/LTest.cpp
    src/LTest.h
    src/LTestAssert.h
    src/LTestMisuseException.h
    src/ManagedFixture.h
    src/MuteStream.h
    src/ParameterTest.h
    src/TestResult.h
    src/toStringPatch.h
    src/hackPrivate.h)

 set(SelfTestSource
     src/OutputFormat/rapidxml-1.13/rapidxml.hpp
     src/OutputFormat/rapidxml-1.13/rapidxml_iterators.hpp
     src/OutputFormat/rapidxml-1.13/rapidxml_print.hpp
     src/OutputFormat/rapidxml-1.13/rapidxml_utils.hpp
     src/OutputFormat/Format.h
     src/OutputFormat/OutputFormat.h
     src/OutputFormat/OutputFormatBase.h
     src/OutputFormat/TextOutput.h
     src/OutputFormat/TextTable.h
     src/OutputFormat/XmlOutput.h
     src/AsyncTaskExecuter.h
     src/FunctionPattern.h
     src/LTest.cpp
     src/LTest.h
     src/LTestAssert.h
     src/LTestMisuseException.h
     src/ManagedFixture.h
     src/MuteStream.h
     src/ParameterTest.h
     src/TestResult.h
     src/toStringPatch.h
     src/hackPrivate.h
     test-src/addTest.h
     test-src/chrono_lib_test.h
     test-src/run-ltest-tests.cpp)

add_executable(LTestExample ${ExampleSource})
add_executable(LTestSelfTest ${SelfTestSource})