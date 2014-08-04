#ifndef TEXTOUTPUT_H_INCLUDED
#define TEXTOUTPUT_H_INCLUDED

#include "OutputFormatBase.h"

template<typename ResultType>
class TextOutput: public OutputFormatBase<ResultType>{
public:
    string run(ResultType resultset){
        return "hallo welt";
    }
};

#endif // TEXTOUTPUT_H_INCLUDED
