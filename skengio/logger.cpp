#include "logger.h"

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

}