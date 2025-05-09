/*
 * File:        printfUtils/printfutl.h
 * Author:      KaliAssistant <work.kaliassistant.github@gmail.com>
 * URL:         https://github.com/KaliAssistant/Radio_Utils
 * Licence:     GNU/GPLv3.0
 *
 * Description:
 *    header (function define) for printfutl.c
 *
 *    Provides portable string formatting utilities for C programs,
 *    including a custom `sappendf()` function that appends formatted text
 *    to a dynamically growing buffer. This is especially useful on systems
 *    where `vasprintf()` is unavailable (e.g., embedded environments).
 *
 *    Features:
 *      - `sappendf()` appends printf-style formatted strings to an existing buffer.
 *      - Fallback implementation of `vasprintf()` for libc environments that lack it.
 *      - Designed to be compatible with static and weak linking for override flexibility.
 *      - Symbols are designed to be safely used or hidden in embedded/static contexts.
 *
 *    Intended for lightweight logging, diagnostics, or dynamically building
 *    messages in constrained or cross-platform C codebases.
 */

#ifndef PRINTFUTL_H
#define PRINTFUTL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>

#if defined(__has_builtin)
    #if __has_builtin(vasprintf)
        #define HAS_VASPRINTF 1
    #else
        #define HAS_VASPRINTF 0
    #endif
#elif defined(__GLIBC__)
    #define HAS_VASPRINTF 1
#elif defined(__NEWLIB__) || defined(__AVR__) || defined(__arm__) || defined(__riscv)
    #define HAS_VASPRINTF 0  // Most embedded libc (newlib, avr-libc, etc.) do not support vasprintf
#else
    #define HAS_VASPRINTF 0  // Default to not available
#endif


#ifdef __cplusplus
extern "C" {
#endif


int sappendf(char **buf, const char *fmt, ...);

#if HAS_VASPRINTF
int vasprintf(char **strp, const char *fmt, va_list ap);
#endif



#ifdef __cplusplus
}
#endif


#endif // PRINTFUTL_H
