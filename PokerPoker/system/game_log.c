#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "game_log.h"

void InitializeLog(GameLog* log)
{
    if (log == NULL) return;

    log->count = 0;
}

void AddLog(GameLog* log, LogType type, const char* format, ...)
{
    if (log == NULL) return;
    if (format == NULL) return;

    if (log->count >= LOG_MAX)
    {
        for (int i = 1; i < LOG_MAX; i++)
        {
            log->entries[i - 1] = log->entries[i];
        }

        log->count = LOG_MAX - 1;
    }

    LogEntry* entry = &log->entries[log->count];
    entry->type = type;

    va_list args;
    va_start(args, format);
    vsnprintf(entry->message, LOG_LENGTH, format, args);
    va_end(args);

    log->count++;

    if (type == LOG_SYSTEM) {
        printf("%s\n", entry->message);
        fflush(stdout);
    }
}