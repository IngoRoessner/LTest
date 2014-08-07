#ifndef OUTPUTFORMATBASE_H_INCLUDED
#define OUTPUTFORMATBASE_H_INCLUDED

#include <sstream>
#include <string>

template<typename T>
std::string util_toString(const T& element){
    std::stringstream toStr;
    toStr<<element;
    return toStr.str();
}

template<typename ResultType>
class OutputFormatBase{
public:
    virtual std::string run(ResultType resultset)=0;
    virtual ~OutputFormatBase(){};
};

#endif // OUTPUTFORMATBASE_H_INCLUDED
