#ifndef MIMIR_MAIN_H
#define MIMIR_MAIN_H

#if defined(__GNUC__) || defined(__clang__)
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define LIKELY(x) (x)
#define UNLIKELY(x) (x)
#endif

#define MIMIR_APPLICATION_NAME "mimir"
#define MIMIR_APPLICATION_VERSION "0.7.0"

#endif // MIMIR_MAIN_H
