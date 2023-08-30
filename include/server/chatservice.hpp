#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <muduo/net/TcpConnection.h>
#include "json.hpp"
#include <unordered_map>
#include <functional>
#include <mutex>

#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"

using namespace std;
using namespace muduo::net;
using namespace muduo;
using json = nlohmann::json;

// 处理消息的事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr &ptr, json &js, Timestamp time)>;

// 聊天服务器业务类  //单例模式
class ChatService
{
public:
    // 获取单例对象的接口函数
    static ChatService *instance();
    // 获取消息对应的获取器
    MsgHandler getHandler(int msgid);
    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &);

    // 服务器异常后，业务重置方法
    void reset();

    // 处理登录业务
    void login(const TcpConnectionPtr &, json &, Timestamp);
    // 处理注册业务
    void reg(const TcpConnectionPtr &, json &, Timestamp);
    // 添加好友业务
    void addFriend(const TcpConnectionPtr &, json &, Timestamp);

    // 一对一聊天业务
    void oneChat(const TcpConnectionPtr &, json &, Timestamp);

    // 群聊业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time); // 创建群组业务
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);    // 加入群组业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);   // 群组聊天业务

private:
    // 单例模式 构造函数
    ChatService();

    // 存储消息id和其对应的处理操作
    unordered_map<int, MsgHandler> _msgHandlerMap;

    // 存储在线用户的通信连接   //注意线程安全
    unordered_map<int, TcpConnectionPtr> _userConnMap;
    // 定义互斥锁，保证_userConnMap的线程安全
    mutex _connMutex;

    // 数据操作类对象
    UserModel _userModel;             // 操作User表的类
    OfflineMsgModel _offlineMsgModel; // 离线消息表的操作类
    FriendModel _friendModel;         // 维护好友信息的操作类
    GroupModel _groupModel;           // 群聊操作类
};

#endif