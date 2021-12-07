
#if defined(WIN32)
#include <windows.h>
#elif defined(__unix__)
#include <time.h>
#include <unistd.h>
#else
#endif


#ifndef COFFEEMAKER_SLEEP_H
#define COFFEEMAKER_SLEEP_H

int millisleep(unsigned ms);

#endif //COFFEEMAKER_SLEEP_H