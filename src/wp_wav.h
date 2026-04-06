#ifndef WP_WAV_H
#define WP_WAV_H

#include <stdint.h>

typedef struct wp_wav {
    struct {
        uint32_t cbsize;
        uint16_t format_tag;
        uint16_t channels;
        uint32_t samples_per_second;
        uint32_t avg_bytes_per_sec;
        uint16_t block_align;
        uint16_t bits_per_sample;
    } format;

    struct {
        uint32_t cbsize;
        uint8_t *raw;
    } data;
} wp_wav;

wp_wav *wp_wav_read(const char *path);
void wp_wav_free(wp_wav *wav);

#endif