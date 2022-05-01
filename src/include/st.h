#ifndef ST_H
#define ST_H

#include <no.h>
#include <list.h>

#define ST_BASE (0xFFFFFD00)

#define ST_IER_PITS (0x0) // Period Interval Timer Status Interrupt Enable
#define ST_IER_ALMS (0x3)

#define ST_SR_PITS (0x0)
#define ST_SR_ALMS (0x3)

/**
 * @brief The number of milliseconds per second
 */
#define MS_PER_SEC          (1000LU)

#define ST_RTT_TICKRATE (0x1) // 1000hz

#define ST_CRTR_MAX (1048576) // max counter value

// section 24.6
typedef struct
{
    uint32_t cr;
    uint32_t pimr;
    uint32_t wdmr;
    uint32_t rtmr;
    uint32_t sr;
    uint32_t ier;
    uint32_t idr;
    uint32_t imr;
    uint32_t rtar;
    uint32_t crtr;
} st_t;

typedef void (*timer_cb_t)(int arg);

typedef struct
{
    uint32_t end;
    timer_cb_t cb;
    int arg;
    list_node_t sq_entry;
} st_timer_t;

void st_init(void);
void st_set_pit_interval(uint32_t ms);
void st_handle_irq(void);
void st_timer_sleep(st_timer_t* timer, uint32_t sec, timer_cb_t cb, int arg);

#endif
