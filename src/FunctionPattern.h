/*
 *     The MIT License (MIT)
 *
 *     Copyright (c) 2014 Ingo Rössner
 *                        Dan Häberlein
 *
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 */


#ifndef FUNCTIONPATTERN_H_INCLUDED
#define FUNCTIONPATTERN_H_INCLUDED


namespace LTestSource
{
    struct AnyType {};

    template <typename ReturnType, typename... Parameter>
    struct FunctionType
    {
        typedef ReturnType returnType;
        typedef std::tuple<Parameter...> parameterType;
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
            : std::enable_if<
            std::is_same<
            typename GetFunctionType<Functor>::type,
            FunctionType<ReturnType, Parameters...>
            >::value,
            EnabledRet
            >
    {};

    template<typename EnabledRet, typename Functor, typename... Parameters>
    struct FunctionPattern_<EnabledRet, Functor, AnyType, Parameters...>
            : std::enable_if<
            std::is_same<
            typename GetFunctionType<Functor>::type::parameterType,
            std::tuple<Parameters...>
            >::value,
            EnabledRet
            >
    {};

    template<typename EnabledRet, typename Functor, typename ReturnType>
    struct FunctionPattern_<EnabledRet, Functor, ReturnType, AnyType>
            : std::enable_if<
            std::is_same<
            typename GetFunctionType<Functor>::type::returnType,
            ReturnType
            >::value,
            EnabledRet
            >
    {};

    template<typename EnabledRet, typename Functor, typename ReturnType, typename... Parameters>
    using FunctionPattern = typename FunctionPattern_<EnabledRet, Functor, ReturnType, Parameters...>::type;

};
#endif // FUNCTIONPATTERN_H_INCLUDED
