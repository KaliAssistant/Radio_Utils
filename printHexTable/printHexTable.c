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
#include <stdint.h>
//#include "printHexTable.h"
#include "ANSI_types.h"
#include <colorUtils/conv.h>
#include <printfUtils/printfutl.h>


static char* __g_E_n_R_a_I_n_B_o_W_S_t_R__(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    char* buffer = NULL;

    for (size_t i = 0; i < len; i++) {
        float hue = ((float)i / len) * 360.0f;
        float rf, gf, bf;
        hsv2rgb(hue, 1.0f, 1.0f, &rf, &gf, &bf);
        uint8_t r, g, b;
        f01_2rgb888(rf, gf, bf, &r, &g, &b);
        uint8_t color = rgb2ansi256(r, g, b);

        sappendf(&buffer, "\e[38;5;%dm%c", color, str[i]);
    }

    sappendf(&buffer, "\e[0m");
    return buffer;
}

static char* __p_R_i_N_t_H_e_X_t_A_b_L_e_T_a_I_l__(bool color, const char* str) {
    if (!str) return NULL;

    size_t orig_len = strlen(str);
    char* rainbow_str = (color && orig_len <= 60) ? __g_E_n_R_a_I_n_B_o_W_S_t_R__(str) : NULL;
    const char* display_str = (rainbow_str) ? rainbow_str : str;

    char preview[61] = {0};  // buffer for truncated version
    bool truncated = false;

    if (orig_len > 60) {
        strncpy(preview, str, 57);
        strcpy(preview + 57, "...");
        display_str = preview;
        truncated = true;
        orig_len = 60;
    }

    size_t total_pad = (84 > orig_len) ? (84 - orig_len) : 0;
    size_t left_pad = total_pad / 2;
    size_t right_pad = total_pad - left_pad;

    // NOTE: Don't Need add one, because right_pad size is (total_pad - left_pad)
    // 
    // (x) For original string (not truncated) with odd length, add one extra dash only on the right
    //if (!truncated && (orig_len % 2 != 0)) right_pad++;

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

static char* __p_R_i_N_t_H_e_X_t_A_b_L_e_2_5_6__(uint8_t *buffer, size_t buffer_len, const char *title_str, const char *tail_str) {
    if (!buffer) return NULL;
    char *dst_buf = NULL;
    char *title_buf = NULL;
    char *return_buf = NULL;

    char *tail_alloc_buf = (tail_str) ? __p_R_i_N_t_H_e_X_t_A_b_L_e_T_a_I_l__(0, tail_str) : NULL;

    const char *title_header_str = 
        "----- ASCII -------+\n"
        "+    00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F | 0123456789ABCDEF |+\n"
        "+---------------------------------------------------------------------------------------+\n";

    const char *tail_header_str = 
        "+---------------------------------------------------------------------------------------+\n";

    const char* tail_buf = (tail_alloc_buf) ? tail_alloc_buf : tail_header_str;

    for (int row = 0; row < 16; row++) {
        char ascii[17] = {0};  // Collect 16 ASCII chars
        sappendf(&dst_buf, "+ %X|", row);
        //printf("+ %X|", row);
        for (int col = 0; col < 16; col++) {
            int i = row * 16 + col;
            if (i < buffer_len) {
                sappendf(&dst_buf, " %02X ", buffer[i]);
                //printf(" %02X ", buffer[i]);
                ascii[col] = isprint(buffer[i]) ? buffer[i] : 0;
            } else {
                sappendf(&dst_buf, " XX ");
                //printf(" XX ");
                ascii[col] = 0;
            }
        }
        ascii[16] = '\0'; // null-terminate
        sappendf(&dst_buf, "| ");
        //printf("| ");
        for (int k = 0; k < 16; k++) {
            int idx = row * 16 + k;
            uint8_t c = (idx < buffer_len) ? buffer[idx] : 0xFF;
            if (ascii[k]) {
                //printf("%", ascii[k]);
                //putchar(ascii[k]);
                sappendf(&dst_buf, "%c", ascii[k]);
            } else if (idx >= buffer_len) {
                //printf(" ");
                sappendf(&dst_buf, " ");
            } else {
                if (c == 0x00) sappendf(&dst_buf, " "); //printf(" ");
                else sappendf(&dst_buf, "."); //printf(".");
            }
        }
        sappendf(&dst_buf, " |+\n");
        //printf(" |+\n");
    }
    
    sappendf(&title_buf, "+");
    if (title_str) {
        const char *_pvp_title_str = title_str;
        size_t title_str_len = strlen(title_str);
        char preview[41] = {0};  // buffer for truncated version
        bool truncated = false;
        if (title_str_len > 40) {
            strncpy(preview, title_str, 37);
            strcpy(preview + 37, "...");
            _pvp_title_str = preview;
            truncated = true;
        }
        size_t _pvp_str_len = strlen(_pvp_title_str);
        size_t total_pad = (66 > _pvp_str_len) ? (66 - _pvp_str_len) : 0;
        size_t left_pad = total_pad / 2;
        size_t right_pad = total_pad - left_pad;
        // NOTE: Don't Need add one, because right_pad size is (total_pad - left_pad)
        // 
        // (x) For original string (not truncated) with odd length, add one extra dash only on the right
        //if (!truncated && (title_str_len % 2 != 0)) right_pad++;

        for (size_t i = 0; i < left_pad; i++) sappendf(&title_buf, "-");
        sappendf(&title_buf, " %s ", _pvp_title_str);
        for (size_t i = 0; i < right_pad; i++) sappendf(&title_buf, "-");
    } else {
        for (size_t i = 0; i < 68; i++) sappendf(&title_buf, "-");
    }
    
    sappendf(&title_buf, "%s", title_header_str);
    sappendf(&return_buf, "%s%s%s", title_buf, dst_buf, tail_buf);
    free(title_buf);
    free(dst_buf);
    free(tail_alloc_buf);
    return return_buf;
}

static char* __p_r_i_n_t_C_o_l_o_r_H_e_x_T_a_b_l_e_2_5_6__(uint8_t* buffer, size_t buffer_len, 
                                                           ANSIColorMap256_t *ansiMap, 
                                                           ANSIErrTagMap256_t *errMap, 
                                                           const char* title_str, const char* tail_str) {
    if (!buffer) return NULL;
    char *dst_buf = NULL;
    char *title_buf = NULL;
    char *return_buf = NULL;

    const char *title_header_str = 
        "+---------------------------------------------------------------------------------------+\n";

    const char *tail_header_str = 
        "+---------------------------------------------------------------------------------------+\n";

    char *tail_alloc_buf = (tail_str) ? __p_R_i_N_t_H_e_X_t_A_b_L_e_T_a_I_l__(1, tail_str) : NULL;


    const char* tail_buf = (tail_alloc_buf) ? tail_alloc_buf : tail_header_str;
    
    ANSIColorMap256_t __ansiMap__ = (ansiMap) ? *ansiMap : (ANSIColorMap256_t){0};
    ANSIErrTagMap256_t __errMap__ = (errMap) ? *errMap : (ANSIErrTagMap256_t){0};

    sappendf(&title_buf, "+");
    if (title_str) {
        const char *_pvp_title_str = title_str;
        size_t title_str_len = strlen(title_str);
        char preview[41] = {0};  // buffer for truncated version
        bool truncated = false;
        if (title_str_len > 40) {
            strncpy(preview, title_str, 37);
            strcpy(preview + 37, "...");
            _pvp_title_str = preview;
            truncated = true;
        }
        size_t _pvp_str_len = strlen(_pvp_title_str);
        size_t total_pad = (66 > _pvp_str_len) ? (66 - _pvp_str_len) : 0;
        size_t left_pad = total_pad / 2;
        size_t right_pad = total_pad - left_pad;

        // NOTE: Don't Need add one, because right_pad size is (total_pad - left_pad)
        // 
        // (x) For original string (not truncated) with odd length, add one extra dash only on the right
        //if (!truncated && (title_str_len % 2 != 0)) right_pad++;

        for (size_t i = 0; i < left_pad; i++) sappendf(&title_buf, "-");
        sappendf(&title_buf, " %s ", _pvp_title_str);
        for (size_t i = 0; i < right_pad; i++) sappendf(&title_buf, "-");
    } else {
        for (size_t i = 0; i < 68; i++) sappendf(&title_buf, "-");
    }
    sappendf(&title_buf, "%s", "----- ASCII -------+\n+   ");
    const char* RESET = "\e[0m";

    for (int col = 0; col < 16; col++) {
        uint8_t maxColLevel = 0;
        
        for (int row = 0; row < 16; row++) {
            int idx = row * 16 + col;
            if (idx < 256 && __errMap__.errLevel[idx] > maxColLevel) {
                maxColLevel = __errMap__.errLevel[idx];
            }
        }
        sappendf(&title_buf, " %s%02X%s ", ANSI_LEVEL_COLOR[maxColLevel], col, RESET);
        //printf(" %s%02X%s ", ANSI_LEVEL_COLOR[maxColLevel], col, RESET);
    }
    sappendf(&title_buf, "| ");
    for (int col = 0; col < 16; col++) {
        uint8_t maxColLevel = 0;
        for (int row = 0; row < 16; row++) {
            int idx = row * 16 + col;
            if (idx < 256 && __errMap__.errLevel[idx] > maxColLevel) {
                maxColLevel = __errMap__.errLevel[idx];
            }
        }
        sappendf(&title_buf, "%s%X%s", ANSI_LEVEL_COLOR[maxColLevel], col, RESET);
        //printf(" %s%02X%s ", ANSI_LEVEL_COLOR[maxColLevel], col, RESET);
    }
    sappendf(&title_buf, " |+\n%s", title_header_str);
    
    for (int row = 0; row < 16; row++) {
        int base = row * 16;
        uint8_t maxRowLevel = 0;

        for (int i = 0; i < 16; i++) {
            if (__errMap__.errLevel[base + i] > maxRowLevel) {
                maxRowLevel = __errMap__.errLevel[base + i];
            }
        }
        sappendf(&dst_buf, "%s+ %X|%s", ANSI_LEVEL_COLOR[maxRowLevel], row, RESET);
        //printf("%s+ %X|%s", ANSI_LEVEL_COLOR[maxRowLevel], row, RESET);
        char ascii[17] = {0};  // Collect 16 ASCII chars
        const char* asciiColor[16] = {0};  // Store color for each ASCII cell
        for (int col = 0; col < 16; col++) {
            int i = row * 16 + col;
            // Defaults
            const char* color = (__ansiMap__.ansiColorStr[i]) ? __ansiMap__.ansiColorStr[i] : NULL;
            char left = isprint(__ansiMap__.charBegin[i]) ? __ansiMap__.charBegin[i] : ' ';
            char right = isprint(__ansiMap__.charEnd[i]) ? __ansiMap__.charEnd[i] : ' ';
            if (__errMap__.errLevel[i] > 0) color = ANSI_LEVEL_COLOR_BG[__errMap__.errLevel[i]];
            // Print Hex Byte
            if (i < buffer_len) {
                if (color) {
                    sappendf(&dst_buf, "%s%c%02X%c%s", color, left, buffer[i], right, RESET);
                } else {
                    sappendf(&dst_buf, "%c%02X%c", left, buffer[i], right);
                }
                //printf("%s%c%02X%c%s", color, left, buffer[i], right, RESET);
                ascii[col] = isprint(buffer[i]) ? buffer[i] : 0; 
                asciiColor[col] = color;
            } else {
                sappendf(&dst_buf, "%s%cXX%c%s", "\e[1;31m", left, right, RESET);
                //printf("%s%cXX%c%s", "\e[1;31m", left, right, RESET);
                ascii[col] = 0;
            }
        }

        ascii[16] = '\0'; // null-terminate
        sappendf(&dst_buf, "%s| %s", ANSI_LEVEL_COLOR[maxRowLevel], RESET);
        //printf("%s| %s", ANSI_LEVEL_COLOR[maxRowLevel], RESET);
        
        for (int k = 0; k < 16; k++) {
            int idx = row * 16 + k;
            uint8_t c = (idx < buffer_len) ? buffer[idx] : 0xFF;
            const char* color = asciiColor[k];
            if (ascii[k]) {
                if (color) {
                    sappendf(&dst_buf, "%s%c%s", color, ascii[k], RESET);
                    //printf("%s%c%s", color, ascii[k], RESET);
                } else {
                    sappendf(&dst_buf, "%c", ascii[k]);
                    //putchar(ascii[k]);
                }
            } else if (idx >= buffer_len) {
                const char* dotColor = "\e[1;31m";
                sappendf(&dst_buf, "%s.%s", dotColor, RESET);
                //printf("%s.%s", dotColor, RESET);
            } else {
                const char* dotColor = "\e[1;37m";
                if (c == 0x00)       dotColor = "\e[1;30m";
                else if (c == '\n')  dotColor = "\e[1;34m";
                else if (c == '\r')  dotColor = "\e[1;35m";
                else if (c == '\t')  dotColor = "\e[1;33m";
                else if (c == 0x1B)  dotColor = "\e[1;36m";
                if (color && __errMap__.errLevel[idx] > 0) dotColor = color;
                sappendf(&dst_buf, "%s.%s", dotColor, RESET);
                //printf("%s.%s", dotColor, RESET);
            }
        }
        sappendf(&dst_buf, "%s |+%s\n", ANSI_LEVEL_COLOR[maxRowLevel], RESET);
        //printf("%s |+%s\n", ANSI_LEVEL_COLOR[maxRowLevel], RESET);
    }
    sappendf(&return_buf, "%s%s%s", title_buf, dst_buf, tail_buf);

    free(title_buf);
    free(dst_buf);
    free(tail_alloc_buf);
    return return_buf;
}


__attribute__((weak, alias("__g_E_n_R_a_I_n_B_o_W_S_t_R__"))) char* genRainbowStr(const char* str);
__attribute__((weak, alias("__p_R_i_N_t_H_e_X_t_A_b_L_e_T_a_I_l__"))) char* printHexTableTail(bool color, const char* str);
__attribute__((weak, alias("__p_R_i_N_t_H_e_X_t_A_b_L_e_2_5_6__"))) 
char* printHexTable256(uint8_t *buffer, size_t buffer_len, const char *title_str, const char *tail_str);

__attribute__((weak, alias("__p_r_i_n_t_C_o_l_o_r_H_e_x_T_a_b_l_e_2_5_6__"))) 
char* printColorHexTable256(uint8_t* buffer, size_t buffer_len, ANSIColorMap256_t *ansiMap,
                            ANSIErrTagMap256_t *errMap, const char* title_str, const char* tail_str);
