#ifndef MONITOR_H
#define MONITOR_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT        9969

using namespace std;

enum e_Type {HEART, INFO, OTHER};
 
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

void* heartListen(void* arg);

class moudleList
{
public:
    moudleList();
    ~moudleList();

    int addItem(s_moudleDate moudleDate);
    int deleteItem();

private:
    s_moudleDate moudleDate;
    
};

class server
{
public:
    
    server(int port=PORT);
    ~server();
    void Bind();    //bind()会与库函数冲突
    void Listen(int queue_len = 20);    //linsten()也会有冲突
    void Accept();
    void Run();
    void Recv(int nums);
    friend void* heartListen(void* arg);

private:

    sockaddr_in serverAddress;
    socklen_t serverAddressLen;
    int listenfd;
    int maxfd;
    timeval timeOut;
    moudleList *list;
    fd_set masterSet;
    fd_set workingSet;
    map<int, pair<string, int> > mmap;
};

#endif