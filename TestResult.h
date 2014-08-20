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

class TestResult
{
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
};

class TestResultIgnored: public TestResult{
public :
    TestResultIgnored(testname tname = "no_testname_given") : TestResult(tname) {}
};

class TestResultFailed: public TestResult{
    string message;
public :
    TestResultFailed(testname tname = "no_testname_given") : TestResult(tname) {}

    TestResultFailed(testname tname, MuteStreamMap muteStream, double time_taken, string msg = ""):TestResult(TestState::FAILED, time_taken, muteStream, tname), message(msg){}

    string getMessage(){
        return message;
    }
};

class TestResultAborted: public TestResult{
    string message;
public :
    TestResultAborted(testname tname = "no_testname_given") : TestResult(tname) {}

    TestResultAborted(testname tname, MuteStreamMap muteStream, double time_taken, string msg=""):TestResult(TestState::ABORTED, time_taken, muteStream, tname), message(msg){}

    string getMessage(){
        return message;
    }
};


template<typename ElementType>
class TestResultTypedSubSet:public list<shared_ptr<ElementType>>{ // why does this exist?
public:
    TestResultTypedSubSet<ElementType> getSubSet(function<bool(shared_ptr<ElementType>)> pred){
        TestResultTypedSubSet<ElementType> subset;
        for(auto& element : *this){
            if(pred(element)){
                subset.push_back(element);
            }
        }
        return subset;
    }
};

class TestResultSet:public list<shared_ptr<TestResult>>{
public:
    string out(Format format = Format::Text){
        GetOutputFormat<TestResultSet> output(format);
        return output.run(*this);
    }

    TestResultSet getSubSet(function<bool(shared_ptr<TestResult>)> pred){
        TestResultSet subset;
        copy_if(this->begin(), this->end(), back_inserter(subset), pred); // this performs a filtered copy
        return subset;
    }

    TestResultSet getSubSetByState(TestState state){
        return getSubSet([&](shared_ptr<TestResult> ptr){return ptr->get_state() == state;});
    }

    TestResultSet getIgnores(){
        return getSubSetByState(TestState::IGNORED);
    }

    TestResultSet getOK(){
        return getSubSetByState(TestState::OK);
    }

    TestResultTypedSubSet<TestResultFailed> getFails(){
        TestResultTypedSubSet<TestResultFailed> result;
        for(auto& element : getSubSetByState(TestState::FAILED)){
            result.push_back(castToFailed(element));
        }
        return result;
    }

    TestResultTypedSubSet<TestResultAborted> getAborts(){
        TestResultTypedSubSet<TestResultAborted> result;
        for(auto& element : getSubSetByState(TestState::ABORTED)){
            result.push_back(castToAborted(element));
        }
        return result;
    }

    double getTotalExecutionTimeInSeconds(){
    	double total = 0.0;
    	for_each(this->begin(), this->end(), [&total](shared_ptr<TestResult> element){total += element->get_time_taken();});
    	return total;
    }

    static shared_ptr<TestResultAborted> castToAborted(shared_ptr<TestResult> old){
        return dynamic_pointer_cast<TestResultAborted>(old);
    }

    static shared_ptr<TestResultFailed> castToFailed(shared_ptr<TestResult> old){
        return dynamic_pointer_cast<TestResultFailed>(old);
    }
};



#endif // TESTRESULT_H_INCLUDED
