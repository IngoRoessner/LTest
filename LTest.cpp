#include "LTest.h"

LTest::LTest():counter(0){

}

LTest& LTest::getInstanz(){
    static LTest instanz;
    return instanz;
}

void LTest::runTest(const string& testName, LTestFunctionPointer& testFunction){
    try{
        if(testFunction()){
            getInstanz().ok.push_back(testName);
        }else{
            getInstanz().fail.push_back(testName);
        }
    }
    catch(LTAssert::FalseAssert a){getInstanz().assert.push_back(make_pair(testName, a.what()));}
    catch(exception e){getInstanz().error.push_back(make_pair(testName, e.what()));}
    catch(int e){getInstanz().error.push_back(make_pair(testName, "int exception: "+e));}
    catch(char e){getInstanz().error.push_back(make_pair(testName, "char exception: "+e));}
    catch(...){getInstanz().error.push_back(make_pair(testName, "Unknown Exception"));}
}

void LTest::runTests(){
    for(TestListType::iterator it = getInstanz().testCases.begin(); it!=getInstanz().testCases.end(); it++){
        pair<string, LTestFunctionPointer>& element = *it;
        string testName = element.first;
        LTestFunctionPointer testFunction = element.second;
        if(!(getInstanz().ignores.count(testName))){
            runTest(testName, testFunction);
        }else{
            getInstanz().actualIgnore.push_back(testName);
        }
    }
}

void LTest::runTest(const string& test){
    for(TestListType::iterator it = getInstanz().testCases.begin(); it!=getInstanz().testCases.end(); it++){
        pair<string, LTestFunctionPointer>& element = *it;
        string testName = element.first;
        LTestFunctionPointer testFunction = element.second;
        if(testName.compare(test) == 0){
            runTest(testName, testFunction);
        }
    }
}

void LTest::runTest(list<string>& testsuite){
    for (list<string>::iterator it = testsuite.begin(); it != testsuite.end(); it++){
        LTest::runTest(*it);
    }
}

void LTest::errorOut(ostream& os){
    for(list<pair<string, string> >::iterator it = getInstanz().error.begin(); it!=getInstanz().error.end(); it++){
        pair<string, string>& element = *it;
        os << element.first + ": " + element.second << endl;
    }
}

void LTest::assertOut(ostream& os){
    for(list<pair<string, string> >::iterator it = getInstanz().assert.begin(); it!=getInstanz().assert.end(); it++){
        pair<string, string>& element = *it;
        os << element.first + ": " + element.second << endl;
    }
}

void LTest::failOut(ostream& os){
    for(list<string>::iterator it = getInstanz().fail.begin(); it!=getInstanz().fail.end(); it++){
        string& element = *it;
        os << element << ": Fail" << endl;
    }
}

void LTest::okOut(ostream& os){
    for(list<string>::iterator it = getInstanz().ok.begin(); it!=getInstanz().ok.end(); it++){
        string& element = *it;
        os << element << ": OK" << endl;
    }
}

void LTest::ignoreOut(ostream& os){
        for(list<string>::iterator it = getInstanz().actualIgnore.begin(); it!=getInstanz().actualIgnore.end(); it++){
        string& element = *it;
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

void LTest::addTestFunction(string testName, LTestFunctionPointer test){
    if(getInstanz().ignores.count(patch::to_string(getInstanz().counter))){
        ignore(testName);
    }
    getInstanz().counter++;
    getInstanz().testCases.push_back(make_pair(testName, test));
}

void LTest::ignore(string testName){
    getInstanz().ignores[testName] = true;
}

void LTest::ignore(unsigned int number){
    unsigned int start = getInstanz().counter;
    unsigned int stop = start+number;
    for(unsigned int i = start; i<stop; i++){
        getInstanz().ignore(patch::to_string(i));
    }
}

void LTest::ignore(list<string>& testsuite){
    for (list<string>::iterator it = testsuite.begin(); it != testsuite.end(); it++){
        LTest::ignore(*it);
    }
}
