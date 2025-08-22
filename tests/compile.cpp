#ifdef PROG_COMPILER_GCC
    #include "../src/unicpp/unicpp.h"
#else
    #include <unicpp.h>
#endif

ER Main()
{
    ConWrite("Hello, world!");
    rets;
}
