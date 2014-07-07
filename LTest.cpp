#include "LTest.h"

LTest::LTest():counter(0){

}

LTest& LTest::getInstanz(){
    static LTest instanz;
    return instanz;
}

void LTest::runTest(const string& testName, funktionPointer& testFunction){
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
    for(auto const & element : getInstanz().testCases) {
        string testName = element.first;
        funktionPointer testFunction = element.second;
        if(!(getInstanz().ignores.count(testName))){
            runTest(testName, testFunction);
        }else{
            getInstanz().actualIgnore.push_back(testName);
        }
    }
}

void LTest::runTest(const string& test){
    for(auto const & element : getInstanz().testCases) {
        string testName = element.first;
        funktionPointer testFunction = element.second;
        if(testName.compare(test) == 0){
            runTest(testName, testFunction);
        }
    }
}

void LTest::errorOut(ostream& os){
    for(auto const & element : getInstanz().error) {
        os << element.first + ": " + element.second << endl;
    }
}

void LTest::assertOut(ostream& os){
    for(auto const & element : getInstanz().assert) {
        os << element.first + ": " + element.second << endl;
    }
}

void LTest::failOut(ostream& os){
    for(auto const & element : getInstanz().fail) {
        os << element << ": Fail" << endl;
    }
}

void LTest::okOut(ostream& os){
    for(auto const & element : getInstanz().ok) {
        os << element << ": OK" << endl;
    }
}

void LTest::ignoreOut(ostream& os){
    for(auto const & element : getInstanz().actualIgnore) {
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
}

void LTest::run(ostream& os){
    runTests();
    output(os);
}

void LTest::run(string test, ostream& os){
    runTest(test);
    output(os);
}

void LTest::addTest(string testName, funktionPointer test){
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

