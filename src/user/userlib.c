#include <userlib.h>
#include <stdarg.h>

int read(void* buf, size_t len)
{
    char *_buf = (char *)buf;
    int ret;
    size_t read = 0;
    while (read != len)  {
        ret = readc();

        if (ret < 0) {
            return ret;
        }

        _buf[read++] = (char)ret;
    }

    return OK;
}

int write(const void *buf, size_t len)
{
    (void) buf;
    (void) len;

    char *_buf = (char *)buf;

    for (size_t i = 0; i < len; i++) {
        writec(_buf[i]);
    }

    return OK;
}

// todo: pass read / write funcs as fp to allow multiple output variants ?

/**
 * Adding kprint_num as a syscall is a dirty hack because the function uses
 * mod and div for which arm-none-eabi-gcc creates c helper wrappers automatically.
 * They will be mapped into kernel section causing a page fault in user space.
 *
 * The functions are:
 *
 * __aeabi_uidivmod
 * __aeabi_uidiv
 * __aeabi_idiv
 *
 * I have not found a way how to force these functions into a specific section
 * like I did with memset in no.c
 *
 * https://developer.arm.com/documentation/dui0773/b/chr1359124235078
 */

void printf2(char* fmt, ...)
{
    char c;
    uint32_t u;
    int32_t s;
    va_list args;
    va_start(args, fmt);

    while ((c = *fmt) != '\0') {
        if (c != '%') {
            writec(c);
            fmt++;
            continue;
        }

        fmt++;
        c = *fmt;

        switch(c) {
            case 'c':
                c = va_arg(args, int);
                writec(c);
                break;
            case 's':
            {
                char* p;
                p = va_arg(args, char *);
                while (*p != '\0') {
                    writec(*p++);
                }
            }
            break;
            case 'p':
            printf2("%s", "0x");
            u = va_arg(args, uint32_t);
            kprint_num(u, 16, true);
            break;
            case 'x':
            u = va_arg(args, uint32_t);
            kprint_num(u, 16, false);
            break;
            case 'd': {
                s = va_arg(args, int32_t);
                if (s < 0) {
                    printf2("-");
                    s = -s;
                }
                kprint_num(s, 10, false);
                break;
            }
            case 'u':
            s = va_arg(args, uint32_t);
            kprint_num(s, 10, false);
            break;
            default:
            writec(c);
            break;

        }

        fmt++;
    }
}

void puts2(const char *str)
{
    size_t len = strlen2(str);
    for (size_t i = 0; i < len; i++) {
        writec(str[i]);
    }
    writec('\n');
}

size_t strlen2(const char *str)
{
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }

    return i;
}


