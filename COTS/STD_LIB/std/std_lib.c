#include "error_status.h"
#include "std_types.h"
#include "std_lib.h"


uint8_t strlen(const char* ptr)
{
    uint8_t i = 0;
    while(*ptr != 0)
    {
        i++;
        ptr += 1;
    }

    return i;
}

uint8_t strnlen(const char* ptr, int max)
{
    uint8_t i = 0;
    for (i = 0; i < max; i++)
    {
        if (ptr[i] == 0)
            break;
    }

    return i;
}


char* strcpy(char* dest, const char* src)
{
    char* res = dest;
    while(*src != 0)
    {
        *dest = *src;
        src += 1;
        dest += 1;
    }

    // end with a null terminator
    *dest = 0x00;

    return res;
}

char* strncpy(char* dest, const char* src, int count)
{
    int i = 0;
    for (i = 0; i < count; i++)
    {
        if (src[i] == 0x00)
        {
            break;
        }

        dest[i] = src[i];
    }

    // end with a null terminator
    dest[i] = 0x00;
    return dest;
}

uint32_t ceil(float32_t num) 
{
    /* get the integer part of the number */
    int int_part = (int)num; 
    
    /* if the number is positive and has a decimal part, round up */
    if (num > int_part) 
    {
        int_part += 1;
    }

    return int_part;
}