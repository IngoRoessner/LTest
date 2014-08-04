#ifndef OUTPUTFORMAT_H_INCLUDED
#define OUTPUTFORMAT_H_INCLUDED

#include "OutputFormatBase.h"
#include "TextOutput.h"

using namespace std;

enum Format{
    Text
};

template<typename ResultType>
class GetOutputFormat{
    OutputFormatBase<ResultType>* output;
public:
    GetOutputFormat(Format format){
        switch (format){
            case Text:
                output = new TextOutput<ResultType>();
                break;
            default:
                throw "their is no such format";
        }
    }

    ~GetOutputFormat(){
        delete output;
    }

    string run(ResultType resultset){
        return output->run(resultset);
    }
};



#endif // OUTPUTFORMAT_H_INCLUDED
