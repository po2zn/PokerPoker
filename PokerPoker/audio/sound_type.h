#pragma once
// 사실 본래라면 여긴 enum 관련된 작업이라 ../enum/audio_type.h로 가야하지만, enum이 하나밖에 없어서 그냥 여기에 둠

#pragma once
#ifndef SOUND_TYPE_H
#define SOUND_TYPE_H

typedef enum SoundType
{
    SOUND_WIN = 0,
    SOUND_LOSE,
    SOUND_CHECK,
    SOUND_FOLD,
    SOUND_ALL_IN,
} SoundType;

#endif