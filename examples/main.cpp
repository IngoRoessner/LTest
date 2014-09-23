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

#define ALL
//ALL || LEAP_YEAR || MINIMAL || IGNORES || PARAMTEST || RESULTSET


#if defined(LEAP_YEAR)
    #include "leap_year.h"
#elif defined(MINIMAL)
    #define EXAMPLE minimal
    #include "minimal.h"
#elif defined(IGNORES)
    #define EXAMPLE ignores
    #include "ignores.h"
#elif defined(PARAMTEST)
    #define EXAMPLE parametertest
    #include "parametertest.h"
#elif defined(RESULTSET)
    #define EXAMPLE resultset
    #include "resultset.h"
#elif defined(ALL)
    #define EXAMPLE all
    #include "leap_year.h"
    #include "minimal.h"
    #include "ignores.h"
    #include "parametertest.h"
    #include "resultset.h"
    #include <fstream>

    namespace all{
        int main() {
            minimal::main();
            ignores::main();
            parametertest::main();
            resultset::main();
            leap_year::main();
            std::ofstream xmlfile("bin/TEST-ExampleTestsResults.xml");
            ltest.outstream(xmlfile).format(Format::Xml).run();
            xmlfile.close();
            return 0;
        }
    }

#endif // EXAMPLE

int main(){
    //LTest::setStreamCaptureMode(std::cout, CaptureMode::DIRECTOUT);
    EXAMPLE::main();
}
