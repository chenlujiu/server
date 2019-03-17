// @Author Chenlujiu
// @Email 434425042@qq.com
#include "EventLoop.h"
#include "Server.h"
#include "base/Logging.h"
#include <getopt.h>
#include <string>

int main(int argc, char *argv[])
{
    int threadNum = 4;
    int port = 8888;
    std::string logPath = "./WebServer.log";
    Logger::setLogFileName(logPath);
    EventLoop mainLoop;
    Server myHTTPServer(&mainLoop, threadNum, port);
    myHTTPServer.start();
    mainLoop.loop();
    return 0;
}
