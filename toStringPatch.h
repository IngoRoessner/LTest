#ifndef TOSTRINGPATCH_H_INCLUDED
#define TOSTRINGPATCH_H_INCLUDED

#include <string>
#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

#endif // TOSTRINGPATCH_H_INCLUDED
