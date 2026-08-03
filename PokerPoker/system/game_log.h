#ifndef GAME_LOG_H
#define GAME_LOG_H
 
#include "../enum/log_type.h"
 
#define LOG_MAX 512
#define LOG_LENGTH 512
 
typedef struct LogEntry {
    LogType type;
    char message[LOG_LENGTH];
} LogEntry;
 
typedef struct GameLog {
    LogEntry entries[LOG_MAX];
    int count;
} GameLog;
 
void InitializeLog(GameLog* log);
void AddLog(GameLog* log, LogType type, const char* format, ...);
 
#endif