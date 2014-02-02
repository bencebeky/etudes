/* Given random5() generating uniform discrete random variables on {0, 1, 2, 3, 4},
 * create random7() that would do the same on {0, 1, 2, 3, 4, 5, 6}.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define floor7(x) ((int)floor(7.0*(x)))
#define random5() ((int)floor(5.0*drand48()))

/* Form pairs, map 7 triplets to {0, 1, 2, 3, 4, 5, 6}, 
 * redraw in the remaining 5*5 - 7*3 = 4 cases.
 */
int random7_pair() {
  int a, b, result = -1;
  while (result == -1) {
    a = random5();
    b = random5();
    if (a == 0 && (b == 0 || b == 1 || b == 2))
      result = 0;
    else if (a == 0 || (a == 1 && b == 0))
      result = 1;
    else if (a == 1 && (b == 1 || b == 2 || b == 3))
      result = 2;
    else if (a == 1 || (a == 2 && (b == 0 || b == 1)))
      result = 3;
    else if (a == 2)
      result = 4;
    else if (a == 3 && (b == 0 || b == 1 || b == 2))
      result = 5;
    else if (a == 3 || (a == 4 && b == 0))
      result = 6;
    // else we have a == 4 && (b == 1 || b == 2 || b == 3 || b == 4); draw again.
  }
  return result;
}

/* Use real number 0.random5()random5()random5()... in base five,
 * apply floor7() to it.
 */
int random7_int() {
  double min = 0.0, length = 1.0;
  while (floor7(min) != floor7(min+length)) {
    length /= 5.0;
    min += random5() * length;
  }
  return floor7(min);
}

// Test.
int main() {
  int i, count[7];
  // Initialize random seed with time.
  srand48(time(NULL));
  // Reset counters.
  for(i=0; i<7; i++)
    count[i] = 0;
  // Bin first function.
  for(i=0; i<1.4e5; i++)
    count[random7_pair()]++;
  // Print bin counts.
  printf("random7_pair bin counts:\n");
  for(i=0; i<7; i++)
    printf("Bin %d count %d.\n", i, count[i]);
  // Reset counters.
  for(i=0; i<7; i++)
    count[i] = 0;
  // Bin second function.
  for(i=0; i<1.4e5; i++)
    count[random7_int()]++;
  // Print bin counts.
  printf("random7_int bin counts:\n");
  for(i=0; i<7; i++)
    printf("Bin %d count %d.\n", i, count[i]);
  return 0;
}
