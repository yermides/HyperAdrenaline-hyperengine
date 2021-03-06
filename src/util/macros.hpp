#pragma once
#include <iostream>

// #define __HYPER_ENGINE_LOG_SYSTEM


#ifdef  __HYPER_ENGINE_LOG_SYSTEM
    #define LOG(x)      std::cout << x << "\n";
    #define INFOLOG(x)  std::cout << "[-- INFO --] " << x << "\n";
    #define ERRLOG(x)   std::cout << "[-- ERROR --] " << x << "\n";
    #define VAR(x)      "{" << x << "}"
#else
    #define LOG(x)
    #define INFOLOG(x) 
    #define ERRLOG(x)
    #define VAR(x)
#endif
