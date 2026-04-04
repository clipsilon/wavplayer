#ifndef LA_WAV_H
#define LA_WAV_H

#include <stdint.h>

typedef struct la_wav {
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
} la_wav;

la_wav *la_wav_read(const char *path);
void la_wav_free(la_wav *wav);

#endif