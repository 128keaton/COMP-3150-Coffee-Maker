#include "sleep.h"

int millisleep(unsigned ms) {
#if defined(WIN32)
    SetLastError(0);
    Sleep(ms);
    return GetLastError() ? -1 : 0;

#elif _POSIX_C_SOURCE >= 199309L
    /* prefer to use nanosleep() */

    const struct timespec ts = {
      ms / 1000, /* seconds */
      (ms % 1000) * 1000 * 1000 /* nano seconds */
    };

    return nanosleep(&ts, NULL);


#else
    return usleep(1000 * ms);

#endif
}