#ifndef MUTESTREAM_H_INCLUDED
#define MUTESTREAM_H_INCLUDED

#include <iostream>
#include <sstream>
#include <map>
#include <memory>

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

        string flushOut(string testName, bool testFailed){
        	string output = "";
            mutedStream.rdbuf(mutedStreamRdbuf);
            if(muteMode == MuteMode::EVERYTHING || (testFailed && muteMode == MuteMode::FAIL)){
                output = mutedStreamBuffer.str();
                if(output.size()){
                    mutedStream<<"-------------- OUTPUT START: "<<testName<<" ----------------"<<endl;
                    mutedStream<<output<<endl;
                    mutedStream<<"--------------- OUTPUT END: "<<testName<<" -----------------"<<endl;
                }
            }
            mutedStreamBuffer.str("");
            return output;
        }

};

class MuteStreamMap: public map<ostream*, unique_ptr<MuteStream>>{
public:

    void setMuteMode(ostream& os, MuteMode mode){
        MuteStreamMap& that = *this;
        that[&os] = unique_ptr<MuteStream>(new MuteStream(os, mode));
    }

    void muteOut(){
        for (MuteStreamMap::iterator it=this->begin(); it!=this->end(); ++it){
            it->second->muteOut();
        }
    }

    map<ostream*, string> flushOut(string testName, bool testFailed){
    	map<ostream*, string> returnable;
        for (MuteStreamMap::iterator it=this->begin(); it!=this->end(); ++it){
            string&& flushed_output = it->second->flushOut(testName, testFailed);
            returnable.emplace(it->first, move(flushed_output));
        }
        return returnable;
    }

};

#endif // MUTESTREAM_H_INCLUDED
