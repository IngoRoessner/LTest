#ifndef DATATEST_H_INCLUDED
#define DATATEST_H_INCLUDED

void viii(int a, int b, int c){
    cout<<"viii"<<a<<b<<c<<endl;
}

int iii(int b, int c){
    cout<<"viii"<<b<<c<<endl;
    return 1;
}

int iiie(int b, int c){
    cout<<"viii"<<b<<c<<endl;
    if(b==4)
        throw "dow";
    return 1;
}

int ili(list<int> l, int i){
    list<int>::iterator it = next(l.begin(), i);
    return *it;
}

char csi(string s, int i){
    return s.at(i);
}

char ccsi(char* s, int i){
    return s[i];
}

void vi(int i){
    if(i==3)
        throw "dow";
}

void ir(int& i){
    if(i==3)
        throw "dow";
}

TestSuite DataTests = {
    LTest::addTest("viii", viii, [](){
        LTest::fixture(1,2,3);
        LTest::fixture(2,2,3);
        LTest::fixture(3,2,3);
    }),

    LTest::addTest("iii", iii, [](){
        LTest::fixture(1,2,3);
        LTest::fixture(2,4,3);
        LTest::fixture(3,2,3);
    }),

    LTest::addTest("iiie", iiie, [](){
        LTest::fixture(1,2,3);
        LTest::fixture(2,4,3);
        LTest::fixture(3,2,3);
    }),

    LTest::addTest("ili", ili, [](){
        LTest::fixture(1,list<int>{1,2,3,1},3);
        LTest::fixture(2,list<int>{1,2,3,2},3);
        LTest::fixture(3,list<int>{1,2,3,1},3);
    }),

    LTest::addTest("csi", csi, [](){
        LTest::fixture('d',string("abcde"),3);
        LTest::fixture('a',string("abcde"),0);
        LTest::fixture('a',string("abcde"),1);
    }),

    LTest::addTest("vi", vi, [](){
        LTest::fixture(1);
        LTest::fixture(2);
        LTest::fixture(3);
    }),

    //cast lambda to functionpointer (only capture less lambdas)
    LTest::addTest("ir", (void(*)(int))([](int i){ir(i);}), [](){
        LTest::fixture(1);
        LTest::fixture(2);
        LTest::fixture(3);
    }),

    //wrap functions with reference or pointer parameter with lambda
    //because the wouldnt work with the fixture function
    LTest::addTest(
        "lambdaiii",
        (int(*)(int, int))([](int a, int b)->int{
            cout<<"lambdaiii"<<a<<b<<endl;
            return 1;
        }),
        [](){
            LTest::fixture(1,2,3);
            LTest::fixture(2,3,3);
            LTest::fixture(3,2,3);
        }
    ),

    //you cann use the lambda to convert your fixtures to test specific parameters
    //(the function ccsi is tested in the example)
    LTest::addTest(
        "ccsi",
        (char(*)(string, int))([](string s, int i){
            char* cs = const_cast<char*>(s.c_str());
            return ccsi(cs, i);
        }),
        [](){
            LTest::fixture('d',string("abcde"),3);
            LTest::fixture('a',string("abcde"),0);
            LTest::fixture('a',string("abcde"),1);
        }
    )
};

#endif // DATATEST_H_INCLUDED
