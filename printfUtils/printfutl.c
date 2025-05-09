/*
 * File:        printfUtils/printfutl.c
 * Author:      KaliAssistant <work.kaliassistant.github@gmail.com>
 * URL:         https://github.com/KaliAssistant/Radio_Utils
 * Licence:     GNU/GPLv3.0
 *
 * Description:
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



static int __v_A_s_P_r_I_n_T_f__(char **strp, const char *fmt, va_list ap) {
    va_list ap_copy;
    va_copy(ap_copy, ap);

    int len = vsnprintf(NULL, 0, fmt, ap_copy);
    va_end(ap_copy);

    if (len < 0) {
        *strp = NULL;
        return -1;
    }

    *strp = (char *)malloc((size_t)len + 1);
    if (!*strp)
        return -1;

    return vsnprintf(*strp, (size_t)len + 1, fmt, ap);
}



static int __s_A_p_P_e_N_d_F__(char **buf, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    // Format the new data
    char *newstr;
    int len;
    #if HAS_VASPRINTF
    len = vasprintf(&newstr, fmt, args);
    #else
    len = __v_A_s_P_r_I_n_T_f__(&newstr, fmt, args);
    #endif
    va_end(args);

    if (len < 0 || !newstr)
        return -1; // Allocation or format error

    // Append it to the old buffer
    if (*buf) {
        size_t old_len = strlen(*buf);
        char *joined = realloc(*buf, old_len + len + 1);
        if (!joined) {
            free(newstr);
            return -1; // realloc failed
        }

        memcpy(joined + old_len, newstr, len + 1); // copy including null terminator
        *buf = joined;
    } else {
        *buf = newstr;
    }

    return len;
}


#if !HAS_VASPRINTF
__attribute__((weak, alias("__v_A_s_P_r_I_n_T_f__"))) int vasprintf(char **strp, const char *fmt, va_list ap);
#endif
__attribute__((weak, alias("__s_A_p_P_e_N_d_F__"))) int sappendf(char **buf, const char *fmt, ...);

