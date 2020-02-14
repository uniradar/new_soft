#ifndef PUBLICSURVEILLANCE_H
#define PUBLICSURVEILLANCE_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define HOST        "127.0.0.1"
#define PORT        9969
#define MAXTIME     4096

using namespace std;

void* sendHeart(void* arg);

enum e_Type {HEART, INFO , OTHER};

struct s_PACKET_HEAD
{
    e_Type type;
    int length;
};

struct s_moudleDate{
    s_PACKET_HEAD head;
    int PID;
    char createTime[19];    //格式为MM-DD HH:MM:SS共14位
    int state;
    char moudleName[64];
    int fd;
    char date[256];
};

class connectMonitor
{
public:

    connectMonitor(string moudleName="noName");
    ~connectMonitor();

    void getInfo();
    int sendInfo(string date);
    int sendNormalExit();
    void initializeMoudleDate(s_moudleDate* moudledate);
    //心跳包相关
    void connectServer();
    void startJump();
    friend void* sendHeart(void* arg);

private:

    //心跳包相关
    int sockfd;
    bool isSendInfo;
    sockaddr_in serverAddress;
    socklen_t serverAddressLen;

    s_moudleDate moudledate;

};

#endif
