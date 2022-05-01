#ifndef DBGU_H
#define DBGU_H

#include <no.h>
#include <list.h>

#define DBGU_RX_BUF_SZ 0x40

#define DBGU_BASE (0xFFFFF200)

#define DBGU_CR_RXEN    (0x4)
#define DBGU_CR_TXEN    (0x6)
#define DBGU_CR_RSTSTA  (0x8)

#define DBGU_SR_RXRDY   (0x0)
#define DBGU_SR_TXRDY   (0x1)
#define DBGU_SR_OVRE    (0x5)
#define DBGU_SR_TXEMPTY (0x9)

#define DBGU_IER_RXRDY (0x0)

// section 26.5
typedef struct
{
    uint32_t cr;
    uint32_t mode;
    uint32_t ier;
    uint32_t idr;
    uint32_t imr;
    uint32_t sr;
    uint32_t rhr;
    uint32_t thr;
    uint32_t brgr;
} dbgu_t;

void dbgu_init(void);
void dbgu_waitc(list_node_t* node);
bool dbgu_empty(void);
int dbgu_getc(void);
int dbgu_putc(uint8_t c);
void dbgu_handle_irq(void);

#endif
