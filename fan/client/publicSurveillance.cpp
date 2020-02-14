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
    //初始化模块信息以便发送
    char* temp;
    temp=(char*)moudleName.data();
    for(int i=0;i<moudleName.size();i++)
    {
        moudledate.moudleName[i]=*temp;
        temp++;
    }
    moudledate.PID=getpid();
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep));
    strcpy(moudledate.createTime,tmp);
    moudledate.state=0;
    moudledate.head.type=INFO;
    isSendInfo=false;

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
        s_moudleDate md;
        md.head.type = HEART;
        md.head.length = 0;    
        send(c->sockfd, &md, sizeof(md), 0);
        sleep(3);     // 定时3秒
 
        ++count;      // 测试：发送15次心跳包就停止发送
        /*if(count > 15)
            break;*/
    }
}

void connectMonitor::getInfo()
{

}

int connectMonitor::sendInfo(string date)
{
    if(!isSendInfo)
    {
        moudledate.head.length=sizeof(moudledate);
        send(sockfd,&moudledate,sizeof(moudledate),0);
        isSendInfo=true;
    }
    s_moudleDate* sendDate=new s_moudleDate;
    initializeMoudleDate(sendDate);
    sendDate->head.type=OTHER;
    char *tmp=(char*)date.data();
    for(int i=0;i<date.size();i++)
    {
        sendDate->date[i]=*tmp;
        tmp++;
    }
    sendDate->head.length=sizeof(sendDate);
    send(sockfd,sendDate,sizeof(*sendDate),0);
    delete sendDate;
}

int connectMonitor::sendNormalExit()
{

}

void connectMonitor::initializeMoudleDate(s_moudleDate* moudledate)
{
    moudledate->fd=0;
    moudledate->head.type=OTHER;
    bzero(moudledate->moudleName,sizeof(moudledate->moudleName));
    bzero(moudledate->createTime,sizeof(moudledate->createTime));
    moudledate->PID=0;
    moudledate->state=0;
    bzero(moudledate->date,sizeof(moudledate->date));
}