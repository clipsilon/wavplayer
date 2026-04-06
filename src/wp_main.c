#include "wp_constants.h"
#include "wp_logging.h"
#include "wp_player.h"
#include "wp_wav.h"

#include <alsa/asoundlib.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static wp_wav *s_wav = NULL;
static wp_stream *s_stream = NULL;

static void cleanup_exit(int signal);

int main(int argc, const char **argv) {
    if (argc < 3) {
        printf("usage: %s WAVFILE VOLUME\n", WP_NAME);
        return 0;
    }

    signal(SIGINT, &cleanup_exit);

    s_wav = wp_wav_read(argv[1]);
    s_stream = wp_player_create(&s_wav);

    wp_player_upload(s_stream, strtof(argv[2], NULL));
    wp_player_drain(s_stream);

    wp_player_free(s_stream);
    wp_wav_free(s_wav);

    return 0;
}

static void cleanup_exit(int signal) {
    wp_player_free(s_stream);
    wp_wav_free(s_wav);
    printf("\n");
    wp_log_info("Exiting");
    exit(0);
}