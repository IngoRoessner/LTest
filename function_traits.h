#ifndef FUNCTION_TRAITS_H_INCLUDED
#define FUNCTION_TRAITS_H_INCLUDED

#include <type_traits>

template <typename T>
struct function_traits : public function_traits<decltype(&T::operator())>
{};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const>
{
    typedef ReturnType ret;
};

template <>
struct function_traits<void(*)()>
{
    typedef void(*ret)();
};

template <>
struct function_traits<bool(*)()>
{
    typedef bool(*ret)();
};


template<typename Functor, typename ret>
struct FunctionTypeIs : enable_if<is_same<typename function_traits<Functor>::ret, ret>::value, string>
{};

#endif // FUNCTION_TRAITS_H_INCLUDED
