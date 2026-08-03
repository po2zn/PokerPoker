#pragma once
#pragma once
#ifndef AUDIO_H
#define AUDIO_H

#include "sound_type.h"

#include <stdbool.h>

void InitializeAudio(bool enabled);

void SetVolume(int volume);

void PlayBGM();
void StopBGM();
void PlaySFX(SoundType type);

#endif