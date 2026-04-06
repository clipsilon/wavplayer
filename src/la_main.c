#include "la_logging.h"
#include "la_player.h"
#include "la_wav.h"

#include <alsa/asoundlib.h>
#include <signal.h>

#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static la_wav *s_wav = NULL;
static la_stream *s_stream = NULL;

static void cleanup_exit(int signal);

int main(int argc, const char **argv) {
    if (argc < 3) {
        printf("usage: linux-audio WAVFILE VOLUME\n");
        return 0;
    }

    signal(SIGINT, &cleanup_exit);

    s_wav = la_wav_read(argv[1]);
    s_stream = la_player_create(&s_wav);

    la_player_upload(s_stream, strtof(argv[2], NULL));
    la_player_drain(s_stream);

    la_player_free(s_stream);
    la_wav_free(s_wav);

    return 0;
}

static void cleanup_exit(int signal) {
    la_player_free(s_stream);
    la_wav_free(s_wav);
    printf("\n");
    log_info("Exiting");
    exit(0);
}