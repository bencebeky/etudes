#!/usr/bin/env python
# Find maximum in the cyclic permutation of a sorted array.

import numpy;

def findmax(a):
  """
  Find maximum element in the cyclic permutation of a strictly
  increasind array (every element is unique).
  """
  n = len(a);
  assert (n >= 1);
  # Only one element?
  if (n==1):
    return 0;
  # Sorted.
  if (a[0] < a[-1]):
    return n-1;
  # Otherwise, we do a binary search.
  left = 0;
  right = n-1;
  # From this point on, we have
  # left <= argmax
  # argmax < right
  # a[left] > a[right]
  while (left+1 < right):
    middle = int((left+right)/2);
    if (a[left] > a[middle]):
      right = middle;
    else:
      left = middle;
  return left;

# Test on arrays of different lengths.
for n in range(1,10):
  # Actual values do not matter, let us use 0, 1, ..., n-1.
  a = numpy.arange(n);
  for i in xrange(len(a)):
    b = numpy.append(a[i:], a[:i]);
    assert findmax(b) == numpy.argmax(b);
