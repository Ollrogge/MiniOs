#ifndef RINGBUF_H
#define RINGBUF_H

#include <no.h>

// shamelessly (partly) copied from https://github.com/RIOT-OS/RIOT/blob/master/core/include/ringbuffer.h

typedef struct {
    uint8_t *buf; /**< Buffer to operate on. */
    size_t sz;  /**< Size of buf. */
    size_t start; /**< Current read position in the ring buffer. */
    size_t avail; /**< Number of elements available for reading. */
} ringbuf_t;

void ringbuf_init(ringbuf_t* rb, uint8_t* buffer, size_t bufsize);
bool ringbuf_full(const ringbuf_t* rb);
bool ringbuf_empty(const ringbuf_t* rb);
int ringbuf_get_one(ringbuf_t *rb);
int ringbuf_add_one(ringbuf_t* rb, uint8_t c);


#endif /* RINGBUFFER_H */