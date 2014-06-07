#include "LTest.h"

LTest::LTest():counter(0){

}

LTest& LTest::getInstanz(){
    static LTest instanz;
    return instanz;
}

        void LTest::runTests(){
            for (TestListType::iterator it = getInstanz().testCases.begin(); it != getInstanz().testCases.end(); it++){
                string testName = get<0>(*it);
                if(!(getInstanz().ignores.count(testName))){
                    function<bool ()> test = get<1>(*it);
                    try{
                        if(test()){
                            getInstanz().ok.push_back(testName);
                        }else{
                            getInstanz().fail.push_back(testName);
                        }
                    }catch(exception e){getInstanz().error.push_back(make_tuple(testName, e.what()));}
                    catch(int e){getInstanz().error.push_back(make_tuple(testName, "int exception: "+e));}
                    catch(char e){getInstanz().error.push_back(make_tuple(testName, "char exception: "+e));}
                    catch(...){getInstanz().error.push_back(make_tuple(testName, "Unknown Exception"));}
                }else{
                    getInstanz().actualIgnore.push_back(testName);
                }
            }
        }


        void LTest::errorOut(ostream& os){
            for (list<tuple<string, string>>::iterator it = getInstanz().error.begin(); it != getInstanz().error.end(); it++){
                os<<get<0>(*it)+": "+get<1>(*it)<<endl;
            }
        }

        void LTest::failOut(ostream& os){
            for (list<string>::iterator it = getInstanz().fail.begin(); it != getInstanz().fail.end(); it++){
                os<<(*it)+": Fail"<<endl;
            }
        }

        void LTest::okOut(ostream& os){
            for (list<string>::iterator it = getInstanz().ok.begin(); it != getInstanz().ok.end(); it++){
                os<<(*it)+": OK"<<endl;
            }
        }

        void LTest::testOut(ostream& os){
            okOut(os);
            failOut(os);
            errorOut(os);
        }

        void LTest::run(ostream& os){
            runTests();
            testOut(os);
        }

        void LTest::addTest(string testName, function<bool ()> test){
            if(getInstanz().ignores.count(""+getInstanz().counter)){
                ignore(testName);
            }
            if(getInstanz().nexIgnore){
                ignore(testName);
                getInstanz().nexIgnore = false;
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
        getInstanz().ignore(""+i);
    }
}

void LTest::ignore(){
    getInstanz().nexIgnore = true;
}
