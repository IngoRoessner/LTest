#ifndef MULTITHREADSTREAMBUF_H_INCLUDED
#define MULTITHREADSTREAMBUF_H_INCLUDED

#include <map>
#include <unordered_set>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;


class PublicVirtualsBuffer: public stringbuf{
public:
    using streambuf::imbue;
    using streambuf::setbuf;
    using streambuf::seekoff;
    using streambuf::seekpos;
    using streambuf::sync;
    using streambuf::showmanyc;
    using streambuf::xsgetn;
    using streambuf::underflow;
    using streambuf::uflow;
    using streambuf::pbackfail;
    using streambuf::xsputn;
    using streambuf::overflow;
};

class PublicVirtualsBufferStream:public iostream{
public:
    PublicVirtualsBufferStream():iostream(){
        delete this->rdbuf();
        this->rdbuf(new PublicVirtualsBuffer());
    }

};

class MultiThreadStreambuf : public streambuf{
private:
    map<thread::id, PublicVirtualsBufferStream> threadToStream;
    streambuf* originalBufer;
    unordered_set<thread::id> calledIds;
    unordered_set<thread::id> defaultByfForce;
    mutex threadToStreamMutex;

    PublicVirtualsBuffer* getCurrentBuf(){
        threadToStreamMutex.lock();
        PublicVirtualsBufferStream& pvbs = threadToStream[this_thread::get_id()];
        PublicVirtualsBuffer* result = dynamic_cast<PublicVirtualsBuffer*>(pvbs.rdbuf());
        threadToStreamMutex.unlock();
        return result;
    }

    MultiThreadStreambuf(ostream& os){
        originalBufer = os.rdbuf();
    }

public:
    static void addToOutstream(ostream& os, MultiThreadStreambuf* buffer){
        os.rdbuf(buffer);
    }

    static void addToOutstream(ostream& os){
        os.rdbuf(new MultiThreadStreambuf(os));
    }

    static MultiThreadStreambuf* getFromOutstream(ostream& os){
        MultiThreadStreambuf* mtbuf = dynamic_cast<MultiThreadStreambuf*>(os.rdbuf());
        os.rdbuf(mtbuf->originalBufer);
        return mtbuf;
    }

    ~MultiThreadStreambuf(){
        delete originalBufer;
    }

    string flushThisThreadsOutput(){
        threadToStreamMutex.lock();
        PublicVirtualsBufferStream& stream = threadToStream[this_thread::get_id()];

        istreambuf_iterator<char> eos;
        string result(istreambuf_iterator<char>(stream), eos);
        //threadToStream.erase(threadToStream.find(this_thread::get_id()));

        calledIds.insert(this_thread::get_id());
        threadToStreamMutex.unlock();
        return result;
    }

    string flushUnknownTreadOutput(){
        threadToStreamMutex.lock();
        string result = "";
        for(auto& kv : threadToStream){
            if(!calledIds.count(kv.first)){
                istreambuf_iterator<char> eos;
                string temp(istreambuf_iterator<char>(kv.second), eos);
                result += temp + "\n";
            }
        }
        threadToStreamMutex.unlock();
        return result;
    }

    //-------------------------------------------------------
    //overwritten basic_streambuf functions
    //-------------------------------------------------------
protected:
    void imbue(const locale& __loc){
        getCurrentBuf()->imbue(__loc);
    }

    basic_streambuf<char_type,traits_type>* setbuf(char_type* a, streamsize b){
        return getCurrentBuf()->setbuf(a, b);
    }

    pos_type seekoff(off_type a, ios_base::seekdir b, ios_base::openmode c = ios_base::in | ios_base::out){
        return getCurrentBuf()->seekoff(a,b,c);
    }

    pos_type seekpos(pos_type a, ios_base::openmode b = ios_base::in | ios_base::out){
        return getCurrentBuf()->seekpos(a,b);
    }

    int sync() {
        return getCurrentBuf()->sync();
    }

    streamsize showmanyc(){
        return getCurrentBuf()->showmanyc();
    }

    streamsize xsgetn(char_type* __s, streamsize __n){
        return getCurrentBuf()->xsgetn(__s, __n);
    }

    int_type underflow(){
        return getCurrentBuf()->underflow();
    }

    int_type uflow(){
        return getCurrentBuf()->uflow();
    }

    int_type pbackfail(int_type __c  = traits_type::eof()){
        return getCurrentBuf()->pbackfail(__c);
    }

    streamsize xsputn(const char_type* __s, streamsize __n){
        return getCurrentBuf()->xsputn(__s, __n);
    }

    int_type overflow(int_type __c  = traits_type::eof()){
        return getCurrentBuf()->overflow(__c);
    }
};



#endif // MULTITHREADSTREAMBUF_H_INCLUDED
