## 简介
这是一个用C++编写的多线程web服务器，采用了Main Ractor+Sub Ractor的整体架构，支持get，head请求，http长连接，管线化，采用异步日志记录服务器信息。

## 环境

 - OS: Ubuntu 14.04
 - Complier: g++ 5.4

## 配置
 - 默认端口：8888
 - 默认子Ractor数量：4
 - 默认日志文件路径：./WebServer.log

## 主要技术要点
[具体介绍](https://github.com/chenlujiu/server/blob/master/%E5%B9%B6%E5%8F%91%E7%B3%BB%E7%BB%9F.md)

 - epoll+边沿触发+非阻塞socket+ractor
 - 使用多线程提高多核cpu利用率
 - 主线程负责接收连接，并且把clientsocket依据round robin模式分发给子线程
 - 使用eventfd实现了线程的异步唤醒
 - 使用智能指针管理主要的资源，防止内存泄漏
 - 使用状态机解析HTTP请求
 - 使用string完成简单的缓冲区
 - 使用小根堆定时器处理超时请求

## 行为模式

![Image text](https://github.com/chenlujiu/server/blob/master/model.png)
