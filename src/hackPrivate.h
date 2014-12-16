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

////////////////////////////////////////////////////
//  User Interface
////////////////////////////////////////////////////

template <class Tag, typename Tag::type x>
struct StorePrivate
{
    StorePrivate() { PtrStore<Tag>::value = x; }
    static StorePrivate instance;
};

template <class Tag, typename Tag::type x>
StorePrivate<Tag,x> StorePrivate<Tag,x>::instance;

template<class ClassType, typename ReturnValue, typename... ParameterTypes>
struct CreateTag{
    typedef ReturnValue(ClassType::*type)(ParameterTypes...);
    typedef ClassType ObjectType;
    typedef ReturnValue ReturnType;

    static ReturnValue call(ClassType& object, ParameterTypes&&... args){
        (object.*AccessPrivate<CreateTag>::value)(args...);
    }
};

template<int id_, class ClassType, typename ReturnValue, typename... ParameterTypes>
struct CreateIdTag{
    static constexpr int id = id_;
    typedef ReturnValue(ClassType::*type)(ParameterTypes...);
    typedef ClassType ObjectType;
    typedef ReturnValue ReturnType;

    static ReturnValue call(ClassType& object, ParameterTypes&&... args){
        (object.*AccessPrivate<CreateIdTag>::value)(args...);
    }
};

template<class ClassType, typename FieldType, int id_ = 0>
struct CreateFieldTag{
    static constexpr int id = id_;
    typedef FieldType ClassType::* type;

    static FieldType& get(ClassType& object){
        return (object.*AccessPrivate<CreateFieldTag>::value);
    }
};



#endif //__privateCall_H_
