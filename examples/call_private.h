#ifndef __private_call_H_
#define __private_call_H_

#include <iostream>
#include "../src/LTest.h"
#include "../src/hackPrivate.h"

class ClassWithPrivateFunctions {
    int x;

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
};

typedef CreateTag<ClassWithPrivateFunctions, void> voidVoidFooTag;
template class StorePrivateFunction<voidVoidFooTag, &ClassWithPrivateFunctions::voidVoidFoo>;

typedef CreateIdTag<1, ClassWithPrivateFunctions, int, int> iiFooTag;
template class StorePrivateFunction<iiFooTag, &ClassWithPrivateFunctions::iiFoo>;

typedef CreateTag<ClassWithPrivateFunctions, double, double, int> ddiFooTag;
template class StorePrivateFunction<ddiFooTag, &ClassWithPrivateFunctions::ddiFoo>;

typedef CreateIdTag<2, ClassWithPrivateFunctions, int, int> gsXTag;
template class StorePrivateFunction<gsXTag, &ClassWithPrivateFunctions::getAndSetX>;

namespace private_call{

    TestSuite PrivateCallTests = {
            ltest().addTest("void void", [](){
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
            })
    };


    int main() {
        ltest().run();
        return 0;
    }

};


#endif //__private_call_H_
