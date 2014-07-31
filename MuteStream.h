#ifndef MUTESTREAM_H_INCLUDED
#define MUTESTREAM_H_INCLUDED

#include <iostream>
#include <sstream>
#include <map>

using namespace std;

enum MuteMode{
    EVERYTHING,
    FAIL,
    NONE
};

class MuteStream{
    private:
        MuteMode muteMode;
        ostringstream mutedStreamBuffer;
        streambuf* mutedStreamRdbuf;
        ostream& mutedStream;

    public:

        MuteStream(ostream& os = cout, MuteMode mode = MuteMode::FAIL):mutedStream(os){
            muteMode = mode;
        }

        void muteOut(){
            mutedStreamBuffer.str("");
            mutedStreamRdbuf = mutedStream.rdbuf();
            mutedStream.rdbuf(mutedStreamBuffer.rdbuf());
        }

        void flushOut(string testName, bool testFailed){
            mutedStream.rdbuf(mutedStreamRdbuf);
            if(muteMode == MuteMode::EVERYTHING || (testFailed && muteMode == MuteMode::FAIL)){
                string output = mutedStreamBuffer.str();
                if(output.size()){
                    mutedStream<<"-------------- OUTPUT START: "<<testName<<" ----------------"<<endl;
                    mutedStream<<output<<endl;
                    mutedStream<<"--------------- OUTPUT END: "<<testName<<" -----------------"<<endl;
                }
            }
            mutedStreamBuffer.str("");
        }

};

class MuteStreamMap: public map<ostream*, MuteStream*>{
public:

    void setMuteMode(ostream& os, MuteMode mode){
        MuteStreamMap& that = *this;
        that[&os] = new MuteStream(os, mode);
    }

    void muteOut(){
        for (MuteStreamMap::iterator it=this->begin(); it!=this->end(); ++it){
            it->second->muteOut();
        }
    }

    void flushOut(string testName, bool testFailed){
        for (MuteStreamMap::iterator it=this->begin(); it!=this->end(); ++it){
            it->second->flushOut(testName, testFailed);
        }
    }
};

#endif // MUTESTREAM_H_INCLUDED
