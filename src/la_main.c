#include "la_player.h"
#include "la_wav.h"

#include <alsa/asoundlib.h>
#include <signal.h>

#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define FILEPATH  "res/blues-short-stereo.wav"
#define AMPLITUDE 0.5

static la_wav *s_wav = NULL;
static la_stream *s_stream = NULL;

static void cleanup_exit(int signal);

int main(void) {
    signal(SIGINT, &cleanup_exit);

    s_wav = la_wav_read(FILEPATH);
    s_stream = la_player_create(&s_wav);

    la_player_upload(s_stream, AMPLITUDE);
    la_player_drain(s_stream);

    la_player_destroy(s_stream);
    la_wav_free(s_wav);

    return 0;
}

static void cleanup_exit(int signal) {
    la_player_destroy(s_stream);
    la_wav_free(s_wav);
    printf("\nexiting...\n");
    exit(0);
}