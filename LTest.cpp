#include "LTest.h"

double CLOCKS_PER_SEC_AS_DOUBLE = static_cast<double>(CLOCKS_PER_SEC);

LTest::LTest():counter(0){
    mutedStreams.setMuteMode(cout, MuteMode::FAIL);
}

LTest& LTest::getInstanz(){
    static LTest instanz;
    return instanz;
}

double measure_time_taken(const clock_t& before){
    clock_t after = clock();
    return static_cast<double>(after - before) / CLOCKS_PER_SEC_AS_DOUBLE;
}

TestResult LTest::runTest(const testname& tname, function<bool ()> testFunction){
    bool testFailed = true;
    getInstanz().mutedStreams.mute();
    double time_taken_sec;
    TestResult::TestState state;
    try{
        clock_t before;
        before = clock();
        try {
            bool&& test_successful = testFunction();
            time_taken_sec = measure_time_taken(before);
            if(test_successful){
                getInstanz().ok.push_back(tname);
                testFailed = false;
                state = TestResult::TestState::OK;
            }else{
                getInstanz().fail.push_back(tname);
                state = TestResult::TestState::FAILED;
            }
        } catch(...){
            time_taken_sec = measure_time_taken(before);
            state = TestResult::TestState::ABORTED;
            throw;
        }
    }
    catch(LTAssert::FalseAssert a){getInstanz().assert.push_back(make_pair(tname, a.what()));}
    catch(exception e){getInstanz().error.push_back(make_pair(tname, e.what()));}
    catch(int e){getInstanz().error.push_back(make_pair(tname, "int exception: "+e));}
    catch(char e){getInstanz().error.push_back(make_pair(tname, "char exception: "+e));}
    catch(...){getInstanz().error.push_back(make_pair(tname, "Unknown Exception"));}
    map<ostream*, string>&& output_mapping = getInstanz().mutedStreams.flush(tname, testFailed);
    return TestResult(state, time_taken_sec, move(output_mapping), tname);
}

bool LTest::isIgnored(string testName){
    bool result;
    if(getInstanz().ignores.count(testName)){
        result = true;
        getInstanz().actualIgnore.push_back(testName);
    }else{
        result = false;
    }
    return result;
}

list<TestResult> LTest::runTests(){
	return runTests(getInstanz().test_inserted_order);
}

TestResult LTest::runTest(const string& test){
	function<bool ()> testFunction = getInstanz().testCases.at(test);
	return runTest(test, testFunction);
}

list<TestResult> LTest::runTests(const TestSuite& testsuite, bool force){
	list<TestResult> results;
    for (auto &testName : testsuite){
    	if(testName != getIgnoreLable()){
			if(force || !isIgnored(testName)){
				results.push_back(move(LTest::runTest(testName)));
			} else {
				TestResult ignored_result(testName);
				results.push_back(forward<TestResult>(ignored_result));
			}
    	}
    }
    return results;
}

void LTest::errorOut(ostream& os){
    for(auto &element : getInstanz().error){
        os << element.first + ": " + element.second << endl;
    }
}

void LTest::assertOut(ostream& os){
    for(auto &element : getInstanz().assert){
        os << element.first + ": " + element.second << endl;
    }
}

void LTest::failOut(ostream& os){
    for(auto &element : getInstanz().fail){
        os << element << ": Fail" << endl;
    }
}

void LTest::okOut(ostream& os){
    for(auto &element : getInstanz().ok){
        os << element << ": OK" << endl;
    }
}

void LTest::ignoreOut(ostream& os){
    for(auto element : getInstanz().actualIgnore){
        os << element + ": Ignore" << endl;
    }
}

void LTest::countOut(ostream& os){
    os<<"Ignored("<<getInstanz().actualIgnore.size()<<"), OK("<<getInstanz().ok.size()<<"), Fail("<<getInstanz().fail.size()<<"), Assert("<<getInstanz().assert.size()<<")"<<"), Exception("<<getInstanz().error.size()<<")"<<endl;
}

void LTest::output(ostream& os){
    okOut(os);
    failOut(os);
    assertOut(os);
    errorOut(os);
    ignoreOut(os);
    countOut(os);
    clearOutput();
}

void LTest::clearOutput(){
    getInstanz().ok.clear();
    getInstanz().fail.clear();
    getInstanz().assert.clear();
    getInstanz().error.clear();
    getInstanz().actualIgnore.clear();
}

list<TestResult> LTest::run(ostream& os){
	auto&& returnable = runTests();
    output(os);
    return returnable;
}

TestResult LTest::run(string test, ostream& os){
    auto&& returnable = runTest(test);
    output(os);
    return returnable;
}

list<TestResult> LTest::run(TestSuite& testsuite, bool force, ostream& os){
	auto&& returnable = runTests(testsuite, force);
    output(os);
    return returnable;
}

void LTest::addTestFunction(string testName, function<bool ()> test){
    if(getInstanz().ignores.count(patch::to_string(getInstanz().counter))){
        ignore(testName);
    }
    getInstanz().counter++;
    getInstanz().testCases.emplace(testName, test);
    getInstanz().test_inserted_order.push_back(move(testName));
}

string LTest::ignore(string testName){
    getInstanz().ignores[testName] = true;
    return getIgnoreLable();
}

string LTest::ignoreNext(unsigned int nextNTests){
    unsigned int start = getInstanz().counter;
    unsigned int stop = start + nextNTests;
    for(unsigned int i = start; i<stop; i++){
        getInstanz().ignore(patch::to_string(i));
    }
    return getIgnoreLable();
}

string LTest::ignore(TestSuite& testsuite){
    for (auto testName : testsuite){
        if(testName != getIgnoreLable()){
            LTest::ignore(testName);
        }
    }
    return getIgnoreLable();
}

string LTest::getIgnoreLable(){
    return "___LTEST::IGNORE___";
}
