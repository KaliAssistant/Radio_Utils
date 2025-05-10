/*
 * File:        printHexTables/ANSI_types.h
 * Author:      KaliAssistant <work.kaliassistant.github@gmail.com>
 * URL:         https://github.com/KaliAssistant/Radio_Utils
 * Licence:     GNU/GPLv3.0
 *
 * Description:
 *    This header defines enums and data structures for annotating byte ranges
 *    with ANSI color escape sequences. It supports error tagging levels (NML, DBG,
 *    WAN, ERR) and is designed to be used with printHexTables to provide visually
 *    informative, color-coded hex+ASCII dumps for terminal-based inspection.
 *
 */

#ifndef ANSI_TYPES_H
#define ANSI_TYPES_H

typedef enum{
    ANSI_ErrLevel_NML = 0b00,
    ANSI_ErrLevel_DBG = 0b01,
    ANSI_ErrLevel_WAN = 0b10,
    ANSI_ErrLevel_ERR = 0b11
} ANSI_ErrLevel_t;

typedef struct{
    char charBegin[256];
    char charEnd[256];
    char* ansiColorStr[256];
} ANSIColorMap256_t;

typedef struct{
    ANSI_ErrLevel_t errLevel[256];
} ANSIErrTagMap256_t;

static const char* ANSI_LEVEL_COLOR[4] = {
    "\e[0m",        // NML
    "\e[38;5;33m",  // DBG - fg Blue, bg NL
    "\e[38;5;226m", // WAN - fg Yellow, bg NL
    "\e[38;5;196m"  // ERR - fg Red, bg NL
};

static const char* ANSI_LEVEL_COLOR_BG[4] = {
    "\e[0m",                      // NML
    "\e[48;5;33m",                // DBG - bg Blue, fg NL
    "\e[48;5;226m\e[38;5;16m",    // WAN - bg Yellow, fg BLACK
    "\e[48;5;196m"                // ERR - bg Red, fg NL
};

#endif // ANSI_TYPES_H
