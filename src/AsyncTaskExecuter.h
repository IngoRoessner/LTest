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

#ifndef LTESTTHREADPOOL_H_INCLUDED
#define LTESTTHREADPOOL_H_INCLUDED

#include <list>
#include <future>
#include <thread>
#include <mutex>
#include <tuple>

namespace LTestSource{

    template<typename ResultType>
    class AsyncTaskExecuter:public std::list<std::function<ResultType()>>{
        std::list<ResultType> results;
        std::mutex tasksMutex;
        std::mutex resultsMutex;

    public:
        std::list<ResultType> execute(unsigned int poolSize){
            AsyncTaskExecuter& that = *this;
            std::list<std::thread*> threads;
            for(unsigned int i = 0; i<poolSize; i++){
                threads.push_back(new std::thread([&](){
                    std::function<ResultType()> task;
                    while(that.getNextTask(task)){
                        that.addToResults(task());
                    }
                }));
            }
            for(std::thread* t : threads){
                t->join();
                delete t;
            }
            std::list<ResultType> tempResults = results;
            results.clear();
            return tempResults;
        }

        bool getNextTask(std::function<ResultType()>& task){
            tasksMutex.lock();
            bool result;
            if(this->size()>0){
                task = this->front();
                this->pop_front();
                result = true;
            }else{
                result = false;
            }
            tasksMutex.unlock();
            return result;
        }

        void addToResults(ResultType element){
            resultsMutex.lock();
            //std::clog<<"add result"<<std::endl;
            results.push_back(element);
            resultsMutex.unlock();
        }

    };

}

#endif // LTESTTHREADPOOL_H_INCLUDED
