#pragma once
#include <iostream>

#define __HYPER_ENGINE_LOG_SYSTEM

#ifdef  __HYPER_ENGINE_LOG_SYSTEM
    #define LOG(x)      std::cout << x << std::endl;
    #define INFOLOG(x)  std::cout << "[INFO] " << x << std::endl;
#else
    #define LOG(x)
    #define INFOLOG(x) 
#endif
