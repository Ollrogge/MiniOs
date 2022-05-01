#include <ringbuf.h>

static uint8_t get_head(ringbuf_t* rb);
static void add_tail(ringbuf_t* rb, uint8_t c);

void ringbuf_init(ringbuf_t* rb, uint8_t* buf, size_t sz)
{
    rb->buf = buf;
    rb->sz = sz;
    rb->start = 0;
    rb->avail = 0;
}

static uint8_t get_head(ringbuf_t* rb)
{
    uint8_t result = rb->buf[rb->start];

    if ((--rb->avail == 0) || (++rb->start == rb->sz)) {
        rb->start = 0;
    }
    return result;
}

static void add_tail(ringbuf_t* rb, uint8_t c)
{
    unsigned pos = rb->start + rb->avail++;

    if (pos >= rb->sz) {
        pos -= rb->sz;
    }

    rb->buf[pos] = c;
}

bool ringbuf_full(const ringbuf_t* rb)
{
    return rb->avail == rb->sz;
}

bool ringbuf_empty(const ringbuf_t* rb)
{
    return rb->avail == 0;
}

int ringbuf_add_one(ringbuf_t* rb, uint8_t c)
{
    int result = -1;
    if (ringbuf_full(rb)) {
        result = get_head(rb);
    }
    add_tail(rb, c);

    return result;
}

int ringbuf_get_one(ringbuf_t* rb)
{
    if (!ringbuf_empty(rb)) {
        return get_head(rb);
    }
    else {
        return -1;
    }
}