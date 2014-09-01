#ifndef NET_THREAD_H
#define NET_THREAD_H

#include <memory>
#include <thread>
#include <common/base/util.h>
#include <common/base/thread_queue.h>
#include <common/base/event_loop.h>
#include <common/net/connector_list.h>


namespace sails {
namespace common {
namespace net {


template <typename T> class EpollServer;


// 定义数据队列中的结构
struct TagRecvData
{
    uint32_t        uid;           // 连接标示
    std::string     data;          // 接收到的内容
    std::string     ip;            // 远程连接的ip
    uint16_t        port;          // 远程连接的端口
};

struct TagSendData
{
    char            cmd;            // 命令:'c',关闭fd; 's',有数据需要发送
    uint32_t        uid;            // 连接标示
    std::string     buffer;         // 需要发送的内容
    std::string     ip;             // 远程连接的ip
    uint16_t        port;           // 远程连接的端口
};

template <typename T>
class NetThread {
public:

    // 链接状态
    struct ConnStatus
    {
	std::string     ip;
	int32_t         uid;
	uint16_t        port;
	int             timeout;
	int             iLastRefreshTime;
    };

    // 网络线程运行状态
    enum RunStatus {
	RUNING,
	PAUSE,
	STOPING
    };

    // 状态
    struct ThreadStatus {
	int thread_num;
	RunStatus status;
	long run_time;
	int listen_port;	// only for recv accept
	int connector_num;	
    };

    // 监听端口信息
    struct BindSocketInfo {
	int listen_port;
	long accept_times;
    };
    
    NetThread(EpollServer<T> *server);
    virtual ~NetThread();
    
    // 创建一个epoll的事件循环
    void create_event_loop();
    
    EventLoop* get_event_loop() { return this->ev_loop; }

    // 监听端口
    int bind(int port);

    // 创建一个connector超时管理器
    void setEmptyConnTimeout(int connector_read_timeout=10);
    
    void run();

    void terminate();

    bool isTerminate() { return status == STOPING;}
    
    void join();

    // 接收连接请求
    static void accept_socket_cb(common::event* e, int revents, void* owner);
    // 接收连接数据
    static void read_data_cb(common::event* e, int revents, void* owner);
    
    static void read_pipe_cb(common::event* e, int revents, void* owner);
    
    // 获取连接数
    size_t get_connector_count() { return connector_list.size();}

/*
    // 发送数据,把data放入一个send list中,然后再触发epoll的可写事件
    void send(const std::string &ip, int port, std::string &data);
    
    BindSocketInfo get_listen_socket_info();
    
    ThreadStatus get_thread_status();  
*/
    // 获取服务
    EpollServer<T>* getServer();
protected:
    void accept_socket(common::event* e, int revents);
    
    void read_data(common::event* e, int revents);
    
private:
    EpollServer<T> *server;
    int status;
    std::thread *thread; 	// 
    int listenfd;
    int listen_port;
    
    EventLoop *ev_loop; 	// 事件循环

    ConnectorTimeout* connect_timer; // 连接超时器
    std::list<Timer*> timerList;	// 定时器
    
    ConnectorList connector_list; 
    
    int shutdown;		// 管道(用于关闭服务)
    int notify;			// 管道(用于通知有数据要发送)
    
};




template <typename T>
NetThread<T>::NetThread(EpollServer<T> *server) {
    this->server = server;
    status = NetThread::STOPING;
    thread = NULL;
    listenfd = 0;
    listen_port = 0;
    ev_loop = NULL;
    connector_list.init(10000);
    connect_timer = NULL;
    notify = socket(AF_INET, SOCK_STREAM, 0);
    printf("notify fd:%d\n", notify);
}


template <typename T>
NetThread<T>::~NetThread() {
    
}


template <typename T>
void NetThread<T>::create_event_loop() {
    ev_loop = new EventLoop(this); 
    ev_loop->init();

    // 创建 notify 事件
    sails::common::event notify_ev;
    emptyEvent(notify_ev);
    notify_ev.fd = notify;
    notify_ev.events = sails::common::EventLoop::Event_READ;
    notify_ev.cb = NetThread<T>::read_pipe_cb;

    assert(ev_loop->event_ctl(EventLoop::EVENT_CTL_ADD,
			      &notify_ev));


}

template <typename T>
int NetThread<T>::bind(int port) {
     
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	perror("create listen socket");
	exit(EXIT_FAILURE);
    }
    struct sockaddr_in servaddr, local;
    int addrlen = sizeof(struct sockaddr_in);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    int flag=1,len=sizeof(int); // for can restart right now
    if( setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, len) == -1) 
    {
	perror("setsockopt"); 
	exit(EXIT_FAILURE); 
    }
    ::bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    sails::common::setnonblocking(listenfd);

    printf("listen fd:%d\n", listenfd);

    if(listen(listenfd, 10) < 0) {
	perror("listen");
	exit(EXIT_FAILURE);
    }

    sails::common::event listen_ev;
    emptyEvent(listen_ev);
    listen_ev.fd = listenfd;
    listen_ev.events = sails::common::EventLoop::Event_READ;
    listen_ev.cb = NetThread<T>::accept_socket_cb;

    if(!ev_loop->event_ctl(sails::common::EventLoop::EVENT_CTL_ADD,
				&listen_ev)) {
	fprintf(stderr, "add listen fd to event loop fail");
        exit(EXIT_FAILURE);
    }

    
    return listenfd;
}

template <typename T>
void NetThread<T>::accept_socket_cb(common::event* e, int revents, void* owner) {
    printf("call accept socket cb \n");
    if (owner != NULL) {
	NetThread<T>* net_thread = (NetThread<T>*)owner;
	net_thread->accept_socket(e, revents);
    }
}


template <typename T>
void NetThread<T>::accept_socket(common::event* e, int revents) {
    printf("call accept socket\n");
    if(revents & common::EventLoop::Event_READ) {
	struct sockaddr_in local;
	int addrlen = sizeof(struct sockaddr_in);
	for (;;) {
	    memset(&local, 0, addrlen);

	    int connfd = -1;
	    do {
	        connfd = accept(e->fd, 
				(struct sockaddr*)&local, 
				(socklen_t*)&addrlen);
	    }while((connfd < 0) && (errno == EINTR));

	    printf("connectfd %d\n", connfd);
	    if (connfd > 0) {
		sails::common::setnonblocking(connfd);

		// 新建connector
		std::shared_ptr<common::net::Connector> connector(new common::net::Connector(connfd));
		uint32_t uid = connector_list.getUniqId();
		connector->setId(uid);
		int port = ntohs(local.sin_port);
		connector->setPort(port);
		char sAddr[20] = {'\0'};
		inet_ntop(AF_INET, &(local.sin_addr), sAddr, 20);
		std::string ip = sAddr;
		connector->setIp(ip);

		connector_list.add(connector);
	        connect_timer->update_connector_time(connector);

		
		// 加入event poll中
		sails::common::event ev;
		emptyEvent(ev);
		ev.fd = connfd;
		ev.events = sails::common::EventLoop::Event_READ;
		ev.cb = NetThread<T>::read_data_cb;
		ev.data.u32 = uid;

		printf("add connector\n");
		if(!ev_loop->event_ctl(common::EventLoop::EVENT_CTL_ADD, &ev)){
		    connector_list.del(uid);
		}
	    }else {
		break;
	    }
	}
    }

}


template <typename T>
void NetThread<T>::read_data_cb(common::event* e, int revents, void* owner) {
    if (owner != NULL) {
	NetThread<T>* net_thread = (NetThread<T>*)owner;
	net_thread->read_data(e, revents);
    }
}


template <typename T>
void NetThread<T>::read_data(common::event* ev, int revents) {
    
    if(ev == NULL || ev->fd < 0) {
	return;
    }

    uint32_t uid = ev->data.u32;

    std::shared_ptr<common::net::Connector> connector = connector_list.get(uid);


    if (connector == NULL || connector.use_count() <= 0) {
	return;
    }

    bool readerror = false;

    // read nonblock connfd
    int totalNum = 0;
    while(true) {
	int lasterror = 0;
	int n = 0;
	do {
	    n = connector->read();
	    lasterror = errno;
	}while((n == -1) && (lasterror == EINTR)); // read 调用被信号中断
	

	if (n > 0) {
	    totalNum+=n;
	    if (totalNum >= 4096) { // 大于4k就开始解析,防止数据过多
		this->server->parse();
	    }
	    if (n < READBYTES) { // no data
		break;
	    }else {
		continue;
	    }

	}else if (n == 0) { // client close or shutdown send, and there is no error,  errno will not reset, so don't print errno
	    readerror = true;
	     char errormsg[100];
	    memset(errormsg, '\0', 100);
	    sprintf(errormsg, "read connfd %d, return:%d", connector->get_connector_fd(), n);
	    perror(errormsg);
	    break;
	}else if (n == -1) {
	    if (lasterror == EAGAIN || lasterror == EWOULDBLOCK) { // 没有数据
		break;
	    }else { // read fault
		readerror = true;
		char errormsg[100];
		memset(errormsg, '\0', 100);
		sprintf(errormsg, "read connfd %d, return:%d, errno:%d", connector->get_connector_fd(), n, lasterror);
		perror(errormsg);
		break;
	    }
	}
    }

    if (readerror) {
	if (!connector->isClosed()) {
	    connector->close();
	}
    }

    else {
	
        connect_timer->update_connector_time(connector);// update timeout
	this->server->parse();
	printf("connector:%d, read:%s\n", connector->get_connector_fd(), connector->peek());
    }
    
}



template <typename T>
void NetThread<T>::read_pipe_cb(common::event* e, int revents, void* owner) {
    printf("read pipe cb\n");
    if (owner != NULL) {
	NetThread<T>* net_thread = (NetThread<T>*)owner;
	net_thread->getServer()->process_pipe(e, revents);
    }
}


template <typename T>
void NetThread<T>::setEmptyConnTimeout(int connector_read_timeout) {
    connect_timer = new ConnectorTimeout(connector_read_timeout);
    connect_timer->init(ev_loop);

}

template <typename T>
EpollServer<T>* NetThread<T>::getServer() {
    return this->server;
}

void startEvLoop(EventLoop* ev_loop) {
    ev_loop->start_loop();
}

template <typename T>
void NetThread<T>::run() {
    thread = new std::thread(startEvLoop, ev_loop);
}

template <typename T>
void NetThread<T>::terminate() {
    if (thread != NULL) {
	// 向epoll管理的0号连接发一个终止事件,让epoll wait结束,然后再退出
	ev_loop->stop_loop();
    }
}


template <typename T>
void NetThread<T>::join() {
    if (thread != NULL) {
	thread->join();
    }
}



} // namespace net
} // namespace common
} // namespace sails 



#endif /* NET_THREAD_H */