## logging

### Overview

Arduino logging library, supports a variable list of arguments and its format specifiers. 
Logging macros defined in logger.h to reduce code size depending on selected logger level.

#### Implementation supported platform

  * Arduino Uno - ATmega328p
  * Jeenode µ - ATtiny 84

#### Code snippet

    #include <logger.h>
    
    #define LOG_LEVEL LOG_LEVEL_DEBUG

    void setup()
    {
	    LOG_INIT_STATIC_SPEED(LOG_LEVEL);
	    LOG_INFO(F("application started ! \n"));
    }
