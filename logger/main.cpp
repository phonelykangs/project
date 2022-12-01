#include"logger.h"
#include<memory.h>

int main(){
    server::Logger::ptr logger = std::make_shared<server::Logger>();
    logger->addAppender(std::make_shared<server::StdoutAppender>());
    logger->addAppender(std::make_shared<server::FileAppender>("logfile.txt"));
    LOG_FATAL(logger);
    return 0;
}