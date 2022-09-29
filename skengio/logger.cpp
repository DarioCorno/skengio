#include "logger.h"

#include <iostream>
#include <sstream>
#include <list>
#include <string>

namespace SKEngio {

    /* Null, because instance will be initialized on demand. */
    Logger* Logger::instance = 0;

    Logger* Logger::getInstance()
    {
        if (instance == 0)
        {
            instance = new Logger();
        }

        return instance;
    }

    Logger::Logger()
    {}

    void Logger::log(std::string a) {
        std::cout << a << std::endl;
        buffer.push_back(a);
    }            


}