#include "monitor.h"

int main()
{
    server sver(9969);
    sver.Bind();
    sver.Listen();
    sver.Run();
    return 1;
}