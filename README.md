# LTest - C++ 11 Lambda Test
LTest (short for LambdaTest) is a C++ 11 test library based on [lambdas](http://en.cppreference.com/w/cpp/language/lambda). 

Because the usage of lambdas, the library was **designed for** compilers supporting the **C++ 11** standard or higher. 
LTest does **not make use of any macro constructs** for the tests. 
Therefore you write **plain C++** test code without any macro magic. 
LTests supports **programmatic access** to the test results and **many output formats** (e.g. xml output jenkins CI servers).

```cpp
#include "LTest.h"

bool is_leap_year(unsigned int year){
        return (year % 4 == 0 and year % 100 != 0) or year % 400 == 0;
}

TestSuite suite = {
     LTest::addTest("test is leap year 2014 : ", [](){
         return !is_leap_year(2014);
     }),
     LTest::addTest("test is leap year 1900 : ", [](){
         return !is_leap_year(1900);
     }),
     LTest::addTest("test is leap year 2000 : ", [](){
         return is_leap_year(2000);
     })
};

int main() {
    LTest::run(suite);
    return 0;
}

```

## Getting Started
LTest consists just of a set of header and implementation files. You can add our source code to your test folder. We are destributing LTest this way to avoid building it for all available plattforms. We only use "normal" features of C++ 11 which should be available on the most plattforms. See developer info for the compilers we already support and tested with.

### How to include and use LTest
We expect that you will create some test code which should be linked to your production code, testing your functionality. 
So in your project you should have an test-src folder which will contain all your (Ltest)-testcode. You should also embed this git-repository into this folder (you can use [git submodules](http://git-scm.com/book/en/Git-Tools-Submodules) to do so). 
Now you can create your testcode like in the example above and run it using a main function. Note that the production code under test can't contain another main function, because you can have only one of them. 
Don't forget to include the folder, in which LTest.h is located, into your compile path (for g++ for example use -I and -L to inlude and link against this folder). 
For more detailed information about using LTest, please visit our wiki!
Happy testing!

## Developer info

LTest Compiles with:

* GNU GCC on Linux (g++ 4.8.2)
* [MinGW](http://www.mingw.org/) 32 and 64 bit

We develop this project using the [CodeBlocks IDE](http://www.codeblocks.org/). 
There is a buildscript called build.sh to also build LTest headless.
Note: As described in the building section you just need to compile LTest with your tests, there is no need to build LTest in front. 
      Developing enhancements and fixes to LTest is therefore quite simple too. We have an LTestUse module, which can be used for easy testing of your changes.

## Colophon
This project in published in terms of the [MIT license](http://opensource.org/licenses/MIT):

<code>
    The MIT License (MIT)

    Copyright (c) 2014 Ingo Rössner
                       Dan Häberlein

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
</code>

### Included libraries

* To export the test results, the creation of XML-XUnit datafile was implemented using the [RapidXml](http://rapidxml.sourceforge.net/) C++ library. Thanks to Marcin Kalicinski for his great work!
