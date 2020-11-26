#ifndef COMMON_H
#define COMMON_H

#include <stdarg.h>

void Info(const char *fmt, ...);
void Error(const char *fmt, ...);

#define BIT(n)         (1UL << (n))
#define UNUSED(sym)    ((void)(sym))

#define CHUNK_SIZE     1024

#define E_OPENFILE     "Failed to open file"
#define E_READFILE     "Failed to read file"
#define E_MAKETMP      "Failed to create temporary file"
#define E_GETTIME      "Unable to get current system time"
#define E_COMMIT       "Unable to commit, check git settings"
#define E_NOINPUT      "No input file specified"
#define E_NOMEM        "Out of memory"

#endif /* COMMON_H */
