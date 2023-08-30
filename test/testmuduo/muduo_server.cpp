/*
    TcpServer：用于编写服务器程序的
    TcpClient：用于编写客户端程序的

    epoll + 线程池
    好处：将网络I/O的代码和业务代码区分开
        用户的连接和断开、用户的可读写事件
*/
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <string>
#include <functional>
using namespace std;
using namespace muduo::net;
using namespace placeholders;

/*  基于muduo网络库开发服务器程序
    1、组合TcpServer对象
    2、创建EventLoop事件循环对象的指针
    3、明确TcpServer构造函数需要什么参数，输出ChatServer
    4、注册处理连接、读写时间的回调函数
    5、设置合适的线程数量。
*/
class ChatServer
{
public:
    ChatServer(EventLoop *loop,               // 事件循环
               const InetAddress &listenAddr, // ip + port
               const string &nameArg)         // 服务器名字
        : _server(loop, listenAddr, nameArg), _loop(loop)
    {
        // 给服务器注册用户连接的创建和断开回调
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

        // 给服务器注册用户读写事件回调
        _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

        // 设置服务器端的线程数量    //1个IO线程，3个work线程
        _server.setThreadNum(4);
    }
    // 开启事件循环
    void start()
    {
        _server.start();
    }

private:
    // 专门处理用户的创建和断开
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            cout << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort() << " state:online" << endl;
        }
        else
        { // 连接断开了
            cout << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort() << " state:offline" << endl;
            conn->shutdown(); // close(fd)
        }
    }
    // 专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr &conn, // 连接
                   Buffer *buffer,               // 缓冲区
                   muduo::Timestamp time)        // 接受到数据的事件信息
    {
        string buf = buffer->retrieveAllAsString();
        cout << "recv data：" << buf << " time:" << time.toString() << endl;
        conn->send(buf);
    }

    TcpServer _server; // #1
    EventLoop *_loop;  // #2
};

int main()
{
    EventLoop eventLoop;
    InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&eventLoop, addr, "chart server");
    server.start();   // listenfd epoll_ctl => epoll
    eventLoop.loop(); // 相当于epoll_wait，以阻塞方式等待新用户连接

    return 0;
}