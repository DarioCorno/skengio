#pragma once
#ifndef SKENGIO_LOGGER

#define SKENGIO_LOGGER

#include <iostream>
#include <sstream>

namespace SKEngio {

    class Logger
    {
        private:

            /* Here will be the instance stored. */
            static Logger* instance;

            /* Private constructor to prevent instancing. */
            Logger();

        public:
            /* Static access method. */
            static Logger* getInstance();

            static void out(std::ostream& s) {
                //std::string data = dynamic_cast<std::ostringstream&>(s).str();
                //std::cout << data << std::endl;

                std::stringstream ss;
                ss << s.rdbuf();
                std::string myString = ss.str();     

                std::cout << myString << std::endl;           
            }
    };

}

#ifndef SK_LOG
#define SK_LOG(...)    Logger::getInstance()->out( std::cout << __VA_ARGS__)
#endif

#endif