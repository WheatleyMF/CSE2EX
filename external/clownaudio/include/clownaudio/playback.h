/*
 *  (C) 2018-2020 Clownacy
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 */

#pragma once

#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <stddef.h>

#if !defined(CLOWNAUDIO_EXPORT) && !defined(CLOWNAUDIO_NO_EXPORT)
#include "clownaudio_export.h"
#endif

#define CLOWNAUDIO_STREAM_CHANNEL_COUNT 2

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ClownAudio_Stream ClownAudio_Stream;

CLOWNAUDIO_EXPORT bool ClownAudio_InitPlayback(void);
CLOWNAUDIO_EXPORT void ClownAudio_DeinitPlayback(void);
CLOWNAUDIO_EXPORT ClownAudio_Stream* ClownAudio_CreateStream(unsigned long *sample_rate, void (*user_callback)(void*, short*, size_t));
CLOWNAUDIO_EXPORT bool ClownAudio_DestroyStream(ClownAudio_Stream *stream);
CLOWNAUDIO_EXPORT void ClownAudio_SetStreamCallbackData(ClownAudio_Stream *stream, void *user_data);
CLOWNAUDIO_EXPORT bool ClownAudio_PauseStream(ClownAudio_Stream *stream);
CLOWNAUDIO_EXPORT bool ClownAudio_ResumeStream(ClownAudio_Stream *stream);
CLOWNAUDIO_EXPORT void ClownAudio_LockStream(ClownAudio_Stream *stream);
CLOWNAUDIO_EXPORT void ClownAudio_UnlockStream(ClownAudio_Stream *stream);

#ifdef __cplusplus
}
#endif
