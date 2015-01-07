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
        return (object.*AccessPrivate<CreateTag>::value)(args...);
    }
};

template<int id_, class ClassType, typename ReturnValue, typename... ParameterTypes>
struct CreateIdTag{
    static constexpr int id = id_;
    typedef ReturnValue(ClassType::*type)(ParameterTypes...);
    typedef ClassType ObjectType;
    typedef ReturnValue ReturnType;

    static ReturnValue call(ClassType& object, ParameterTypes&&... args){
        return (object.*AccessPrivate<CreateIdTag>::value)(args...);
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
