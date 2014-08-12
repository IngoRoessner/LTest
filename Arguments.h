#ifndef ARGUMENTS_H_INCLUDED
#define ARGUMENTS_H_INCLUDED

#include "DataFunction.h"
#include "function_traits.h"
#include <tuple>
#include <exception>

using namespace std;


template<typename... Types>
class Arguments{
    DataFunctionBase* dataFunction;
    tuple<Types...> storedArgs;
    bool dataFuncReturnsVoid;
public:

    Arguments(DataFunctionBase* datafunc, Types... args):dataFunction(datafunc), storedArgs(args...) {
        dataFuncReturnsVoid = datafunc->isVoidReturn();
        if(dataFuncReturnsVoid){
            (dynamic_cast<DataFunction<void, Types...>*>(dataFunction))->run(args...);
        }
    }

    template<typename Ret>
    void expect(Ret expectedValue){
        if(dataFuncReturnsVoid){
            throw logic_error("void function cant expect anything");
        }
        (dynamic_cast<DataFunction<Ret, Types...>*>(dataFunction))->runWithTuple(expectedValue, storedArgs);
    }
};

#endif // ARGUMENTS_H_INCLUDED
