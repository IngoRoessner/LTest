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




template <std::size_t... I> struct index_sequence {};
template <std::size_t N, std::size_t... I>
struct make_index_sequence : public make_index_sequence<N-1, N-1, I...> {};
template <std::size_t... I>
struct make_index_sequence<0, I...> : public index_sequence<I...> {};

template <typename Function, typename... Types, std::size_t... I>
void apply_(Function&& f, const std::tuple<Types...>& t, index_sequence<I...>){
  f(std::get<I>(t)...);
}

template <typename Function, typename... Types>
void apply(Function&& f, const std::tuple<Types...>& t){
  apply_(f, t, make_index_sequence<sizeof...(Types)>());
}

#endif // FUNCTION_TRAITS_H_INCLUDED
