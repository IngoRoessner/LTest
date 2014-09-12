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

using namespace std;

enum VerboseMode{
    EVERYTHING,
    FAIL,
    NONE
};

class MuteStream{
    private:
        VerboseMode _mode;
        ostringstream mutedStreamBuffer;
        streambuf* mutedStreamRdbuf;
        ostream& mutedStream;
        mutex mutestreammutex;

    public:

        MuteStream(ostream& os = cout, VerboseMode mode = VerboseMode::FAIL):mutedStream(os){
            _mode = mode;
        }

        void mute(){
            mutestreammutex.lock();
            if(mutedStreamRdbuf == NULL){
                mutedStreamBuffer.str("");
                mutedStreamRdbuf = mutedStream.rdbuf();
                mutedStream.rdbuf(mutedStreamBuffer.rdbuf());
            }
            mutestreammutex.unlock();
        }

        string flush(string testName, bool testFailed){
            mutestreammutex.lock();
        	string output = "";
            mutedStream.rdbuf(mutedStreamRdbuf);
            output = mutedStreamBuffer.str();
            if(_mode == VerboseMode::EVERYTHING || (testFailed && _mode == VerboseMode::FAIL)){
                if(output.size()){
                    mutedStream<<"-------------- OUTPUT START: "<<testName<<" ----------------"<<endl;
                    mutedStream<<output<<endl;
                    mutedStream<<"--------------- OUTPUT END: "<<testName<<" -----------------"<<endl;
                }
            }
            mutedStreamBuffer.str("");
            mutestreammutex.unlock();
            return output;
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

    void mute(){
        mutestreammapmutex.lock();
        for (MuteStreamMap::iterator it=this->begin(); it!=this->end(); ++it){
            it->second->mute();
        }
        mutestreammapmutex.unlock();
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
