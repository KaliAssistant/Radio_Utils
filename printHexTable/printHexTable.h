/*
 * File:        printHexTable/printHexTable.h
 * Author:      KaliAssistant <work.kaliassistant.github@gmail.com>
 * URL:         https://github.com/KaliAssistant/Radio_Utils
 * Licence:     GNU/GPLv3.0
 *
 * Description:
 *    A header-only utility for generating terminal-friendly hex+ASCII tables with
 *    per-byte ANSI color annotations and error level tagging. The output visually
 *    highlights regions with color-coded ranges and severity levels (NML, DBG,
 *    WAN, ERR), making it ideal for debugging binary data, memory dumps, or
 *    protocol payloads directly in the terminal.
 *
 */

#ifndef PRINTHEXTABLE_H
#define PRINTHEXTABLE_H

#include <ctype.h> // for isprint()
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./ANSI_types.h" // ANSI types defines, enums and structures... etc.



void printColorHexTable256(uint8_t* buffer, size_t buffer_len, ANSI_Color_Map_t* ansiMap, size_t ansiMap_len, ANSI_ErrTag_Map_t* errMap, size_t errMap_len, const char* title_str) {
  size_t title_str_len = strlen(title_str);
  printf("+");
  if(title_str_len <= 40) {
    if(title_str_len % 2 != 0) printf("-");
    for(int i = 0; i < (66 - title_str_len)/2; i++) {
      printf("-");
    }
    printf(" %s ", title_str);
    for(int i = 0; i < (66 - title_str_len)/2; i++) {
      printf("-");
    }
  } else {
    for(int i = 0; i < 13; i++) {
      printf("-");
    }
    char _P[41];
    strncpy(_P, title_str, 37);
    strcpy(_P + 37, "...");
    printf(" %s ", _P);
    for(int i = 0; i < 13; i++) {
      printf("-");
    }
  }
  printf("----- ASCII -------+\n");
  const char* RESET = "\e[0m";
  uint8_t levelMap[256] = {0}; // 0 = NML
  // Apply tag map overlay
  for (size_t j = 0; j < errMap_len; j++) {
    for (int i = errMap[j].byteAddrBegin; i <= errMap[j].byteAddrEnd; i++) {
      if (i < 256 && errMap[j].errLevel > levelMap[i]) {
        levelMap[i] = errMap[j].errLevel;
      }
    }
  }
  printf("+   ");
  for (int col = 0; col < 16; col++) {
    uint8_t maxColLevel = 0;
    for (int row = 0; row < 16; row++) {
      int idx = row * 16 + col;
      if (idx < 256 && levelMap[idx] > maxColLevel) {
        maxColLevel = levelMap[idx];
      }
    }
    printf(" %s%02X%s ", ANSI_LEVEL_COLOR[maxColLevel], col, RESET);
  }
  printf("| ");
  for (int col = 0; col < 16; col++) {
    uint8_t maxAscolLevel = 0;
    for (int row = 0; row < 16; row++) {
      int idx = row * 16 + col;
      if (idx < 256 && levelMap[idx] > maxAscolLevel) {
        maxAscolLevel = levelMap[idx];
      }
    }
    printf("%s%X%s", ANSI_LEVEL_COLOR[maxAscolLevel], col , RESET);
  }
  printf(" |+\n");
  printf("+---------------------------------------------------------------------------------------+\n");
  
  for (int row = 0; row < 16; row++) {
    int base = row * 16;
    uint8_t maxRowLevel = 0;

    for (int i = 0; i < 16; i++) {
      if (levelMap[base + i] > maxRowLevel) {
        maxRowLevel = levelMap[base + i];
      }
    }
    printf("%s+ %X|%s", ANSI_LEVEL_COLOR[maxRowLevel], row, RESET);
    char ascii[17] = {0};  // Collect 16 ASCII chars
    const char* asciiColor[16] = {0};  // Store color for each ASCII cell
    for (int col = 0; col < 16; col++) {
      int i = row * 16 + col;
      // Defaults
      const char* color = NULL;
      char left = ' ';
      char right = ' ';
      bool matched = false;
      for (size_t j = 0; j < ansiMap_len; j++) {
        if (i >= ansiMap[j].byteAddrBegin && i <= ansiMap[j].byteAddrEnd) {
          matched = true;
          color = ansiMap[j].ansiColorStr;
          if (i == ansiMap[j].byteAddrBegin) left = ansiMap[j].charBegin;
          if (i == ansiMap[j].byteAddrEnd)   right = ansiMap[j].charEnd;
          break;
        }
      }

      if (levelMap[i] > 0) color = ANSI_LEVEL_COLOR_BG[levelMap[i]];           
      // Print Hex Byte
      if (i < buffer_len) {
        if (matched) {
          printf("%s%c%02X%c%s", color, left, buffer[i], right, RESET);
        } else {
          printf(" %02X ", buffer[i]);
        }
        ascii[col] = isprint(buffer[i]) ? buffer[i] : 0; 
        if (matched) {
          asciiColor[col] = color;
        }
      } else {
        printf("%s%cXX%c%s", "\e[1;31m", left, right, RESET);
        ascii[col] = 0;
      }
    }

    ascii[16] = '\0'; // null-terminate
    printf("%s| %s", ANSI_LEVEL_COLOR[maxRowLevel], RESET);
    for (int k = 0; k < 16; k++) {
      int idx = row * 16 + k;
      uint8_t c = (idx < buffer_len) ? buffer[idx] : 0xFF;
      const char* color = asciiColor[k];
      if (ascii[k]) {
        if (color) {
          printf("%s%c%s", color, ascii[k], RESET);
        } else {
          putchar(ascii[k]);
        }
      } else if (idx >= buffer_len) {
        const char* dotColor = "\e[1;31m";
        printf("%s.%s", dotColor, RESET);
      } else {
        const char* dotColor = "\e[1;37m";
        if (c == 0x00)       dotColor = "\e[1;30m";
        else if (c == '\n')  dotColor = "\e[1;34m";
        else if (c == '\r')  dotColor = "\e[1;35m";
        else if (c == '\t')  dotColor = "\e[1;33m";
        else if (c == 0x1B)  dotColor = "\e[1;36m";
        if (color && levelMap[idx] > 0) dotColor = color;
        printf("%s.%s", dotColor, RESET);
      }
    }
    printf("%s |+%s\n", ANSI_LEVEL_COLOR[maxRowLevel], RESET);
  }
  printf("+------------------------------- *printHexTable256 v1.0* -------------------------------+\n");
}


#endif // PRINTHEXTABLE_H
