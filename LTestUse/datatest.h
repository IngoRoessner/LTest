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

auto testInt = manageFixture(3)
    .after([](int& i){
        cout<<"after: "<<i++<<endl;
    })
    .before([](int& i){
        cout<<"before: "<<i<<endl;
    });

auto out = manageFixture<ostream&>(cout)
    .after([](ostream& outStr){outStr<<"test ende"<<endl;});

auto ints = manageFixture<vector<int>>()
            .before([](vector<int>& vi){vi = {1,2,3,4,5,6};});

TestSuite DataTests = {
    LTest::addTest("manageFixture vi1", [](int index){
        return ints().at(index);
    }, [](){
        LTest::arguments(0).expect(1);
        LTest::arguments(1).expect(2);
        LTest::arguments(2).expect(3);
        LTest::arguments(3).expect(4);
        LTest::arguments(4).expect(5);
        LTest::arguments(5).expect(6);
        ints() = {9,8,7};
        LTest::arguments(0).expect(9);
        LTest::arguments(1).expect(8);
        LTest::arguments(2).expect(7);
    }),

    LTest::addTest("manageFixture vi2", [](int index){
        return ints().at(index);
    }, [](){
        LTest::arguments(0).expect(1);
        LTest::arguments(1).expect(2);
        LTest::arguments(2).expect(3);
        LTest::arguments(3).expect(4);
        LTest::arguments(4).expect(5);
        LTest::arguments(5).expect(6);
        ints() = {9,8,7};
        LTest::arguments(0).expect(9);
        LTest::arguments(1).expect(8);
        LTest::arguments(2).expect(7);
    }),

    LTest::addTest("testInt1", [&]{out()<<"run testInt1: "<<testInt();}),
    LTest::addTest("testInt2", [&]{out()<<"run testInt2: "<<testInt(); testInt()++;}),
    LTest::addTest("testInt3", [&]{out()<<"run testInt3: "<<testInt();}),

    LTest::addTest("viii", viii, [](){
        LTest::arguments(1,2,3);
        LTest::arguments(2,2,3);
        LTest::arguments(3,2,3);
    }),

    LTest::addTest("iii", iii, [](){
        LTest::arguments(2,3).expect(1);
        LTest::arguments(4,3).expect(2);
        LTest::arguments(2,3).expect(3);
    }),

    LTest::addTest("validate iii", iii, [](){
        LTest::arguments(2,3).validate([](int i){return i>0;});
        LTest::arguments(4,3).validate([](int i){return i>0;});
        LTest::arguments(2,3).validate([](int i){return i<0;});
    }),

    LTest::addTest("validate Assert iii", iii, [](){
        LTest::arguments(2,3).validate([](int i){LTAssert::True(true, "not true"); return i>0;});
        LTest::arguments(4,3).validate([](int i){LTAssert::True(false, "not true"); return i>0;});
        LTest::arguments(2,3).validate([](int i){LTAssert::True(true, "not true"); return i>0;});
    }),

    LTest::addTest("anything iii", iii, [](){
        LTest::arguments(2,3).expect<int>();
        LTest::arguments(4,3).expect<int>();
        LTest::arguments(2,3).expect<int>();
    }),

    LTest::addTest("iiie", iiie, [](){
        LTest::arguments(2,3).expect(1);
        LTest::arguments(4,3).expect(2);
        LTest::arguments(2,3).expect(3);
    }),

    LTest::addTest("ili", ili, [](){
        LTest::arguments(list<int>{1,2,3,1},3).expect(1);
        LTest::arguments(list<int>{1,2,3,2},3).expect(2);
        LTest::arguments(list<int>{1,2,3,1},3).expect(3);
    }),

    LTest::addTest("csi", csi, [](){
        LTest::arguments(string("abcde"),3).expect('d');
        LTest::arguments(string("abcde"),0).expect('a');
        LTest::arguments(string("abcde"),1).expect('a');
    }),

    LTest::addTest("vi", vi, [](){
        LTest::arguments(1);
        LTest::arguments(2);
        LTest::arguments(3);
    }),

    //cast lambda to functionpointer (only capture less lambdas)
    LTest::addTest("ir", [](int i){ir(i);}, [](){
        LTest::arguments(1);
        LTest::arguments(2);
        LTest::arguments(3);
    }),

    LTest::addTest(
        "lambdaiii",
        [](int a, int b)->int{
            cout<<"lambdaiii"<<a<<b<<endl;
            return 1;
        },
        [](){
            LTest::arguments(2,3).expect(1);
            LTest::arguments(3,3).expect(2);
            LTest::arguments(2,3).expect(3);
        }
    ),

    LTest::addTest(
        "ccsi",
        [](string s, int i){
            char* cs = const_cast<char*>(s.c_str());
            return ccsi(cs, i);
        },
        [](){
            LTest::arguments(string("abcde"),3).expect('d');
            LTest::arguments(string("abcde"),0).expect('a');
            LTest::arguments(string("abcde"),1).expect('a');
        }
    ),

    //add to expected output
    LTest::addTest("ir2", ir, [](){
        int i1=1, i2=2, i3=3;
        LTest::arguments<int&>(i1);
        LTest::arguments<int&>(i2);
        LTest::arguments<int&>(i3);
    }),

    LTest::addTest(
        "ccsi2",ccsi,
        [](){
            LTest::arguments<char*, int>("abcde",3).expect('d');
            LTest::arguments<char*, int>("abcde",0).expect('a');
            LTest::arguments<char*, int>("abcde",1).expect('a');
        }
    )
};

#endif // DATATEST_H_INCLUDED
