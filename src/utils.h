#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

#define KB(value) ((value)*1024LL)
#define MB(value) (KB(value)*1024LL)
#define GB(value) (MB(value)*1024LL)

#define ARRAY_LEN(a) (sizeof(a) / sizeof(a[0]))

#define zeroStruct(instance) zeroSize(sizeof(instance), &(instance))
#define zeroArray(count, pointer) zeroSize(count*sizeof((pointer)[0]), pointer)

internal void zeroSize(size_t size, void *ptr)
{
    u8 *byte = (u8 *)ptr;
    while(size--)
    {
        *byte++ = 0;
    }
}

void concatStr(size_t src_x_cnt, char *src_x,
                size_t src_y_cnt, char *src_y,
                size_t dest_cnt, char *dest) {
    for (int i = 0; i < src_x_cnt; ++i) {
        *dest++ = *src_x++;
    }

    for (int i = 0; i < src_y_cnt; ++i) {
        *dest++ = *src_y++;
    }

    *dest++ = 0;
}

#endif
