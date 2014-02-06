/* CLRS 15-3: Bitonic Euclidean cycle with least total length.
 * Given n >= 3 points on the plane, find the shortest Euclidean cycle
 * that starts with the leftmost points, travels strictly to the right
 * visiting some points on the way until it reaches the rightmost one,
 * than travels back to the leftmost one visiting exactly the ones
 * that are not visited yet, in decreasing order of x coordinates.
 * Input: n, x[], y[]. We can assume that x[] is strictly increasing. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Distance.
double d(int i, int j, double x[], double y[]) {
  double dx = x[i] - x[j];
  double dy = y[i] - y[j];
  return sqrt(dx*dx + dy*dy);
}

void bitonic(int n, double x[], double y[]) {
  // s[i] is minimum total length of bitonic path from i+1 down to 0
  // then back to i. Note that i+1 and i are not connected (except if
  // i == 0), therefore this might not be a subpath of the optimal cycle.
  double s[n-1];
  // In the optimal bitonic path i+1...0...i, i+1 is connected to a[i],
  // and i is connected to b[i]. b[0] is never used.
  int a[n], b[n];
  // Running variables for cycles.
  int i, j, k, last;
  // Temporary store length.
  double sublength;
  // Verify length.
  double length;
  // s[0] is calculated directly.
  s[0] = d(0, 1, x, y);
  // So are the following:
  a[0] = 0;
  a[1] = 0;
  b[1] = 0;
  // s[i] for i=1, 2, ..., n-2 is calculated iteratively.
  for(i=1; i<= n-2; i++) {
    s[i] = HUGE_VAL;
    // Point i+1 is connected to point j on its left. Then
    // j+1, j+2, ..., i are connected in sequence, and
    // j+1 ... 0 ... j is a subproblem.
    for(j=0; j<i; j++) {
      sublength = d(j, i+1, x, y) + s[j];
      b[i] = i-1;
      for(k=j+1; k<i; k++)
        sublength += d(k, k+1, x, y);
      if(sublength < s[i]) {
        s[i] = sublength;
        a[i] = j;
        if (j+1 == i)
          b[i] = a[i-1];
      }
    }
    //printf("s[%d] = %f.\n", i, s[i]);
    //printf("a[%d] = %d.\n", i, a[i]);
    //printf("b[%d] = %d.\n", i, b[i]);
  }
  // Calculate length based on s[].
  length = d(n-2, n-1, x, y) + s[n-2];
  printf("Shortest cycle is %f long.\n", length);
  // Now store cycle based a[], using two branches called top and bottom,
  // filling up cycle[] from two ends.
  // Turns out b[] is not used.
  int top, bottom;
  int topcounter=0, bottomcounter=n-1;
  int cycle[n];
  top = n-2;
  bottom = n-1;
  while((top>0) || (bottom>0)) {
    if(top>bottom) {
      for(;top>bottom;top--) {
        cycle[topcounter] = top;
        topcounter++;
      }
      top = a[bottom];
    } else {
      for(;bottom>top;bottom--) {
        cycle[bottomcounter] = bottom;
        bottomcounter--;
      }
      bottom = a[top];
    }
  }
  // At this point, topcounter == bottomcounter.
  cycle[topcounter] = 0;
  // Calculate length again based on cycle[], also print cycle.
  length = d(cycle[0], cycle[n-1], x, y);
  printf("Shortest cycle is: ");
  for(i=0; i<n-1; i++) {
    printf("%d, ", cycle[i]);
    length += d(cycle[i], cycle[i+1], x, y);
  }
  printf("%d.\n", cycle[n-1]);
  printf("Shortest cycle is %f long.\n", length);
}

int main() {
  int n = 7;
  double x[] = {0.0, 1.0, 1.5, 3.0, 3.5, 5.0, 6.0};
  double y[] = {0.0, 4.0, 4.0, 1.0, 0.5, 2.0, 1.0};
  //int n = 5;
  //double x[] = {0.0, 1.0, 1.5, 3.5, 5.0};
  //double y[] = {0.0, 4.0, 4.0, 0.5, 2.0};
  //int n = 5;
  //double x[] = {0.0, 1.0, 2.0, 3.0, 4.0};
  //double y[] = {0.0, 1.0, 1.0, 0.0, 0.0};
  bitonic(n, x, y);
  return EXIT_SUCCESS;
}
