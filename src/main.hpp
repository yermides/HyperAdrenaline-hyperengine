#pragma once
#include <iostream>
#include <entities/e_light.hpp>

#define __HYPER_ENGINE_LOG_SYSTEM

#ifdef __HYPER_ENGINE_LOG_SYSTEM
    #define LOG(x) std::cout << x << std::endl;
#else
    #define LOG(x)
#endif

