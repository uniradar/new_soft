#include "publicSurveillance.h"

using namespace std;

int main()
{
    connectMonitor conMiro("test");
    conMiro.connectServer();
    conMiro.startJump();
    string sendDate;
    cout<<"input date:"<<endl;
    while(1)
    {
    cin>>sendDate;
    conMiro.sendInfo(sendDate);

    }
    return 1;
}
