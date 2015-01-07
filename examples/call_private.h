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

#ifndef __private_call_H_
#define __private_call_H_

#include <iostream>
#include "../src/LTest.h"
#include "../src/hackPrivate.h"

class ClassWithPrivateFunctions {
    int x;
    int y;
    int z;

    void voidVoidFoo(){
        std::clog << "voidVoidFoo" << std::endl;
    }

    int iiFoo(int i){
        return 2*i;
    }

    double ddiFoo(double d, int i){
        return d/i;
    }

    int getAndSetX(int x){
        int temp = this->x;
        this->x = x;
        return temp;
    }

public:
    void setZ(int z){
        this->z = z;
    }

    void setY(int y){
        this->y = y;
    }
};

typedef CreateTag<ClassWithPrivateFunctions, void> voidVoidFooTag;
template class StorePrivate<voidVoidFooTag, &ClassWithPrivateFunctions::voidVoidFoo>;

typedef CreateIdTag<1, ClassWithPrivateFunctions, int, int> iiFooTag;
template class StorePrivate<iiFooTag, &ClassWithPrivateFunctions::iiFoo>;

typedef CreateTag<ClassWithPrivateFunctions, double, double, int> ddiFooTag;
template class StorePrivate<ddiFooTag, &ClassWithPrivateFunctions::ddiFoo>;

typedef CreateIdTag<2, ClassWithPrivateFunctions, int, int> gsXTag;
template class StorePrivate<gsXTag, &ClassWithPrivateFunctions::getAndSetX>;

typedef CreateFieldTag<ClassWithPrivateFunctions, int> xTag;
template class StorePrivate<xTag, &ClassWithPrivateFunctions::x>;

typedef CreateFieldTag<ClassWithPrivateFunctions, int, 2> yTag;
template class StorePrivate<yTag, &ClassWithPrivateFunctions::y>;

typedef CreateFieldTag<ClassWithPrivateFunctions, int, 3> zTag;
template class StorePrivate<zTag, &ClassWithPrivateFunctions::z>;

namespace private_call{

    TestSuite PrivateCallTests = {
            ltest().addTest("call private method and field", [](){
                ClassWithPrivateFunctions object;

                voidVoidFooTag::call(object);

                LTAssert::True(2 == iiFooTag::call(object, 1));
                LTAssert::True(4 == iiFooTag::call(object, 2));

                LTAssert::True(2.0 == ddiFooTag::call(object, 4.0, 2));

                gsXTag::call(object, 1);
                LTAssert::True(1 == gsXTag::call(object, 2));
                LTAssert::True(2 == gsXTag::call(object, 2));
                LTAssert::True(2 == gsXTag::call(object, 13));
                LTAssert::True(13 == gsXTag::call(object, 0));
                LTAssert::True(0 == xTag::get(object));

                object.setY(5);
                object.setZ(20);
                LTAssert::True(5 == yTag::get(object));
                LTAssert::True(20 == zTag::get(object));
            })
    };


    int main() {
        ltest().run();
        return 0;
    }

};


#endif //__private_call_H_
