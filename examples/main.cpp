
//ALL || LEAP_YEAR || MINIMAL || IGNORES || PARAMTEST
#define ALL


#if defined(LEAP_YEAR)
    #include "leap_year.h"
#elif defined(MINIMAL)
    #define EXAMPLE minimal
    #include "minimal.h"
#elif defined(IGNORES)
    #define EXAMPLE ignores
    #include "ignores.h"
#elif defined(PARAMTEST)
    #define EXAMPLE parametertest
    #include "parametertest.h"
#elif defined(ALL)
    #define EXAMPLE all
    #include "leap_year.h"
    #include "minimal.h"
    #include "ignores.h"
    #include "parametertest.h"

    namespace all{
        int main() {
            minimal::main();
            ignores::main();
            parametertest::main();
            leap_year::main();
            return 0;
        }
    }

#endif // EXAMPLE

int main(){
    EXAMPLE::main();
}
