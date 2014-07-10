#ifndef FUNCTION_TRAITS_H_INCLUDED
#define FUNCTION_TRAITS_H_INCLUDED

template<typename T, typename U>
struct same{
    static const bool value = false;
};

template<typename T>
struct same<T, T>{
    static const bool value = true;
};

template<bool B, class T = void>
struct enableIf {};

template<class T>
struct enableIf<true, T> { typedef T type; };

//No need in c++ 03 (no Lambdas)
#if __cplusplus > 199711L
template <typename T>
struct function_traits : public function_traits<decltype(&T::operator())>
{};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const>
{
    typedef ReturnType ret;
};
#else
template <typename T>
struct function_traits;
#endif // __cplusplus

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
struct RetIs : enableIf<same<typename function_traits<Functor>::ret, ret>::value, string>
{};

//Using
/*
template<typename Functor>
typename RetIs<Functor, void>::type foo(Functor funct){
    cout<<"void"<<endl;
}

template<typename Functor>
typename RetIs<Functor, bool>::type foo(Functor funct){
    cout<<"bool"<<endl;
}
*/

#endif // FUNCTION_TRAITS_H_INCLUDED
