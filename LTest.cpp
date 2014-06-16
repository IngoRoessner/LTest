#include "LTest.h"

LTest::LTest():counter(0){

}

LTest& LTest::getInstanz(){
    static LTest instanz;
    return instanz;
}

        void LTest::runTests(){
            for(auto const & element : getInstanz().testCases) {
                string testName = get<0>(element);
                if(!(getInstanz().ignores.count(testName))){
                    function<bool ()> test = get<1>(element);
                    try{
                        if(test()){
                            getInstanz().ok.push_back(testName);
                        }else{
                            getInstanz().fail.push_back(testName);
                        }
                    }
                    catch(LTAssert::FalseAssert a){getInstanz().assert.push_back(make_tuple(testName, a.what()));}
                    catch(exception e){getInstanz().error.push_back(make_tuple(testName, e.what()));}
                    catch(int e){getInstanz().error.push_back(make_tuple(testName, "int exception: "+e));}
                    catch(char e){getInstanz().error.push_back(make_tuple(testName, "char exception: "+e));}
                    catch(...){getInstanz().error.push_back(make_tuple(testName, "Unknown Exception"));}
                }else{
                    getInstanz().actualIgnore.push_back(testName);
                }
            }
        }


        void LTest::errorOut(ostream& os){
            for(auto const & element : getInstanz().error) {
                os << get<0>(element) + ": " + get<1>(element) << endl;
            }
        }

        void LTest::assertOut(ostream& os){
            for(auto const & element : getInstanz().assert) {
                os << get<0>(element) + ": " + get<1>(element) << endl;
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

        void LTest::testOut(ostream& os){
            okOut(os);
            failOut(os);
            assertOut(os);
            errorOut(os);
            ignoreOut(os);
            countOut(os);
        }

        void LTest::run(ostream& os){
            runTests();
            testOut(os);
        }

        void LTest::addTest(string testName, function<bool ()> test){
            if(getInstanz().ignores.count(patch::to_string(getInstanz().counter))){
                ignore(testName);
            }

            getInstanz().counter++;
            getInstanz().testCases.push_back(make_tuple(testName, test));
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

