#ifndef MEM_H
#define MEM_H

#include <no.h>

typedef enum
{
    MEM_KER,
    MEM_USER
} mapping_t;

void* kmalloc(size_t sz, mapping_t type);

void* alloc_user_stack(size_t sz, uint16_t pid);

#endif /* MEM_H */