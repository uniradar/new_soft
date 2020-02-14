#include "monitor.h"

moudleList::moudleList()
{
}

moudleList::~moudleList()
{
}

int moudleList::addItem(s_moudleDate moudleDate)
{
    cout<<"add a new item."<<endl;
}

server::server(int port)
{
    list=new moudleList;
    //服务器配置
    bzero(&serverAddress,sizeof(serverAddress));
    serverAddress.sin_family=AF_INET;
    serverAddress.sin_addr.s_addr=htons(INADDR_ANY);
    serverAddress.sin_port=htons(port);
    //创建socket
    listenfd=socket(PF_INET,SOCK_STREAM,0);
    if(listenfd < 0)
    {
        cout << "Create Socket Failed!";
        exit(1);
    }
    //下面的内容暂时不懂
    int opt=1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
}

server::~server()
{
    delete list;
}

void server::Bind()
{
    if(-1 == (bind(listenfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress))))
    {
        cout << "Server Bind Failed!";
        exit(1);
    }
    cout << "Bind Successfully.\n"; 
}

void server::Listen(int queue_len)
{
    if(-1 == listen(listenfd, queue_len))
    {
        cout << "Server Listen Failed!";
        exit(1);
    }
    cout << "Listen Successfully.\n";
}

void server::Accept()
{
    sockaddr_in clientAddress;
    socklen_t clientAddresslen = sizeof(clientAddress);
 
    int new_fd = accept(listenfd, (sockaddr*)&clientAddress, &clientAddresslen);
    if(new_fd < 0)
    {
        cout << "Server Accept Failed!";
        exit(1);
    }
 
    string ip(inet_ntoa(clientAddress.sin_addr));    // 获取客户端IP
 
    cout << ip << " new connection was accepted.\n";
 
    mmap.insert(make_pair(new_fd, make_pair(ip, 0)));
 
    // 将新建立的连接的fd加入master_set
    FD_SET(new_fd, &masterSet);
    if(new_fd > maxfd)
    {
        maxfd=new_fd;
    }
}

void server::Recv(int nums)
{
    for(int fd=0; fd<=maxfd; ++fd)
    {
        if(FD_ISSET(fd, &workingSet))
        {
            bool closeConn = false;  // 标记当前连接是否断开了
 
            s_moudleDate *tmp=new s_moudleDate;
            recv(fd, tmp, sizeof(*tmp), 0);   // 先接受包头
 
            if(tmp->head.type == HEART)
            {
                mmap[fd].second = 0;        // 每次收到心跳包，count置0
                cout << "Received heart-beat from client.\n";
            }
            else if(tmp->head.type==INFO)
            {
                list->addItem(*tmp);
            }   
            else
            {
                cout<<"date:"<<tmp->date<<endl;

                /*cout<<"------------------------"<<endl;
                cout<<tmp->createTime<<endl;
                cout<<tmp->fd<<endl;
                cout<<tmp->head.length<<endl;
                cout<<tmp->head.type<<endl;
                cout<<tmp->moudleName<<endl;
                cout<<tmp->PID<<endl;
                cout<<tmp->state<<endl;*/

            }
            
 
            if(closeConn)  // 当前这个连接有问题，关闭它
            {
                close(fd);
                FD_CLR(fd, &masterSet);
                if(fd == maxfd)  // 需要更新max_fd;
                {
                    while(FD_ISSET(maxfd, &masterSet) == false)
                        --maxfd;
                }
            }
            delete tmp;
        }
    }   
}

void server::Run()
{
    pthread_t id;     // 创建心跳检测线程
    int ret = pthread_create(&id, NULL, heartListen, (void*)this);
    if(ret != 0)
    {
        cout << "Can not create heart-beat checking thread.\n";
    }
 
    maxfd = listenfd;   // 初始化max_fd
    FD_ZERO(&masterSet);
    FD_SET(listenfd, &masterSet);  // 添加监听fd
 
    while(1)
    {
        FD_ZERO(&workingSet);
        memcpy(&workingSet, &masterSet, sizeof(masterSet));
 
        timeOut.tv_sec = 30;
        timeOut.tv_usec = 0;
 
        int nums = select(maxfd+1, &workingSet, NULL, NULL, &timeOut);
        if(nums < 0)
        {
            cout << "select() error!";
            exit(1);
        }
 
        if(nums == 0)
        {
            //cout << "select() is timeout!";
            continue;
        }
 
        if(FD_ISSET(listenfd, &workingSet))
            Accept();   // 有新的客户端请求
        else
            Recv(nums); // 接收客户端的消息
    }
}

void* heartListen(void* arg)
{
    cout << "The heartbeat checking thread started.\n";
    server* s = (server*)arg;
    while(1)
    {
        //接收心跳
        map<int, pair<string, int> >::iterator it = s->mmap.begin();
        for( ; it!=s->mmap.end(); )
        {   
            if(it->second.second == 5)   // 3s*5没有收到心跳包，判定客户端掉线
            {
                cout << "The client " << it->second.first << " has be offline.\n";
 
                int fd = it->first;
                close(fd);            // 关闭该连接
                FD_CLR(fd, &s->masterSet);
                if(fd == s->maxfd)      // 需要更新max_fd;
                {
                    while(FD_ISSET(s->maxfd, &s->masterSet) == false)
                        s->maxfd--;
                }
 
                s->mmap.erase(it++);  // 从map中移除该记录
            }
            else if(it->second.second < 5 && it->second.second >= 0)
            {
                it->second.second += 1;
                ++it;
            }
            else
            {
                ++it;
            }
        }
        sleep(3);   // 定时三秒

    }
}