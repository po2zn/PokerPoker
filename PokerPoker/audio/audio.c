#pragma comment(lib, "winmm.lib")

#include "audio.h"
#include <stdio.h>

#include <windows.h>
#include <mmsystem.h>

//fucking how to set path??????????????
//#define BGM_PATH "PokerPoker/assets/bgm_main.wav"

// .wav 파일 경로 매핑
static const char* soundFiles[] = {
    "PokerPoker/assets/win.wav",         // SOUND_WIN
    "PokerPoker/assets/lose.wav",        // SOUND_LOSE
    "PokerPoker/assets/check.wav",       // SOUND_CHECK
    "PokerPoker/assets/fold.wav",       // SOUND_FOLD
    "PokerPoker/assets/all_in.wav"     // SOUND_ALL_IN
};

static bool audioEnabled = false;
static HANDLE bgmThread = NULL;
static bool bgmRunning = false;

void InitializeAudio(bool enabled)
{
    audioEnabled = enabled;
}

void SetVolume(int volume)
{
    if (volume < 0) volume = 0;
    if (volume > 100) volume = 100;

    // waveOutSetVolume은 0x0000 ~ 0xFFFF 범위
    DWORD level = (DWORD)(volume * 0xFFFF / 500);

    // 좌채널 | 우채널
    waveOutSetVolume(0, (level << 16) | level);
}

int GetBGMLength()
{
    char buffer[128];

    mciSendStringA("status bgm length", buffer, sizeof(buffer), NULL);

    return atoi(buffer);
}

DWORD WINAPI BGMThread(LPVOID arg)
{
    while (bgmRunning)
    {
        mciSendStringA("close bgm", NULL, 0, NULL);

        mciSendStringA("open \"PokerPoker/assets/bgm_main.wav\" alias bgm", NULL, 0, NULL);

        mciSendStringA("play bgm", NULL, 0, NULL);

        int length = GetBGMLength();
        Sleep(length);
    }

    return 0;
}

void PlayBGM()
{
    if (!audioEnabled) return;

    if (bgmRunning) return;

    bgmRunning = true;

    bgmThread = CreateThread(NULL, 0, BGMThread, NULL, 0, NULL);
}

// 임시. 쓸 수도, 안 쓸 수도 있음.
void StopBGM()
{
    if (!audioEnabled) return;
    PlaySound(NULL, NULL, 0);
}


void PlaySFX(SoundType type)
{
    if (!audioEnabled) return;

    int count = sizeof(soundFiles) / sizeof(soundFiles[0]);

    if (type < 0 || type > count) {
        printf("Out of range : %d/%d\n", type, count);
        return;
    }
    // SND_ASYNC: 비동기 재생 (게임 흐름을 막지 않음)
    // SND_NODEFAULT: 파일 없으면 기본음 대신 무시
    PlaySoundA(soundFiles[type], NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
}