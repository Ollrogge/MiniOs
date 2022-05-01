#include <stdarg.h>
#include <no.h>
#include <userlib.h>
#include <dbgu.h>

// todo: pass read / write funcs as fp to allow multiple output variants ?

void printf(char* fmt, ...)
{
    char c;
    uint32_t u;
    int32_t s;
    va_list args;
    va_start(args, fmt);

    while ((c = *fmt) != '\0') {
        if (c != '%') {
            putc(c);
            fmt++;
            continue;
        }

        fmt++;
        c = *fmt;

        switch(c) {
            case 'c':
                c = va_arg(args, int);
                putc(c);
                break;
            case 's':
            {
                char* p;
                p = va_arg(args, char *);
                while (*p != '\0') {
                    putc(*p++);
                }
            }
            break;
            case 'p':
            printf("%s", "0x");
            u = va_arg(args, uint32_t);
            print_num(u, 16, true);
            break;
            case 'x':
            u = va_arg(args, uint32_t);
            print_num(u, 16, false);
            break;
            case 'd': {
                s = va_arg(args, int32_t);
                if (s < 0) {
                    printf("-");
                    s = -s;
                }
                print_num(s, 10, false);
                break;
            }
            case 'u':
            s = va_arg(args, uint32_t);
            print_num(s, 10, false);
            break;
            default:
            putc(c);
            break;

        }

        fmt++;
    }
}

void print_num(uint32_t val, unsigned base, bool pad)
{
    char buf[sizeof(uint64_t) * 0x8]; // enough for base 2
    char* p_buf = &buf[sizeof(buf) - 1];
    static char digits[] = "0123456789abcdef";
    unsigned log;

    switch (base) {
        case 16:
            log = 4;
            break;
        case 2:
            log = 1;
            break;
        default:
            log = 8;
    }

    unsigned width = 4; // default for now
    size_t len = 0;

    do {
        *p_buf-- = digits[(val % base)];
        val /= base;
        len++;
    } while (val != 0);

    if (pad) {
      while (len++ < width * (sizeof(uint64_t) / log)) {
          *p_buf-- = '0';
      }
    }

    while (++p_buf != &buf[sizeof(buf)]) {
        putc(*p_buf);
    }
}

void putc(int c)
{
    dbgu_putc(c);
}

int getc(void)
{
    return dbgu_getc();
}

void puts(const char *str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
        putc(str[i]);
    }
    putc('\n');
}

size_t strlen(const char *str)
{
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }

    return i;
}

void* memset(void* dest, uint8_t val, size_t n)
{
    uint8_t* ptr = dest;

    while (n-- > 0) {
        *ptr++ = val;
    }

    return dest;
}

void* memcpy(void* dest, const void *src, size_t n)
{
    uint8_t* p_dest = dest;
    const uint8_t* p_src = src;

    while (n--) {
        *p_dest++ = *p_src++;
    }

    return dest;
}


