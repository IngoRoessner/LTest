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


#ifndef MUTESTREAM_H_INCLUDED
#define MUTESTREAM_H_INCLUDED

#include <iostream>
#include <sstream>
#include <map>
#include <memory>
#include <mutex>
#include "MultiThreadStreambuf.h"

using namespace std;

enum VerboseMode{
    EVERYTHING,
    FAIL,
    NONE
};

class MuteStream{
    private:
        VerboseMode _mode;
        ostream& mutedStream;
        MultiThreadStreambuf* buffer;
        string output;

    public:
        mutex mutestreammutex;

        MuteStream(ostream& os = cout, VerboseMode mode = VerboseMode::FAIL):mutedStream(os){
            _mode = mode;
            MultiThreadStreambuf::addToOutstream(os);
            buffer = MultiThreadStreambuf::getFromOutstream(os);
        }

        void start(){
            MultiThreadStreambuf::addToOutstream(mutedStream, buffer);
        }

        void stop(){
            buffer = MultiThreadStreambuf::getFromOutstream(mutedStream);
        }

        void print(){
            mutedStream<<output<<endl;
            output="";
        }

        string flush(string testName, bool testFailed){
            mutestreammutex.lock();
        	string tempoutput = buffer->flushThisThreadsOutput();
            if(_mode == VerboseMode::EVERYTHING || (testFailed && _mode == VerboseMode::FAIL)){
                if(tempoutput.size()){
                    output += "----------------------------------\n";
                    output += "OUTPUT: "+testName+"\n";
                    output += "----------------------------------\n";
                    output += tempoutput + "\n\n";
                }
            }
            mutestreammutex.unlock();
            return tempoutput;
        }
};

class MuteStreamMap: public map<ostream*, shared_ptr<MuteStream>>{
private:
    mutex mutestreammapmutex;
public:

    void setVerboseMode(ostream& os, VerboseMode mode){
        mutestreammapmutex.lock();
        MuteStreamMap& that = *this;
        that[&os] = shared_ptr<MuteStream>(new MuteStream(os, mode));
        mutestreammapmutex.unlock();
    }

    void start(){
        for (MuteStreamMap::iterator it=this->begin(); it!=this->end(); ++it){
            it->second->start();
        }
    }

    void stop(){
        for (MuteStreamMap::iterator it=this->begin(); it!=this->end(); ++it){
            it->second->stop();
        }
    }

    void print(){
        for (MuteStreamMap::iterator it=this->begin(); it!=this->end(); ++it){
            it->second->print();
        }
    }

    map<ostream*, string> flush(string testName, bool testFailed){
        mutestreammapmutex.lock();
    	map<ostream*, string> returnable;
        for (MuteStreamMap::iterator it=this->begin(); it!=this->end(); ++it){
            string&& flushed_output = it->second->flush(testName, testFailed);
            returnable.emplace(it->first, move(flushed_output));
        }
        mutestreammapmutex.unlock();
        return returnable;
    }

};

#endif // MUTESTREAM_H_INCLUDED
