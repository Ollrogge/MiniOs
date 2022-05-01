#ifndef NO_H
#define NO_H

#ifdef __GNUC__
#define NORETURN  __attribute__((noreturn))
#else
#define NORETURN
#endif

#ifdef __GNUC__
#define NOINLINE __attribute__ ((noinline))
#else
#define NOINLINE
#endif

#ifdef __GNUC__
#define NOOPTIMIZE __attribute__((optimize("O0")))
#else
#define NOOPTIMIZE
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof((a)) / sizeof((a)[0]))
#endif

#define OK  (0)
#define ERR (-1)

#define INT8_MIN   (-1-0x7f)
#define INT16_MIN  (-1-0x7fff)
#define INT32_MIN  (-1-0x7fffffff)
#define INT64_MIN  (-1-0x7fffffffffffffff)

#define INT8_MAX   (0x7f)
#define INT16_MAX  (0x7fff)
#define INT32_MAX  (0x7fffffff)
#define INT64_MAX  (0x7fffffffffffffff)

#define UINT8_MAX  (0xff)
#define UINT16_MAX (0xffff)
#define UINT32_MAX (0xffffffffu)
#define UINT64_MAX (0xffffffffffffffffu)

typedef signed char int8_t;
typedef short int16_t;
typedef long int32_t;
typedef long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;

typedef unsigned long uintptr_t;

typedef unsigned long size_t;
typedef long ssize_t;

typedef enum
{
    false,
    true
} bool;

#define NULL ((void*)0)

__attribute__((format(printf, 1, 2))) void printf(char *fmt, ...);

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(PTR, TYPE, MEMBER) ((TYPE *) ((char *) (PTR) - offsetof(TYPE, MEMBER)))

void putc(int c);
int getc(void);

void puts(const char *str);

__attribute__((section (".user_text"))) void* memset(void* dest, uint8_t val, size_t n);

void* memcpy(void* dest, const void *src, size_t n);

size_t strlen(const char *str);

void print_num(uint32_t val, unsigned base, bool pad);

static inline uint32_t bit_set(uint32_t val, unsigned pos)
{
    return val |= (1 << pos);
}

static inline uint32_t bit_clear(uint32_t val, unsigned pos)
{
    return val &= ~(1 << pos);
}

static inline bool bit_test(unsigned val, unsigned pos)
{
    return val & (1 << pos);
}

static inline unsigned  bit_lsb(unsigned val)
{
    unsigned r = 0;

    while ((val & 0x01) == 0) {
        val >>= 1;
        r++;
    }

    return r;
}

static inline unsigned bit_msb(unsigned val)
{
    unsigned r = 0;
    while (val >>= 1) { /* unroll for more speed... */
        ++r;
    }
    return r;
}

/*
 * barrier() - Compiler Optimization Barrier
 *
 * Verhindert ohne weiteren Overhead, dass GCC irgendwelche Speicherstellen
 * über die Barriere hinweg in Registern zwischenspeichert.
 */
static inline void barrier(void)
{
  	asm("" ::: "memory");
}

/*
 * busy_wait() - Aktiv nichts tun
 *
 * @usec: Länge in Mikrosekunden
 *
 * Die Umrechnung von Mikrosekunden auf Schleifendurchläufe stimmt nur für
 * unsere Hardware und aktivierte Optimierungen: unsere CPU hat 180 MHz und
 * ein Schleifendurchlauf hat 2 Instruktionen, die 4 Takte benötigen.
 */
static inline void busy_wait(unsigned int usec)
{
 	unsigned int loops = usec * (180 / 4);
 	while(loops--) {
        barrier();
    }
}


#endif /* NO_H */
