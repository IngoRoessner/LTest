#ifndef LTESTMISUSEEXCEPTION_H_INCLUDED
#define LTESTMISUSEEXCEPTION_H_INCLUDED

#include <exception>

using namespace std;

class LTestMisuse: logic_error{
public:
    LTestMisuse(string msg):logic_error(msg){}
};

class MissingExpect: LTestMisuse{
public:
    MissingExpect(string msg):LTestMisuse(msg){}
};

class ExpectAtVoid: LTestMisuse{
public:
    ExpectAtVoid(string msg):LTestMisuse(msg){}
};


#endif // LTESTMISUSEEXCEPTION_H_INCLUDED
