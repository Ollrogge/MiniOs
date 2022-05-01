#include <userlib.h>
#include <mem_layout.h>

volatile uint32_t* counter = (uint32_t*)(TASK_DATA_PAGE);
volatile char* symbol = (char*)(TASK_DATA_PAGE + sizeof(uint32_t));

static void* thread(void* arg)
{
  uint32_t my_counter = 0;
  int id = (int)arg;

  while (*counter < 0x20) {
    printf2("%c%d: %u (%u)\n", *symbol, id, *counter, my_counter);

    *counter = *counter + 1;
    my_counter++;

    sleep(500);
  }

  return NULL;
}

static void* process(void *arg)
{
  uint32_t my_counter = 0;
  int id = 0;
  *counter = 0;
  *symbol = (char)arg;

  child_create(thread, id + 1);
  child_create(thread, id + 2);

  while (*counter < 0x20) {
    printf2("%c%d: %u (%u)\n", *symbol, id, *counter, my_counter);

    *counter = *counter + 1;
    my_counter++;

    sleep(500);
  }

  return NULL;
}

void* main(void *arg)
{
  for (int i = 0; i < 8; i++) {
    char c = readc();

    fork(process, c);
  }

  return NULL;
}
