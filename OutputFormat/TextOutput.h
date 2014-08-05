#ifndef TEXTOUTPUT_H_INCLUDED
#define TEXTOUTPUT_H_INCLUDED

#include "OutputFormatBase.h"
#include <sstream>

template<typename ResultType>
class TextOutput: public OutputFormatBase<ResultType>{
private:
    string getOK(ResultType resultset){
        string result = "";
        for(auto elementPtr : resultset.getOK()){
            stringstream toStr;
            toStr<<elementPtr->get_time_taken();
            string time= toStr.str();
            result += elementPtr->get_testname()+": OK ("+time+" sec)\n";
        }
        return result;
    }

public:
    string run(ResultType resultset){
        return getOK(resultset);
    }
};

#endif // TEXTOUTPUT_H_INCLUDED
