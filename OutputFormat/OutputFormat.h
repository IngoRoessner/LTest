#ifndef OUTPUTFORMAT_H_INCLUDED
#define OUTPUTFORMAT_H_INCLUDED

#include "OutputFormatBase.h"
#include "TextOutput.h"
#include "Format.h"
#include "XmlOutput.h"

using namespace std;

template<typename ResultType>
class GetOutputFormat{
    OutputFormatBase<ResultType>* output;
public:
    GetOutputFormat(Format format){
        switch (format){
            case Format::Text:
                output = new TextOutput<ResultType>();
                break;
            case Format::Xml:
                output = new XmlOutput<ResultType>();
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
