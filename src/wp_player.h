#ifndef WP_PLAYER_H
#define WP_PLAYER_H

#include "wp_wav.h"

#include <alsa/asoundlib.h>

#include <stdint.h>

typedef struct wp_stream {
    wp_wav *wav;
    int16_t *buffer;
    size_t buffer_cbsize;
    snd_pcm_t *pcm;
    int64_t total_seconds;
    int64_t total_frames;
    int64_t total_uploads;
    int64_t frames_per_write;
    snd_htimestamp_t timestamp_start;
    snd_htimestamp_t timestamp_end;
} wp_stream;

wp_stream *wp_player_create(wp_wav **wav);
void wp_player_free(wp_stream *stream);
void wp_player_upload(wp_stream *stream, float amplitude);
void wp_player_drain(wp_stream *stream);

#endif