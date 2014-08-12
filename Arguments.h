#ifndef ARGUMENTS_H_INCLUDED
#define ARGUMENTS_H_INCLUDED

#include "DataFunction.h"
#include "function_traits.h"
#include <tuple>

using namespace std;

template<typename... Types>
class Arguments{
    DataFunctionBase* dataFunction;
    tuple<Types...> storedArgs;
public:

    Arguments(DataFunctionBase* datafunc, Types... args)
        :dataFunction(datafunc), storedArgs(args...) {}

    template<typename Ret>
    void expect(Ret expectedValue){
        (dynamic_cast<DataFunction<Ret, Types...>*>(dataFunction))->runWithTuple(expectedValue, storedArgs);
    }
};

#endif // ARGUMENTS_H_INCLUDED
