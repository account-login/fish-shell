#pragma once

#include <stdint.h>


struct pinyin_t {
    uint16_t code;
    char pinyin[6];
};

extern const pinyin_t *const g_pinyin_begin;
extern const pinyin_t *const g_pinyin_end;
