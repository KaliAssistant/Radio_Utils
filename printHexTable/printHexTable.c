/*
 * File:        printHexTable/printHexTable.c
 * Author:      KaliAssistant <work.kaliassistant.github@gmail.com>
 * URL:         https://github.com/KaliAssistant/Radio_Utils
 * Licence:     GNU/GPLv3.0
 *
 * Description:
 *    A c library file for generating terminal-friendly hex+ASCII tables with
 *    per-byte ANSI color annotations and error level tagging. The output visually
 *    highlights regions with color-coded ranges and severity levels (NML, DBG,
 *    WAN, ERR), making it ideal for debugging binary data, memory dumps, or
 *    protocol payloads directly in the terminal.
 *
 */

#include <ctype.h> // for isprint()
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "printHexTable.h"
#include "ANSI_types.h"
#include <colorUtils/conv.h>

char* genRainbowStr(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    size_t alloc_size = 1;  // Start with space for null terminator
    char* buffer = malloc(alloc_size);
    if (!buffer) return NULL;
    buffer[0] = '\0';

    for (size_t i = 0; i < len; i++) {
        float hue = ((float)i / len) * 360.0f;
        float rf, gf, bf;
        hsv2rgb(hue, 1.0f, 1.0f, &rf, &gf, &bf);
        uint8_t r, g, b;
        f01_2rgb888(rf, gf, bf, &r, &g, &b);
        uint8_t color = rgb2ansi256(r, g, b);

        size_t add_size = snprintf(NULL, 0, "\e[38;5;%dm%c", color, str[i]);
        buffer = realloc(buffer, alloc_size + add_size + 1);
        if (!buffer) return NULL;
        sprintf(buffer + alloc_size - 1, "\e[38;5;%dm%c", color, str[i]);
        alloc_size += add_size;
    }

    // Append reset code
    size_t reset_len = strlen("\e[0m");
    buffer = realloc(buffer, alloc_size + reset_len + 1);
    if (!buffer) return NULL;
    strcat(buffer, "\e[0m");

    return buffer;
}

char* printHexTableTail(bool color, const char* str) {
    if (!str) return NULL;

    size_t orig_len = strlen(str);
    char* rainbow_str = (color && orig_len <= 60) ? genRainbowStr(str) : NULL;
    const char* display_str = (rainbow_str) ? rainbow_str : str;

    char preview[61] = {0};  // buffer for truncated version
    bool truncated = false;

    if (orig_len > 60) {
        strncpy(preview, str, 57);
        strcpy(preview + 57, "...");
        display_str = preview;
        truncated = true;
    }

    size_t str_len = strlen(display_str);
    size_t total_pad = (84 > str_len) ? (84 - str_len) : 0;
    size_t left_pad = total_pad / 2;
    size_t right_pad = total_pad - left_pad;

    // For original string (not truncated) with odd length, add one extra dash only on the right
    if (!truncated && (orig_len % 2 != 0)) right_pad++;

    // Allocate final buffer
    size_t buffer_size = left_pad + right_pad + strlen(display_str) + 8;
    char* buffer = malloc(buffer_size);
    if (!buffer) {
        free(rainbow_str);
        return NULL;
    }

    char* p = buffer;
    p += sprintf(p, "+-");
    for (size_t i = 0; i < left_pad; i++) *p++ = '-';
    p += sprintf(p, " %s ", display_str);
    for (size_t i = 0; i < right_pad; i++) *p++ = '-';
    p += sprintf(p, "+\n");
    *p = '\0';

    free(rainbow_str);
    return buffer;
}



char* printHexTable256(uint8_t* buffer, size_t buffer_len, 
                       const char* title_str, const char* tail_str) {
    if (!buffer) return NULL;
    bool _has_title_str = false; bool _has_tail_str = false;
    if (title_str) _has_title_str = true;
    if (tail_str) _has_tail_str = true;
    size_t alloc_size = 1; // Start with space for null terminator
    char* buffer = malloc(alloc_size);
    if (!buffer) return NULL;
    buffer[0] = '\0';
    buffer = realloc(buffer, alloc_size + 2);
    if (!buffer) return NULL;
    alloc_size += 1; strcat(buffer, "+");
    if (_has_title_str) {
        size_t title_str_len = strlen(title_str);
        if (title_str_len <= 40) {
            if (title_str_len % 2 != 0) {
                buffer = realloc(buffer, alloc_size + 2);
                if (!buffer) return NULL;
                alloc_size += 1; strcat(buffer, "-");
            }
            buffer = realloc(buffer, alloc_size + ((66 - tail_str_len) / 2) + 1);
            if (!buffer) return NULL;
            alloc_size += (66 - tail_str_len) / 2;
            for (int i = 0; i < (66 - tail_str_len) / 2; i++) strcat(buffer, "-");
            size_t add_size = snprintf(NULL, 0, " %s ", title_str);
            buffer = realloc(buffer, alloc_size + add_size + 1);
            if (!buffer) return NULL;
            sprintf(buffer + alloc_size - 1, " %s ", title_str);
            alloc_size += add_size;
            buffer = realloc(buffer, alloc_size + ((66 - tail_str_len) / 2) + 1);
            if (!buffer) return NULL;
            alloc_size += (66 - tail_str_len) / 2;
            for (int i = 0; i < (66 - tail_str_len) / 2; i++) strcat(buffer, "-");
        } else {
            buffer = realloc(buffer, alloc_size + 14);
            if (!buffer) return NULL;
            alloc_size += 13; strcat(buffer, "-------------");
            char _p[41];
            strncpy(_p, title_str, 37);
            strcpy(_p + 37, "...");
            size_t add_size = snprintf(NULL, 0, " %s ", _p);
            buffer = realloc(buffer, alloc_size + add_size + 1);
            if (!buffer) return NULL;
            sprintf(buffer + alloc_size - 1, " %s ", _p);
            alloc_size += add_size
            buffer = realloc(buffer, alloc_size + 14);
            if (!buffer) return NULL;
            alloc_size += 13; strcat(buffer, "-------------");
        }
    } else {
        buffer = realloc(buffer, alloc_size + 69);
        if (!buffer) return NULL;
        alloc_size += 68; strcat(buffer, "--------------------------------------------------------------------");
    }
    buffer = realloc(buffer, alloc_size + 202);
    if (!buffer) return NULL;
    alloc_size += 201; strcat("----- ASCII -------+\n+    00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F | 0123456789ABCDEF |+\n+---------------------------------------------------------------------------------------+\n");
    
}


