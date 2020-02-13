#include "publicSurveillance.h"

using namespace std;

int main()
{
    connectMonitor conMiro("test");
    conMiro.connectServer();
    conMiro.startJump();
    return 1;
}
