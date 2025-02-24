#pragma once

#ifdef DEBUG

#include <SDL3/SDL_log.h>
#include <iostream>

#define setup_debug() \
    std::cout << "Debug mode enabled" << std::endl;\
    SDL_LogOutputFunction log_func = [](void *userdata, int category, SDL_LogPriority priority, const char *message) {\
        std::cout << "SDL: " << message << std::endl;\
    };\
    SDL_SetLogOutputFunction(log_func, nullptr);
#else

#define setup_debug()

#endif