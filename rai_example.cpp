#include <atomic>
#include <stdio.h>
#include "rai.h"

std::atomic_flag lock = ATOMIC_FLAG_INIT;

void bad_func()
{
  throw std::exception();
}

int main()
{
  INIT_SYS([] { printf("Hello world\n"); },
           [] { printf("Bye cruel world\n"); });

  try
  {
    lock.test_and_set(std::memory_order_acquire);
    ON_SCOPE_EXIT([] { lock.clear(memory_order_release); });
    bad_func();
  }
  catch (...)
  {
  }

  while (lock.test_and_set(std::memory_order_acquire)) ;

  return 0;
}

