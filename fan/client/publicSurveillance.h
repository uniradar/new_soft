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

struct s_moudleDate{
    int PID;
    char createTime[14];    //格式为MM-DD HH:MM:SS共14位
    int state;
    char moudleName[64];
    int fd;
};

enum e_Type {HEART, OTHER};

struct s_PACKET_HEAD
{
    e_Type type;
    int length;
};

class connectMonitor
{
public:

    connectMonitor(string moudleName="noName");
    ~connectMonitor();

    void getInfo();
    int sendInfo();
    int sendNormalExit();

    //心跳包相关
    void connectServer();
    void startJump();
    friend void* sendHeart(void* arg);

private:

    //心跳包相关
    int sockfd;
    sockaddr_in serverAddress;
    socklen_t serverAddressLen;

    s_moudleDate moudleDate;

};

#endif
