#include "LTest.h"

LTest::LTest():counter(0){
    mutedStreams.setMuteMode(cout, MuteMode::FAIL);
}

LTest& LTest::getInstanz(){
    static LTest instanz;
    return instanz;
}

void LTest::runTest(const string& testName, function<bool ()>& testFunction){
    bool testFailed = true;
    getInstanz().mutedStreams.muteOut();
    try{
        if(testFunction()){
            getInstanz().ok.push_back(testName);
            testFailed = false;
        }else{
            getInstanz().fail.push_back(testName);
        }
    }
    catch(LTAssert::FalseAssert a){getInstanz().assert.push_back(make_pair(testName, a.what()));}
    catch(exception e){getInstanz().error.push_back(make_pair(testName, e.what()));}
    catch(int e){getInstanz().error.push_back(make_pair(testName, "int exception: "+e));}
    catch(char e){getInstanz().error.push_back(make_pair(testName, "char exception: "+e));}
    catch(...){getInstanz().error.push_back(make_pair(testName, "Unknown Exception"));}
    getInstanz().mutedStreams.flushOut(testName, testFailed);
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

void LTest::runTests(){
    for(auto element : getInstanz().testCases){
        string testName = element.first;
        function<bool ()> testFunction = element.second;
        if(!isIgnored(testName)){
            runTest(testName, testFunction);
        }
    }
}

void LTest::runTest(const string& test){
    for(auto element : getInstanz().testCases){
        string testName = element.first;
        function<bool ()> testFunction = element.second;
        if(testName.compare(test) == 0){
            runTest(testName, testFunction);
        }
    }
}

void LTest::runTest(TestSuite& testsuite, bool force){
    for (auto testName : testsuite){
        if(testName != getIgnoreLable() && (force || !isIgnored(testName))){
            LTest::runTest(testName);
        }
    }
}

void LTest::errorOut(ostream& os){
    for(auto element : getInstanz().error){
        os << element.first + ": " + element.second << endl;
    }
}

void LTest::assertOut(ostream& os){
    for(auto element : getInstanz().assert){
        os << element.first + ": " + element.second << endl;
    }
}

void LTest::failOut(ostream& os){
    for(auto element : getInstanz().fail){
        os << element << ": Fail" << endl;
    }
}

void LTest::okOut(ostream& os){
    for(auto element : getInstanz().ok){
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

void LTest::run(ostream& os){
    runTests();
    output(os);
}

void LTest::run(string test, ostream& os){
    runTest(test);
    output(os);
}

void LTest::run(TestSuite& testsuite, bool force, ostream& os){
    runTest(testsuite, force);
    output(os);
}

void LTest::addTestFunction(string testName, function<bool ()> test){
    if(getInstanz().ignores.count(patch::to_string(getInstanz().counter))){
        ignore(testName);
    }
    getInstanz().counter++;
    getInstanz().testCases.push_back(make_pair(testName, test));
}

string LTest::ignore(string testName){
    getInstanz().ignores[testName] = true;
    return getIgnoreLable();
}

string LTest::ignore(unsigned int number){
    unsigned int start = getInstanz().counter;
    unsigned int stop = start+number;
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
