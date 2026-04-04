#ifndef LA_PLAYER_H
#define LA_PLAYER_H

#include "la_wav.h"

#include <alsa/asoundlib.h>

#include <stdint.h>

typedef struct la_stream {
    la_wav *wav;
    int16_t *buffer;
    size_t buffer_cbsize;
    snd_pcm_t *pcm;
    int64_t total_seconds;
    int64_t total_frames;
    int64_t total_uploads;
    int64_t frames_per_write;
    snd_htimestamp_t timestamp_start;
    snd_htimestamp_t timestamp_end;
} la_stream;

la_stream *la_player_create(la_wav **wav);
void la_player_destroy(la_stream *stream);
void la_player_upload(la_stream *stream, float amplitude);
void la_player_drain(la_stream *stream);

#endif