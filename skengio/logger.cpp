#include "logger.h"

#include <iostream>
#include <sstream>
#include <list>
#include <string>

namespace SKEngio {
    Logger& Logger::getInstance()
    {
        static Logger instance{};
        return instance;
    }

    void Logger::log(std::string a, unsigned int type) {
        std::cout << a << std::endl;
        LogEntry entry{
            std::move(a),
            type
        };
        buffer.push_back( std::move(entry) );
    }            


}