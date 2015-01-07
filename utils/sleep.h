
#pragma once

#ifdef LINUX
    #include <unistd.h>
#elif WINDOWS
    #include <windows.h>
#endif

void Sleep(int sleepMs) {
    #ifdef LINUX
        usleep(sleepMs * 1000);   // usleep takes sleep time in us (1 millionth of a second)
    #endif
    #ifdef WINDOWS
        Sleep(sleepMs);
    #endif
    }