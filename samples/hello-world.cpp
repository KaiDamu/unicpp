#define PROG_BUILD_TYPE_DBG // remove this line for release build
#include <unicpp/main.hpp>
#include <unicpp/console.hpp>

ER Main()
{
    ConWrite("Hello, &e;world!");
    ConWait("&9;Press [ENTER] to exit ", ConWaitAnim::SPINNER);
    rets;
}
