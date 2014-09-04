/*
 *     The MIT License (MIT)
 *
 *     Copyright (c) 2014 Ingo Rössner
 *                        Dan Häberlein
 *
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 */


#ifndef TESTRESULT_H_INCLUDED
#define TESTRESULT_H_INCLUDED

#include <memory>
#include <algorithm>
#include <iterator>
#include <list>
#include "MuteStream.h"
#include "OutputFormat/OutputFormat.h"
#include "OutputFormat/Format.h"
typedef string testname;

enum TestState {
	OK, FAILED, ABORTED, IGNORED
};

class TestResult{
private:
    TestState _state;
    double _time_taken;
    map<ostream*, string> _output_mapping;
    testname _tname;

    template <typename K, typename V>
    V get_from_map_with_default(const  std::map <K,V> & m, const K & key, const V & _default ) const {
       typename std::map<K,V>::const_iterator it = m.find( key );
       if ( it == m.end() ) {
          return _default;
       }
       else {
          return it->second;
       }
    }

public :

    TestResult(testname tname = "no_testname_given") : _state(TestState::IGNORED), _time_taken(0), _tname(tname) {}

    TestResult(TestState state, double time_taken, MuteStreamMap& muteStream, testname tname)
        : _state(state), _time_taken(time_taken), _output_mapping(move(muteStream.flush(tname, state != TestState::OK))), _tname(tname) {}

    TestState get_state() const
    {
        return _state;
    }

    double get_time_taken() const
    {
        return _time_taken;
    }

    map<ostream*, string> get_output_mapping() const
    {
        return _output_mapping;
    }

    string get_system_out() const {
    	return get_from_map_with_default(_output_mapping, &cout, string(""));
    }

    string get_system_err() const {
        return get_from_map_with_default(_output_mapping, &cerr, string(""));
    }

    string get_testname() const {
    	return _tname;
    }

    virtual ~TestResult(){};

};

class TestResultOK: public TestResult{
public :
    TestResultOK(testname tname = "no_testname_given") : TestResult(tname) {}

    TestResultOK(testname tname, MuteStreamMap muteStream, double time_taken):TestResult(TestState::OK, time_taken, muteStream, tname){}

    static constexpr TestState expectedState = TestState::OK;
};

class TestResultIgnored: public TestResult{
public :
    TestResultIgnored(testname tname = "no_testname_given") : TestResult(tname) {}
    static constexpr TestState expectedState = TestState::IGNORED;
};

class TestResultFailed: public TestResult{
    string message;
public :
    TestResultFailed(testname tname = "no_testname_given") : TestResult(tname) {}

    TestResultFailed(testname tname, MuteStreamMap muteStream, double time_taken, string msg = ""):TestResult(TestState::FAILED, time_taken, muteStream, tname), message(msg){}

    static constexpr TestState expectedState = TestState::FAILED;

    string getMessage(){
        return message;
    }
};

class TestResultAborted: public TestResult{
    string message;
public :
    TestResultAborted(testname tname = "no_testname_given") : TestResult(tname) {}

    TestResultAborted(testname tname, MuteStreamMap muteStream, double time_taken, string msg=""):TestResult(TestState::ABORTED, time_taken, muteStream, tname), message(msg){}

    static constexpr TestState expectedState = TestState::ABORTED;

    string getMessage(){
        return message;
    }
};

template<template<typename> class ResultSetTemplate, typename ResultType>
class ResultSetMixin:public list<shared_ptr<ResultType>>{
public:
    string out(Format format = Format::Text){
        GetOutputFormat<ResultSetMixin<ResultSetTemplate,ResultType>> output(format);
        return output.run(*this);
    }

    ResultSetTemplate<ResultType> getSubSet(function<bool(shared_ptr<ResultType>)> pred){
        ResultSetTemplate<ResultType> subset;
        copy_if(this->begin(), this->end(), back_inserter(subset), pred);
        return subset;
    }

    ResultSetTemplate<ResultType> getSubSetByState(TestState state){
        return getSubSet([&](shared_ptr<ResultType> ptr){return ptr->get_state() == state;});
    }

    ResultSetTemplate<TestResultIgnored> getIgnores(){
        return *this;
    }

    ResultSetTemplate<TestResultOK> getOK(){
        return *this;
    }

    ResultSetTemplate<TestResultFailed> getFails(){
        return *this;
    }

    ResultSetTemplate<TestResultAborted> getAborts(){
        return *this;
    }

    double getTotalExecutionTimeInSeconds(){
    	double total = 0.0;
    	for_each(this->begin(), this->end(), [&total](shared_ptr<ResultType> element){total += element->get_time_taken();});
    	return total;
    }
};

template<typename ResultType>
class ResultSet:public ResultSetMixin<ResultSet, ResultType>{
public:
    template<typename OtherResultSetType>
    ResultSet(OtherResultSetType other){
        for(auto& element : other.getSubSetByState(ResultType::expectedState)){
            this->push_back(dynamic_pointer_cast<ResultType>(element));
        }
    }

    ResultSet(){}
};

template<>
class ResultSet<TestResult>:public ResultSetMixin<ResultSet, TestResult>{
public:
    template<typename OtherResultSetType>
    ResultSet(OtherResultSetType other){
        for(auto& element : other){
            this->push_back(element);
        }
    }

    ResultSet(){}
};

using TestResultSet = ResultSet<TestResult>;


#endif // TESTRESULT_H_INCLUDED
