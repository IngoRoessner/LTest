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
