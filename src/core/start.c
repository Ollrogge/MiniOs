#include <dbgu.h>
#include <no.h>
#include <cpu.h>
#include <mem.h>
#include <exc.h>
#include <aic.h>
#include <st.h>
#include <sched.h>
#include <task.h>
#include <assert.h>
#include <syscall.h>
#include <mmu.h>
#include <userlib.h>
#include <mem_layout.h>

__attribute__ ((weakref("main"))) static void* user_main(void *arg);

void start_kernel(void) {
  printf("\n\n");
  printf("*** MyOs main func *** \n");

  cpu_init_exception_stacks();

  mmu_init();

  dbgu_init();
  st_init();

  sched_init();
  cpu_irq_enable();

  puts("Init done");

  if (!user_main) {
    puts("No initial user thread");
    return;
  }

  process_create(user_main, NULL, PRIO_NORMAL, PSR_USR);
}

