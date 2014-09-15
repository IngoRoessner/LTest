#ifndef FUNCTIONPATTERN_H_INCLUDED
#define FUNCTIONPATTERN_H_INCLUDED


namespace functionpattern
{
struct AnyType {};
};


template <typename ReturnType, typename... Parameter>
struct FunctionType
{
    typedef ReturnType returnType;
    typedef tuple<Parameter...> parameterType;
};

template<typename Functor>
struct GetFunctionType: public GetFunctionType<decltype(&Functor::operator())>
{};

template <typename ReturnType, typename... Parameter>
struct GetFunctionType<ReturnType(*)(Parameter...)>
{
    typedef FunctionType<ReturnType, Parameter...> type;
};

template <typename ReturnType, typename ClassType, typename... Parameter>
struct GetFunctionType<ReturnType(ClassType::*)(Parameter...) const>
{
    typedef FunctionType<ReturnType, Parameter...> type;
};


template<typename EnabledRet, typename Functor, typename ReturnType, typename... Parameters>
struct FunctionPattern_
        : enable_if<
        is_same<
        typename GetFunctionType<Functor>::type,
        FunctionType<ReturnType, Parameters...>
        >::value,
        EnabledRet
        >
{};

template<typename EnabledRet, typename Functor, typename... Parameters>
struct FunctionPattern_<EnabledRet, Functor, functionpattern::AnyType, Parameters...>
        : enable_if<
        is_same<
        typename GetFunctionType<Functor>::type::parameterType,
        tuple<Parameters...>
        >::value,
        EnabledRet
        >
{};

template<typename EnabledRet, typename Functor, typename ReturnType>
struct FunctionPattern_<EnabledRet, Functor, ReturnType, functionpattern::AnyType>
        : enable_if<
        is_same<
        typename GetFunctionType<Functor>::type::returnType,
        ReturnType
        >::value,
        EnabledRet
        >
{};

template<typename EnabledRet, typename Functor, typename ReturnType, typename... Parameters>
using FunctionPattern = typename FunctionPattern_<EnabledRet, Functor, ReturnType, Parameters...>::type;


template<bool functionIsSameAsNotThis, typename EnabledRet, typename Functor, typename ReturnType, typename... Parameters>
struct NotFunctionPattern:enable_if<false, void>
{};

template<typename EnabledRet, typename Functor, typename ReturnType, typename... Parameters>
struct NotFunctionPattern<false, EnabledRet, Functor, ReturnType, Parameters...>
        :   FunctionPattern_<EnabledRet, Functor, ReturnType, Parameters...>
{};

template<typename NotThis, typename EnabledRet, typename Functor, typename ReturnType, typename... Parameters>
using FunctionPatternNot = typename NotFunctionPattern<
                           is_same<Functor, NotThis>::value,
                           EnabledRet,
                           Functor,
                           ReturnType,
                           Parameters...>::type;

#endif // FUNCTIONPATTERN_H_INCLUDED
