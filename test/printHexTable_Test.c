#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <printHexTable/printHexTable.h>
#include <printHexTable/ANSI_types.h>

ANSIErrTagMap256_t tag = {0};
ANSIColorMap256_t clr = {0};

int main(void) {
    uint8_t buffer[128] = {0};
    tag.errLevel[199] = 3;;
    clr.ansiColorStr[100] = "\e[1;31m";
    addr2AnsiColorMap256(&clr, 100, 100, "\e[1;33m", 100, '[', 100, ']', 1);
    addr2AnsiColorMap256(&clr, 100, 120, "\e[1;31m", 100, '[', 120, ']', 0);
    addr2AnsiErrTag256(&tag, 10, 30, 1);
    addr2AnsiErrTag256(&tag, 20, 30, 2);
    addr2AnsiErrTag256(&tag, 25, 30, 3);

    char* test = printColorHexTable256(buffer, 128, &clr, &tag, NULL, NULL);
    printf("%s", test);
    free(test); // test printfutl malloc
    return 0;
}
