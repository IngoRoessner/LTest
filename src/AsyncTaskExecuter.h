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

        void addToResults(ResultType& element){
            resultsMutex.lock();
            results.push_back(element);
            resultsMutex.unlock();
        }

    };

}

#endif // LTESTTHREADPOOL_H_INCLUDED
