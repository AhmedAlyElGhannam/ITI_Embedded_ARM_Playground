#ifndef STD_LIB_H
#define STD_LIB_H

#include "std_types.h"

uint8_t strlen(const char* ptr);
uint8_t strnlen(const char* ptr, int max);

char* strncpy(char* dest, const char* src, int count);
char* strcpy(char* dest, const char* src);

uint32_t ceil(float32_t num);

#endif