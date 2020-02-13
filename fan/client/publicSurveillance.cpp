#include "publicSurveillance.h"

connectMonitor::connectMonitor(string moudleName)
{
    //配置服务器地址及Socket
    bzero(&serverAddress,sizeof(serverAddress));
    serverAddress.sin_family=AF_INET;
     if(inet_pton(AF_INET, HOST, &serverAddress.sin_addr) == 0)
    {
        cout << "Server IP Address Error!";
        exit(1);
    }
    serverAddress.sin_port=htons(PORT);
    serverAddressLen=sizeof(serverAddress);

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        cout<<"Create Socket Failed!"<<endl;
        exit(2);
    }
    
}

connectMonitor::~connectMonitor()
{
    
}

void connectMonitor::connectServer()
{
    //连接服务器
    cout << "Connecting......" << endl;
    if(connect(sockfd, (struct sockaddr*)&serverAddress, serverAddressLen) < 0)
    {
        cout << "Can not Connect to Server IP!";
        exit(3);
    }
    else
        cout << "Connect to Server successfully." << endl;
}

void connectMonitor::startJump()
{
    pthread_t id;
    int ret = pthread_create(&id, NULL, sendHeart, (void*)this);
    if(ret != 0)
    {
        cout << "Can not create thread!";
        exit(1);
    }
}

void* sendHeart(void* arg)
{
    cout << "The heartbeat sending thread started.\n";
    connectMonitor* c = (connectMonitor*)arg;
    int count = 0;  // 测试
    while(1) 
    {
        s_PACKET_HEAD head;
        head.type = HEART;
        head.length = 0;    
        send(c->sockfd, &head, sizeof(head), 0);
        sleep(3);     // 定时3秒
 
        ++count;      // 测试：发送15次心跳包就停止发送
        if(count > 15)
            break;
    }
}

void connectMonitor::getInfo()
{

}

int connectMonitor::sendInfo()
{

}

int connectMonitor::sendNormalExit()
{

}