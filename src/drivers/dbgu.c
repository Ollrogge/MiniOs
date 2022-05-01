#include <dbgu.h>
#include <no.h>
#include <ringbuf.h>
#include <aic.h>
#include <task.h>
#include <sched.h>

/**
 * AT91RM9200.pdf section 26
 */
static volatile dbgu_t * const dbgu = (dbgu_t *)DBGU_BASE;

/* todo: remove unneded functions dbgu_read */

static uint8_t _rx_buf[DBGU_RX_BUF_SZ];
static ringbuf_t _rx_ringbuf;

static list_node_t _waitc_queue;
static void _wakeup_waitc();

void dbgu_init(void)
{
    ringbuf_init(&_rx_ringbuf, _rx_buf, sizeof(_rx_buf));
    // enable RXRDY interrupt
    dbgu->ier = bit_set(dbgu->ier, DBGU_IER_RXRDY);

    aic_irq_enable(SYS_IRQ);
}

bool dbgu_empty(void)
{
    return ringbuf_empty(&_rx_ringbuf);
}

void dbgu_waitc(list_node_t* node)
{
    list_rpush(&_waitc_queue, node);
}

void dbgu_handle_irq(void)
{
    if (bit_test(dbgu->sr, DBGU_SR_RXRDY)) {
        ringbuf_add_one(&_rx_ringbuf, dbgu->rhr);
        _wakeup_waitc();
    }
}

int dbgu_getc(void)
{
    return ringbuf_get_one(&_rx_ringbuf);
}

int dbgu_putc(uint8_t c)
{
    while (!bit_test(dbgu->sr, DBGU_SR_TXRDY)) {}
    dbgu->thr = c;

    return OK;
}

static void _wakeup_waitc()
{
    if (list_is_empty(&_waitc_queue)) {
        return;
    }

    list_node_t* n = list_lpop(&_waitc_queue);
    task_t* t = container_of(n, task_t, rq_entry);

    t->regs[0] = dbgu_getc();

    sched_wakeup_task(t);
}
