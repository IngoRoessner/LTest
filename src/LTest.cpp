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


#include "LTest.h"

double CLOCKS_PER_SEC_AS_DOUBLE = static_cast<double>(CLOCKS_PER_SEC);

LTest::LTest():counter(0){
    mutedStreams.setVerboseMode(cout, VerboseMode::FAIL);
    mutedStreams.setVerboseMode(cerr, VerboseMode::FAIL);
}

LTest& LTest::getInstanz(){
    static LTest instanz;
    return instanz;
}

double measure_time_taken(const clock_t& before){
    clock_t after = clock();
    return static_cast<double>(after - before) / CLOCKS_PER_SEC_AS_DOUBLE;
}


TestResultSet LTest::runTest(const testname& tname, function<bool ()> testFunction){
    getInstanz().mutedStreams.mute();
    double time_taken_sec;
    TestResult* result;
    try{
        clock_t before;
        before = clock();
        try {
            bool&& test_successful = testFunction();
            time_taken_sec = measure_time_taken(before);
            ManagedFixtureList::after();
            if(test_successful){
                result = new TestResultOK(tname, getInstanz().mutedStreams, time_taken_sec);
            }else{
                result = new TestResultFailed(tname, getInstanz().mutedStreams, time_taken_sec, "return not true");
            }
        } catch(...){
            ManagedFixtureList::after();
            time_taken_sec = measure_time_taken(before);
            throw;
        }
    }
    catch(LTestMisuse e){throw e;}
    catch(LTAssert::FalseAssert a){result = new TestResultFailed(tname, getInstanz().mutedStreams, time_taken_sec, a.what());}
    catch(runtime_error e){result = new TestResultAborted(tname, getInstanz().mutedStreams, time_taken_sec, e.what());}
    catch(exception e){result = new TestResultAborted(tname, getInstanz().mutedStreams, time_taken_sec, e.what());}
    catch(int e){stringstream es; es<<e; result = new TestResultAborted(tname, getInstanz().mutedStreams, time_taken_sec, "int exception: "+es.str());}
    catch(char e){result = new TestResultAborted(tname, getInstanz().mutedStreams, time_taken_sec, "char exception: "+patch::to_string(e));}
    catch(string e){result = new TestResultAborted(tname, getInstanz().mutedStreams, time_taken_sec, "string exception: "+e);}
    catch(...){result = new TestResultAborted(tname, getInstanz().mutedStreams, time_taken_sec, "Unknown Exception");}
    getInstanz().resultset.push_back(shared_ptr<TestResult>(result));
    return getInstanz().resultset;
}


bool LTest::isIgnored(testname testName){
    bool ignored = false;
    if(getInstanz().ignores.count(testName)){
        ignored = true;
    }
    return ignored;
}

bool LTest::isIgnored(uint testIndex){
    bool ignored = false;
    if(getInstanz().ignored_indexes.count(testIndex)){
        ignored = true;
    }
    return ignored;
}

TestResultSet LTest::runTests(){
	map<uint, testname> inserted_map = getInstanz().test_inserted_order;
	list<testname> test_names_sorted_by_insertion_order;
	transform(inserted_map.begin(), inserted_map.end(), back_inserter(test_names_sorted_by_insertion_order), [](map<uint, testname>::value_type& val){return val.second;});
	return runTests(test_names_sorted_by_insertion_order);
}

TestResultSet LTest::runTest(const testname test){
    function<bool ()> testFunction;
    try{
        testFunction = getInstanz().testCases.at(test);
    }catch(...){
        throw WrongTestName("try to run test \""+test+"\" which not exists");
    }
	return runTest(test, testFunction);
}

 TestResultSet LTest::runTests(const TestSuite testsuite, bool force){
	uint current_index = 0;
    for (auto &testName : testsuite){
    	if(testName != getIgnoreLable()){
			if(force || !(isIgnored(testName) || isIgnored(current_index))){
				LTest::runTest(testName);
			} else {
				TestResult* result = new TestResultIgnored(testName);
                getInstanz().resultset.push_back(shared_ptr<TestResult>(result));
			}
			current_index++;
    	}
    }
    return getInstanz().resultset;;
}

TestResultSet LTest::runTests(const initializer_list<string> testsuite, bool force){
    return LTest::runTests(TestSuite(testsuite), force);
}

TestResultSet LTest::run(ostream& os, Format format){
	auto&& returnable = runTests();
    os<<getInstanz().resultset.out(format);
    clearResultSet();
    return returnable;
}

TestResultSet LTest::run(testname test, ostream& os, Format format){
    auto&& returnable = runTest(test);
    os<<getInstanz().resultset.out(format);
    clearResultSet();
    return returnable;
}

TestResultSet LTest::run(TestSuite testsuite, bool force, ostream& os, Format format){
	auto&& returnable = runTests(testsuite, force);
    os<<getInstanz().resultset.out(format);
    clearResultSet();
    return returnable;
}

TestResultSet LTest::run(initializer_list<string> testsuite, bool force, ostream& os, Format format){
    return LTest::run(TestSuite(testsuite), force, os, format);
}

void LTest::addTestFunction(testname testName, function<bool ()> test){
    if(getInstanz().testCases.count(testName) > 0){
        throw logic_error("testname '"+testName+"' is not unique");
    }

	uint current_count = getInstanz().counter;
    getInstanz().test_inserted_order[current_count] = testName;
    getInstanz().testCases.emplace(move(testName), move(test));
    getInstanz().counter++;
}

string LTest::ignore(testname testName){
    getInstanz().ignores.insert(testName);
    return getIgnoreLable();
}

string LTest::ignoreNext(unsigned int nextNTests){
    unsigned int start = getInstanz().counter;
    unsigned int stop = start + nextNTests;
    for(unsigned int i = start; i < stop; i++){
        getInstanz().ignored_indexes.insert(i);
    }
    return getIgnoreLable();
}

string LTest::ignore(TestSuite testsuite){
    for (auto testName : testsuite){
        if(testName != getIgnoreLable()){
            LTest::ignore(testName);
        }
    }
    return getIgnoreLable();
}

string LTest::ignore(initializer_list<string> testsuite){
    return LTest::ignore(TestSuite(testsuite));
}

string LTest::getIgnoreLable(){
    return "___LTEST::IGNORE___";
}

TestResultSet LTest::getResultSet(){
    return getInstanz().resultset;
}

void LTest::clearResultSet(){
    getInstanz().resultset.clear();
}
