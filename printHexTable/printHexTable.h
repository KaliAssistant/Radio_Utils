/*
 * File:        printHexTable/printHexTable.h
 * Author:      KaliAssistant <work.kaliassistant.github@gmail.com>
 * URL:         https://github.com/KaliAssistant/Radio_Utils
 * Licence:     GNU/GPLv3.0
 *
 * Description:
 *    A header file for generating terminal-friendly hex+ASCII tables with
 *    per-byte ANSI color annotations and error level tagging. The output visually
 *    highlights regions with color-coded ranges and severity levels (NML, DBG,
 *    WAN, ERR), making it ideal for debugging binary data, memory dumps, or
 *    protocol payloads directly in the terminal.
 *
 */

#ifndef PRINTHEXTABLE_H
#define PRINTHEXTABLE_H

#define PRINTHEXTABLE_VERSION "v1.0"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <printHexTable/ANSI_types.h> // ANSI types defines, enums and structures... etc.

#ifdef __cplusplus
extern "C" {
#endif

char* genRainbowStr(const char* str);
char* printHexTableTail(bool color, const char* str);
char* printHexTable256(uint8_t *buffer, size_t buffer_len, const char *title_str, const char *tail_str);
char* printColorHexTable256(uint8_t* buffer, size_t buffer_len, ANSIColorMap256_t *ansiMap,
                            ANSIErrTagMap256_t *errMap, const char* title_str, const char* tail_str);


#ifdef __cplusplus
}
#endif

#endif // PRINTHEXTABLE_H
