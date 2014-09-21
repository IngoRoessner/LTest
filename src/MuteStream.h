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

namespace LTestSource{

    enum CaptureMode{
        EVERYTHING,
        FAIL,
        NONE,
        DIRECTOUT
    };

    class MuteStream{
        private:
            CaptureMode _mode;
            std::ostringstream mutedStreamBuffer;
            std::streambuf* mutedStreamRdbuf;
            std::ostream& mutedStream;

        public:

            MuteStream(std::ostream& os = std::cout, CaptureMode mode = CaptureMode::FAIL):mutedStream(os){
                _mode = mode;
            }

            void mute(){
                mutedStreamBuffer.str("");
                mutedStreamRdbuf = mutedStream.rdbuf();
                mutedStream.rdbuf(mutedStreamBuffer.rdbuf());
            }

            std::string flush(std::string testName, bool testFailed){
                std::string output = "";
                mutedStream.rdbuf(mutedStreamRdbuf);
                if(_mode == CaptureMode::EVERYTHING || (testFailed && _mode == CaptureMode::FAIL)){
                    output = mutedStreamBuffer.str();
                }
                mutedStreamBuffer.str("");
                return output;
            }

    };

    class MuteStreamMap: public std::map<std::ostream*, std::shared_ptr<MuteStream>>{
    public:

        void setCaptureMode(std::ostream& os, CaptureMode mode){
            MuteStreamMap& that = *this;
            if(mode == CaptureMode::DIRECTOUT){
                if(that.count(&os)){
                    that.erase(&os);
                }
            }else{
                that[&os] = std::shared_ptr<MuteStream>(new MuteStream(os, mode));
            }
        }

        void mute(){
            for (MuteStreamMap::iterator it=this->begin(); it!=this->end(); ++it){
                it->second->mute();
            }
        }

        std::map<std::ostream*, std::string> flush(std::string testName, bool testFailed){
            std::map<std::ostream*, std::string> returnable;
            for (MuteStreamMap::iterator it=this->begin(); it!=this->end(); ++it){
                std::string&& flushed_output = it->second->flush(testName, testFailed);
                returnable.emplace(it->first, std::move(flushed_output));
            }
            return returnable;
        }

    };
}
#endif // MUTESTREAM_H_INCLUDED
