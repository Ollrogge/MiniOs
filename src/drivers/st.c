#include <st.h>
#include <no.h>
#include <aic.h>
#include <sched.h>

static volatile st_t* const st = (st_t *)ST_BASE;

static list_node_t _sleep_queue;

static void _check_sleep_queue(void);

void st_init (void)
{
    // enable period interval and alarm status interrupt
    st->ier = bit_set(st->ier, ST_IER_PITS);
    st->ier = bit_set(st->ier, ST_IER_ALMS);

    /**
     * 24.5.4:
     * At reset, this value is set to 0x8000, corresponding to feeding the real-time
     * counter with a 1 Hz signal when the Slow Clock is 32.768 Hz.
     *
     * => divide 0x8000 by 0x3e8 to have counter increased every ms instead of s
     */
    //st->rtmr = 0x8000 / 0x3e8; // every ms
    st->rtmr = 0x8000 / 0x64; // every 10ms;
    //st->rtmr = 0x8000;

    // enable system interrupts at AIC
    aic_irq_enable(SYS_IRQ);

    // start alarm
    st->rtar = st->crtr + ST_RTT_TICKRATE;
}

void st_set_pit_interval(uint32_t ms)
{
    // clock_freq = 32768Hz

    // todo asser max val of pimr ?

    st->pimr = (32768 * ms) / 1000;
}

// https://stackoverflow.com/questions/7083482/how-to-prevent-gcc-from-optimizing-out-a-busy-wait-loop

int _cmp(list_node_t *a, list_node_t *b)
{
    st_timer_t* t1 = container_of(a, st_timer_t, sq_entry);
    st_timer_t* t2 = container_of(b, st_timer_t, sq_entry);

    if (t1->end < t2->end) {
        return -1;
    }
    else if (t1->end > t2->end) {
        return 1;
    }

    return 0;
}

void st_timer_sleep(st_timer_t* timer, uint32_t ms, timer_cb_t cb, int arg)
{
    //todo: make adjustable to changing tickrate
    timer->end = st->crtr + ms % ST_CRTR_MAX;
    timer->cb = cb;
    timer->arg = arg;
    list_rpush(&_sleep_queue, &timer->sq_entry);
    list_sort(&_sleep_queue, _cmp);
}

void st_handle_irq(void)
{
    // save val because cleared on read
    uint32_t val = st->sr;

    // check for period interval timer interrupt
    if (bit_test(val, ST_SR_PITS)) {
        sched_req_reschedule();
    }
    // check for alarm status interrupt
    if (bit_test(val, ST_SR_ALMS)) {
        // renew alarm interrupt
        _check_sleep_queue();
        st->rtar = st->crtr + ST_RTT_TICKRATE;
    }
}

static void _check_sleep_queue()
{
    if (list_is_empty(&_sleep_queue)) {
        return;
    }
    list_node_t* n = list_lpeek(&_sleep_queue);
    st_timer_t* t = container_of(n, st_timer_t, sq_entry);

    while (st->crtr >= t->end) {
        list_lpop(&_sleep_queue);
        t->cb(t->arg);

        if (list_is_empty(&_sleep_queue)) {
            break;
        }

        n = list_lpeek(&_sleep_queue);
        t = container_of(n, st_timer_t, sq_entry);
    }
}
