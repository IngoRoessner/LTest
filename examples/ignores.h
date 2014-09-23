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


#ifndef TESTCASE_H_INCLUDED
#define TESTCASE_H_INCLUDED

#include "../src/LTest.h"
#include <string>
#include <list>

namespace ignores{
using namespace std;

string str = "do your tests";
TestSuite suite = {
    ltest.addTest("Suite testCase", [&](){cout<<"Suite testCase"<<str<<endl;}),
    ltest.addTest("Suite testCase2", [](){cout<<"Suite testCase2"<<endl;}),

    ltest.addTest("Suite fail cout", [](){cout<<"Suite testCase2"<<endl;}),

    ltest.ignoreNext(),
    ltest.addTest("Suite ignored 1", [](){cout<<"Suite ignored 1"<<endl;return false;}),

    ltest.ignore("Suite ignored 2"),
    ltest.addTest("Suite ignored 2", [](){cout<<"Suite ignored 2"<<endl;return false;}),

    ltest.ignoreNext(2),
    ltest.addTest("Suite ignored 3", [](){cout<<"Suite ignored 3"<<endl;return false;}),
    ltest.addTest("Suite ignored 4", [](){cout<<"Suite ignored 4"<<endl;return false;}),

    ltest.addTest("Suite after ignores", [](){cout<<"Suite after ignores"<<endl;}),

    ltest.addTest("Suite with cout and cerr", [](){cout<<"This goes to std out and will be available programmatically"<<endl; cerr << "and this to cerr!"; return true;}),
};

TestSuite ignoreSuite2 = {
    ltest.addTest("throw1", [](){throw string("nope");}),

    ltest.addTest("throw2", [](){throw string("nope");}),
    ltest.addTest("throw3", [](){throw string("nope");}),
};


int main()
{
    ltest.ignore(suite);
    ltest.ignore("throw1");
    ltest.ignore({"throw2", "throw3"});
    ltest.run();
    return 0;
}

};

#endif // TESTCASE_H_INCLUDED
