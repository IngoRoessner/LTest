/*
 *     The MIT License (MIT)
 *
 *     Copyright (c) 2014 Ingo R�ssner
 *                        Dan H�berlein
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


#ifndef ASYNC_H_INCLUDED
#define ASYNC_H_INCLUDED

#include "../src/LTest.h"

namespace async{

    TestSuite asyncTests1 = {
        ltest().addTest("async 1", [](){
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }),

        ltest().addTest("async 2", [](){
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }),

        ltest().addTest("async 3", [](){
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        })
    };

    TestSuite asyncTests2 = {
        ltest().addTest("async 4", [](){
            std::cout<<"start async 4"<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::cout<<"end async 4"<<std::endl;
        }),

        ltest().addTest("async 5", [](){
            std::cout<<"start async 5"<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::cout<<"end async 5"<<std::endl;
        }),

        ltest().addTest("async 6", [](){
            std::cout<<"start async 6"<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::cout<<"end async 6"<<std::endl;
        })
    };

    TestSuite asyncTests3 = {
        ltest().addTest("async 7", [](){
            LTAssert::True(false, "async 7 fail");
        }),

        ltest().addTest("async 8", [](){
            return false;
        }),

        ltest().addTest("async 9", [](int i){
            return i;
        }, [](auto test){
            test.with(2).expect(2);
            test.with(3).expect(2);
        })
    };

    int main() {
        ltest().threads(4).run(asyncTests1);
        ltest().threads(4).run();
        return 0;
    }

};

#endif // ASYNC_H_INCLUDED
