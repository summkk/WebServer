#include <sys/epoll.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <vector>
using namespace std;
// #include "HttpData.h"
// #include "Socket.h"
// #include "Timer.h"


class Epoll {

public:
    Epoll();
    ~Epoll();
    void epoll_init(int max_events);

    // static int addfd(int epoll_fd, int fd, __uint32_t events, std::shared_ptr<HttpData>);

    // static int modfd(int epoll_fd, int fd, __uint32_t events, std::shared_ptr<HttpData>);

    static int epoll_del(int epoll_fd, int fd, __uint32_t events);

    // static std::vector<std::shared_ptr<HttpData>>
    // poll(const ServerSocket &serverSocket, int max_event, int timeout);

    // static void handleConnection(const ServerSocket &serverSocket);

public:
    //static std::unordered_map<int, std::shared_ptr<HttpData>> httpDataMap;
    static const int MAX_EVENTS = 100000;
    vector<epoll_event> events;
    //static TimerManager timerManager;
    const static __uint32_t DEFAULT_EVENTS;

};
