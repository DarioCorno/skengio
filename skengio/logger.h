#pragma once
#ifndef SKENGIO_LOGGER

#define SKENGIO_LOGGER

#include <iostream>
#include <sstream>
#include <list>
#include <string>

using namespace std;

#define LOG_ERROR 1
#define LOG_INFO 0

namespace SKEngio {

    struct LogEntry {
        std::string entry;
        unsigned int type;
    };

    class Logger
    {
        private:

            /* Here will be the instance stored. */
            static Logger* instance;

            /* Private constructor to prevent instancing. */
            Logger();

            // prevent copying object
            Logger(const Logger&) = delete;
            Logger(Logger&&) = delete;
            Logger& operator=(const Logger&) = delete;
            Logger& operator=(Logger&&) = delete;


        public:
            void log(std::string a, unsigned int type);

            /* Static access method. */
            static Logger* getInstance();

            std::list<LogEntry> buffer;

            /*
            void out(std::stringstream s) {
                std::string data = s.str();
                std::cout << data << std::endl;

                //std::stringstream ss;
                //ss << s.rdbuf();
                //std::string myString = ss.str();     
                //std::cout << myString << std::endl;      
                //buffer.push_back(myString);
            }
            */

    };

}

#ifndef SK_LOG

#define SK_LOG(Log) { std::ostringstream _os; \
  _os << Log << std::flush;  \
  SKEngio::Logger::getInstance()->log( _os.str(), LOG_INFO ); \
  }

#endif

#ifndef SK_LOG_ERR

#define SK_LOG_ERR(Log) { std::ostringstream _os; \
  _os << Log << std::flush;  \
  SKEngio::Logger::getInstance()->log( _os.str(), LOG_ERROR ); \
  }
#endif

#endif