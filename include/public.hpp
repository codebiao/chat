#ifndef PUBLIC_H
#define PUBLIC_H
/*
    server 和 client的公共文件
*/
enum EnMsgType
{
    LOGIN_MSG = 1,  // 登录消息
    LOGIN_MSG_ACK,  // 登录相应消息
    REG_MSG,        // 注册消息
    REG_MSG_ACK,    // 注册响应消息
    ONE_CHAT_MSG,   // 一对一聊天
    ADD_FRIEND_MSG, // 添加好友
};

#endif