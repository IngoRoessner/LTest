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

#include "../src/LTest.h"

namespace parametertest{

using namespace std;

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

bool validate1(bool b){
    return b;
}

void validate2(bool b){
    if(b)
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
    ltest.addTest("manageFixture vi1", [](int index){
        return ints().at(index);
    }, [](auto test){
        test.with(0).expect(1);
        test.with(1).expect(2);
        test.with(2).expect(3);
        test.with(3).expect(4);
        test.with(4).expect(5);
        test.with(5).expect(6);
        ints() = {9,8,7};
        test.with(0).expect(9);
        test.with(1).expect(8);
        test.with(2).expect(7);
    }),

    ltest.addTest("manageFixture vi2", [](int index){
        return ints().at(index);
    }, [](auto test){
        test.with(0).expect(1);
        test.with(1).expect(2);
        test.with(2).expect(3);
        test.with(3).expect(4);
        test.with(4).expect(5);
        test.with(5).expect(6);
        ints() = {9,8,7};
        test.with(0).expect(9);
        test.with(1).expect(8);
        test.with(2).expect(7);
    }),

    ltest.addTest("manageFixture vi3", [](int index){
        return ints().at(index);
    }, [](auto test){
        ints() = {9,8,7};
        test.with(0).expect(9);
        test.with(1).expect(8);
        test.with(2).expect(7);
    }),


    ltest.addTest("testInt1", [&]{out()<<"run testInt1: "<<testInt();}),
    ltest.addTest("testInt2", [&]{out()<<"run testInt2: "<<testInt(); testInt()++;}),
    ltest.addTest("testInt3", [&]{out()<<"run testInt3: "<<testInt();}),

    //if you cant use c++14 generic lambdas (auto test)
    ltest.addTest("viii", viii, [](ParameterTest<void, int, int, int> test){
        test.with(1,2,3);
        test.with(2,2,3);
        test.with(3,2,3);
    }),

    ltest.addTest("iii", iii, [](auto test){
        test.with(2,3).expect(1);
        test.with(4,3).expect(2);
        test.with(2,3).expect(3);
    }),

    ltest.addTest("validate iii", iii, [](auto test){
        test.with(2,3).expect([](int i){return i>0;});
        test.with(4,3).expect([](int i){return i>0;});
        test.with(2,3).expect([](int i){return i<0;});
    }),

    ltest.addTest("validate Assert iii", iii, [](auto test){
        test.with(2,3).expect([](int i){LTAssert::True(true, "not true"); return i>0;});
        test.with(4,3).expect([](int i){LTAssert::True(false, "not true"); return i>0;});
        test.with(2,3).expect([](int i){LTAssert::True(true, "not true"); return i>0;});
    }),

    ltest.addTest("validate void iii", iii, [](auto test){
        test.with(2,3).expect([](int i){LTAssert::True(true, "not true");});
        test.with(4,3).expect([](int i){LTAssert::True(false, "not true");});
        test.with(2,3).expect([](int i){LTAssert::True(true, "not true");});
    }),

    ltest.addTest("validate1", [](bool b){return b;}, [](auto test){
        test.with(true).expect(validate1);
        test.with(true).expect(validate1);
        test.with(false).expect(validate1);
    }),

    ltest.addTest("validate2", [](bool b){return b;}, [](auto test){
        test.with(false).expect(validate2);
        test.with(false).expect(validate2);
        test.with(true).expect(validate2);
    }),

    ltest.addTest("anything iii", iii, [](auto test){
        test.with(2,3);
        test.with(4,3);
        test.with(2,3);
    }),

    ltest.addTest("iiie", iiie, [](auto test){
        test.with(2,3).expect(1);
        test.with(4,3).expect(2);
        test.with(2,3).expect(3);
    }),

    ltest.addTest("ili", ili, [](auto test){
        test.with(list<int>{1,2,3,1},3).expect(1);
        test.with(list<int>{1,2,3,2},3).expect(2);
        test.with(list<int>{1,2,3,1},3).expect(3);
    }),

    ltest.addTest("csi", csi, [](auto test){
        test.with(string("abcde"),3).expect('d');
        test.with(string("abcde"),0).expect('a');
        test.with(string("abcde"),1).expect('a');
    }),

    ltest.addTest("vi", vi, [](auto test){
        test.with(1);
        test.with(2);
        test.with(3);
    }),

    //cast lambda to functionpointer (only capture less lambdas)
    ltest.addTest("ir", [](int i){ir(i);}, [](auto test){
        test.with(1);
        test.with(2);
        test.with(3);
    }),

    ltest.addTest(
        "lambdaiii",
        [](int a, int b)->int{
            cout<<"lambdaiii"<<a<<b<<endl;
            return 1;
        },
        [](auto test){
            test.with(2,3).expect(1);
            test.with(3,3).expect(2);
            test.with(2,3).expect(3);
        }
    ),

    ltest.addTest(
        "ccsi",
        [](string s, int i){
            char* cs = const_cast<char*>(s.c_str());
            return ccsi(cs, i);
        },
        [](auto test){
            test.with(string("abcde"),3).expect('d');
            test.with(string("abcde"),0).expect('a');
            test.with(string("abcde"),1).expect('a');
        }
    ),

    //add to expected output
    ltest.addTest("ir2", ir, [](auto test){
        int i1=1, i2=2, i3=3;
        test.with(i1);
        test.with(i2);
        test.with(i3);
    }),

    ltest.addTest(
        "ccsi2",ccsi,
        [](auto test){
            test.with("abcde",3).expect('d');
            test.with("abcde",0).expect('a');
            test.with("abcde",1).expect('a');
        }
    )
};

int main() {
    ltest.run();
    ltest.threads(4).run({"manageFixture vi1","manageFixture vi2","manageFixture vi3"});
    return 0;
}

};

#endif // DATATEST_H_INCLUDED
