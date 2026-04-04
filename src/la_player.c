#include "la_player.h"
#include "la_logging.h"
#include "la_wav.h"

#include <alsa/asoundlib.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PER_CHANNEL_FRAMES 1024

#define term_showcursor() printf("\033[?25h")
#define term_hidecursor() printf("\033[?25l")

static snd_htimestamp_t timestamp(snd_pcm_t *handle);

la_stream *la_player_create(la_wav **wav) {
    assert(wav != NULL);

    la_stream *stream = calloc(1, sizeof(la_stream));
    stream->buffer_cbsize = sizeof(int16_t) * PER_CHANNEL_FRAMES;
    stream->buffer = calloc(1, stream->buffer_cbsize);
    stream->wav = *wav;

    snd_pcm_hw_params_t *params;
    int dir = 0;
    int mode = 0;

    snd_pcm_open(&stream->pcm, "default", SND_PCM_STREAM_PLAYBACK, mode);
    snd_pcm_hw_params_malloc(&params);
    snd_pcm_hw_params_any(stream->pcm, params);
    snd_pcm_hw_params_set_access(stream->pcm, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(stream->pcm, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(stream->pcm, params, stream->wav->format.channels);
    snd_pcm_hw_params_set_rate_near(stream->pcm, params, &stream->wav->format.samples_per_second, &dir);
    snd_pcm_hw_params(stream->pcm, params);
    snd_pcm_hw_params_free(params);
    snd_pcm_prepare(stream->pcm);

    stream->frames_per_write = PER_CHANNEL_FRAMES / stream->wav->format.channels;
    stream->total_uploads = stream->wav->data.cbsize / stream->buffer_cbsize;
    stream->total_frames = stream->total_uploads * stream->frames_per_write;
    stream->total_seconds = stream->total_frames / stream->wav->format.samples_per_second;

    log_info("Created PCM stream: (%s) %dhz %d-bit %lds",
             (*wav)->format.channels == 2 ? "stereo" : "mono",
             (*wav)->format.samples_per_second,
             (*wav)->format.bits_per_sample,
             stream->total_seconds);

    return stream;
}

void la_player_destroy(la_stream *stream) {
    if (stream != NULL) {
        snd_pcm_close(stream->pcm);
        free(stream->buffer);
        free(stream);
    }
}

void la_player_upload(la_stream *stream, float amplitude) {
    assert(stream != NULL);
    assert(amplitude > -1);

    //----------------------------------------------------------
    // Validate PCM information.

    if (stream->wav->format.channels != 1 && stream->wav->format.channels != 2) {
        log_error("Invalid number of channels: %d", stream->wav->format.channels);
        return;
    }

    //----------------------------------------------------------
    // Begin sending frames to sound card.

    stream->timestamp_start = timestamp(stream->pcm);

    long frames_written_total = 0;
    for (int i = 0; i < stream->total_uploads; i++) {
        void *stream_position = stream->wav->data.raw + (i * stream->buffer_cbsize);
        memcpy(stream->buffer, stream_position, stream->buffer_cbsize);

        //----------------------------------------------------------
        // Modify the volume of the samples sent to the sound card.

        /* Single Channel (mono) */
        if (stream->wav->format.channels == 1) {
            for (int j = 0; j < stream->frames_per_write; j++) {
                float mono = stream->buffer[j];
                stream->buffer[j] = mono * amplitude;
            }
        }

        /* Dual Channel (stereo) */
        if (stream->wav->format.channels == 2) {
            for (int j = 0; j < stream->frames_per_write; j++) {
                float stereo_left = stream->buffer[j * 2 + 0];
                float stereo_right = stream->buffer[j * 2 + 1];
                stream->buffer[j * 2 + 0] = stereo_left * amplitude;
                stream->buffer[j * 2 + 1] = stereo_right * amplitude;
            }
        }

        //----------------------------------------------------------
        // Upload the current samples to the sound card.

        long frames_written = snd_pcm_writei(stream->pcm, stream->buffer, stream->frames_per_write);
        if (frames_written < 0) {
            snd_pcm_recover(stream->pcm, (int)frames_written, 0);
        }

        /* Print the frame writing status */
        frames_written_total += frames_written;
        term_hidecursor();
        log_info_overwrite("Frames sent to soundcard: (%ld/%ld)",
                           frames_written_total, stream->total_frames);
    }
    term_showcursor();
    printf("\n");
}

void la_player_drain(la_stream *stream) {
    long elapsed_seconds = 0;
    for (;;) {
        if (elapsed_seconds > stream->total_seconds) {
            break;
        }
        stream->timestamp_end = timestamp(stream->pcm);
        term_hidecursor();
        log_info_overwrite("Seconds elapsed: (%ld/%ld)", elapsed_seconds, stream->total_seconds);
        elapsed_seconds = stream->timestamp_end.tv_sec - stream->timestamp_start.tv_sec;
    }
    term_showcursor();
    printf("\n");
}

//----------------------------------------------------------
//
// INTERNAL
//
//----------------------------------------------------------

static snd_htimestamp_t timestamp(snd_pcm_t *handle) {
    snd_pcm_uframes_t avail;
    snd_htimestamp_t timestamp;
    snd_pcm_htimestamp(handle, &avail, &timestamp);
    return timestamp;
}