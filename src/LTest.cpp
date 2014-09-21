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
    mutedStreams.setVerboseMode(std::cout, VerboseMode::FAIL);
    mutedStreams.setVerboseMode(std::cerr, VerboseMode::FAIL);
}

LTest& LTest::getInstanz(){
    static LTest instanz;
    return instanz;
}

double measure_time_taken(const clock_t& before){
    clock_t after = clock();
    return static_cast<double>(after - before) / CLOCKS_PER_SEC_AS_DOUBLE;
}


std::shared_ptr<TestResult> LTest::runTest(const testname& tname, std::function<bool ()> testFunction){
    getInstanz().mutedStreams.mute();
    double time_taken_sec = -1.0;
    double user_time_taken_sec = -1.0;
    TestResult* result;
    try{
        std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now(),
                                                      end;
        clock_t before;
        before = clock();
        try {
            bool&& test_successful = testFunction();
            user_time_taken_sec = measure_time_taken(before);
            end = std::chrono::system_clock::now();
            time_taken_sec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1e9;
            ManagedFixtureList::after();
            if(test_successful){
                result = new TestResultOK(tname, getInstanz().mutedStreams, time_taken_sec, user_time_taken_sec);
            }else{
                result = new TestResultFailed(tname, getInstanz().mutedStreams, time_taken_sec, user_time_taken_sec, "return not true");
            }
        } catch(...){
            ManagedFixtureList::after();
            time_taken_sec = measure_time_taken(before);
            throw;
        }
    }
    catch(LTestMisuse e){throw e;} // TODO well this code smells a bit, i needed to changes to much to add a field, DRY?
    catch(LTAssert::FalseAssert a){result = new TestResultFailed(tname, getInstanz().mutedStreams, time_taken_sec, user_time_taken_sec, a.what());}
    catch(std::runtime_error e){result = new TestResultAborted(tname, getInstanz().mutedStreams, time_taken_sec, user_time_taken_sec, e.what());}
    catch(std::exception e){result = new TestResultAborted(tname, getInstanz().mutedStreams, time_taken_sec, user_time_taken_sec, e.what());}
    catch(int e){std::stringstream es; es<<e; result = new TestResultAborted(tname, getInstanz().mutedStreams, time_taken_sec, user_time_taken_sec, "int exception: "+es.str());}
    catch(char e){result = new TestResultAborted(tname, getInstanz().mutedStreams, time_taken_sec, user_time_taken_sec, "char exception: "+patch::to_string(e));}
    catch(std::string e){result = new TestResultAborted(tname, getInstanz().mutedStreams, time_taken_sec, user_time_taken_sec,"string exception: "+e);}
    catch(...){result = new TestResultAborted(tname, getInstanz().mutedStreams, time_taken_sec, user_time_taken_sec, "Unknown Exception");}
    return std::shared_ptr<TestResult>(result);
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
	std::map<uint, testname> inserted_map = getInstanz().test_inserted_order;
	std::list<testname> test_names_sorted_by_insertion_order;
	std::transform(inserted_map.begin(), inserted_map.end(), back_inserter(test_names_sorted_by_insertion_order), [](std::map<uint, testname>::value_type& val){return val.second;});
	return runTests(test_names_sorted_by_insertion_order);
}

TestResultSet LTest::runTest(const testname test){
    std::function<bool ()> testFunction;
    try{
        testFunction = getInstanz().testCases.at(test);
    }catch(...){
        throw WrongTestName("try to run test \""+test+"\" which not exists");
    }
    TestResultSet resultset;
    resultset.push_back(runTest(test, testFunction));
	return resultset;
}

 TestResultSet LTest::runTests(const TestSuite testsuite, bool force){
	uint current_index = 0;
    for (auto &testName : testsuite){
    	if(testName != getIgnoreLabel()){
			if(force || !(isIgnored(testName) || isIgnored(current_index))){
                for(std::shared_ptr<TestResult>& element : LTest::runTest(testName)){
                    getInstanz().resultset.push_back(element);
                }
			} else {
				TestResult* result = new TestResultIgnored(testName);
                getInstanz().resultset.push_back(std::shared_ptr<TestResult>(result));
			}
			current_index++;
    	}
    }
    return getInstanz().resultset;;
}

TestResultSet LTest::runTests(const std::initializer_list<std::string> testsuite, bool force){
    return LTest::runTests(TestSuite(testsuite), force);
}

TestResultSet LTest::run(std::ostream& os, Format format){
	auto&& returnable = runTests();
    os<<getInstanz().resultset.out(format);
    clearResultSet();
    return returnable;
}

TestResultSet LTest::run(testname test, std::ostream& os, Format format){
    TestResultSet resultset = runTest(test);
    os<<resultset.out(format);
    return resultset;
}

TestResultSet LTest::run(TestSuite testsuite, bool force, std::ostream& os, Format format){
	auto&& returnable = runTests(testsuite, force);
    os<<getInstanz().resultset.out(format);
    clearResultSet();
    return returnable;
}

TestResultSet LTest::run(std::initializer_list<std::string> testsuite, bool force, std::ostream& os, Format format){
    return LTest::run(TestSuite(testsuite), force, os, format);
}

void LTest::addTestFunction(testname testName, std::function<bool ()> test){
    if(getInstanz().testCases.count(testName) > 0){
        throw std::logic_error("testname '"+testName+"' is not unique");
    }

	uint current_count = getInstanz().counter;
    getInstanz().test_inserted_order[current_count] = testName;
    getInstanz().testCases.emplace(std::move(testName), std::move(test));
    getInstanz().counter++;
}

std::string LTest::ignore(testname testName){
    getInstanz().ignores.insert(testName);
    return getIgnoreLabel();
}

std::string LTest::ignoreNext(unsigned int nextNTests){
    unsigned int start = getInstanz().counter;
    unsigned int stop = start + nextNTests;
    for(unsigned int i = start; i < stop; i++){
        getInstanz().ignored_indexes.insert(i);
    }
    return getIgnoreLabel();
}

std::string LTest::ignore(TestSuite testsuite){
    for (auto testName : testsuite){
        if(testName != getIgnoreLabel()){
            LTest::ignore(testName);
        }
    }
    return getIgnoreLabel();
}

std::string LTest::ignore(std::initializer_list<std::string> testsuite){
    return LTest::ignore(TestSuite(testsuite));
}

std::string LTest::getIgnoreLabel(){
    return "___LTEST::IGNORE___";
}

TestResultSet LTest::getResultSet(){
    return getInstanz().resultset;
}

void LTest::clearResultSet(){
    getInstanz().resultset.clear();
}
