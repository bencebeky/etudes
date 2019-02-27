#include <cstdio>

#include "fib.h"

int main() {
  for (int i = 0; i <= 10; ++i) {
    printf("%d, %d\n", i, fib(i));
  }
  return 0;
}
