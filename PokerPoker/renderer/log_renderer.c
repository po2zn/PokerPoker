#include "stdio.h"
#include "log_renderer.h"

void DrawActionLogs(ScreenBuffer* buffer, GameLog* log) {
    int maxLines = 7; // ACTION 영역 최대 줄 수
    int startY = 22;
    int col = 80;

    DrawText(buffer, col, startY++, "=== ACTION ===");
    startY++;

    int actionCount = 0;
    for (int i = 0; i < log->count; i++) {
        if (log->entries[i].type == LOG_ACTION) actionCount++;
    }

    int skip = (actionCount > maxLines) ? actionCount - maxLines : 0;
    int shown = 0;

    for (int i = 0; i < log->count; i++) {
        if (log->entries[i].type != LOG_ACTION) continue;
        if (shown < skip) { shown++; continue; }
        DrawText(buffer, col, startY++, log->entries[i].message);
        shown++;
    }
}

void DrawSystemLogs(ScreenBuffer* buffer, GameLog* log) {
    int maxLines = 16; // SYSTEM 영역 최대 줄 수
    int startY = 2;
    int col = 80;

    DrawText(buffer, col, startY++, "=== SYSTEM ===");
    startY++;

    // 전체 중 마지막 maxLines개만 표시
    int systemCount = 0;
    for (int i = 0; i < log->count; i++) {
        if (log->entries[i].type == LOG_SYSTEM) systemCount++;
    }

    int skip = (systemCount > maxLines) ? systemCount - maxLines : 0;
    int shown = 0;

    for (int i = 0; i < log->count; i++) {
        if (log->entries[i].type != LOG_SYSTEM) continue;
        if (shown < skip) { shown++; continue; }
        DrawText(buffer, col, startY++, log->entries[i].message);
        shown++;
    }
}