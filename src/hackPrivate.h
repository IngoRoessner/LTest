#ifndef __privateCall_H_
#define __privateCall_H_


template <class Tag>
struct PtrStore
{
    static typename Tag::type value;
};
template <class Tag>
typename Tag::type PtrStore<Tag>::value;

template <class Tag>
using AccessPrivate = PtrStore<Tag>;

//#################################################
//  User Interface
//#################################################

template <class Tag, typename Tag::type x>
struct StorePrivateFunction
{
    StorePrivateFunction() { PtrStore<Tag>::value = x; }
    static StorePrivateFunction instance;
};

template <class Tag, typename Tag::type x>
StorePrivateFunction<Tag,x> StorePrivateFunction<Tag,x>::instance;

template<class ClassType, typename ReturnValue, typename... ParameterTypes>
struct CreateTag{
    typedef ReturnValue(ClassType::*type)(ParameterTypes...);
    typedef ClassType ObjectType;
    typedef ReturnValue ReturnType;

    static ReturnValue call(ClassType object, ParameterTypes&&... args){
        (object.*AccessPrivate<CreateTag>::value)(args...);
    }
};

template<int id_, class ClassType, typename ReturnValue, typename... ParameterTypes>
struct CreateIdTag{
    static constexpr int id = id_;
    typedef ReturnValue(ClassType::*type)(ParameterTypes...);
    typedef ClassType ObjectType;
    typedef ReturnValue ReturnType;

    static ReturnValue call(ClassType object, ParameterTypes&&... args){
        (object.*AccessPrivate<CreateIdTag>::value)(args...);
    }
};


#endif //__privateCall_H_
