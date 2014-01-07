#!/usr/bin/env python
# Calculate the 1000th element of the Fibonacci series. Fast.
# (Another popular tech interview question.)

import numpy;

# Definition of Fibonacci numbers:
# F(1) = 1
# F(2) = 1
# For n = 3, 4, 5, ...: F(n) = F(n-2) + F(n-1).

# Method one: recursion.
# Very inefficient: F(n) is called once, it calls F(n-1) once,
# F(n-2) is called twice (once by F(n) and once by F(n-1)),
# F(n-3) is called thrice (once by F(n-1) and twice by F(n-2)),
# F(n-k) is called F(k+1) times, that is an insane number of calls.
fibonaccirecursion = lambda n: 1 if n <=2 else fibonaccirecursion(n-2) + fibonaccirecursion(n-1);

# Method two: dual recursion. Returns the list [F(n-1),F(n)].
# Calling it with n triggers one call for each of 2, 3, ..., n-1: that is only O(n) calls.
def fibonaccidualrecursion(n):
  if n >= 3:
    a, b = fibonaccidualrecursion(n-1);
    # F(n-2) = a, F(n-1) = b, F(n) = a+b.
    return b, a+b;
  elif n == 2:
    return 1, 1;
  elif n == 1:
    # F(0) = 0.
    return 0, 1;
  else:
    raise NotImplementedError;

# Method three: loop. Nothing fancy.
# Should be much like dual recursion without function call overhead.
def fibonacciloop(n):
  a = 1;
  b = 1;
  for i in xrange(n-2):
    c = a+b;
    a = b;
    b = c;
  return b;

# Method four: even loop. Do two iterations at once to avoid moving around values.
# Slightly faster than simple loop.
def fibonaccievenloop(n):
  a = 1;
  b = 1;
  for i in xrange(int(0.5*(n-2))):
    a = a+b;
    b = a+b;
  if n % 2 == 1:
    if n == 1:
      return 1;
    else:
      return a+b;
  else:
    return b;

# Method five: direct formula.
# This is not faster if we need to calculate all Fibonacci numbers up to F(n),
# but much-much faster if we only need F(n), especially if n is large.
# This is how we solve second order homogeneous linear recursions in general:
# The characteristic polynomial of the recursion is x^2 = 1 + x.
# It has two distinct roots, x_12 = (1 pm sqrt(5)) / 2.
# Therefore a general series is alpha*x_1^n + beta*x_2^n.
# Two initial values, two coefficients, two degrees of freedom.
# (We would use alpha*x^n + beta*n*x^n if x was a multiple root.)
# Turns out |x_2| < 1, so we can omit this term and round.
# Note that if n >= 1475, F(n) cannot be represented as long int any more,
# but the float approximation is still valid.
sqrt5 = numpy.sqrt(5);
fibonaccipower = lambda n: int(numpy.power(0.5*(1.0+sqrt5), n)/sqrt5 + 0.5);

print;
print "Testing.";
print [fibonaccirecursion(n) for n in xrange(1,15)];
print [fibonaccidualrecursion(n)[1] for n in xrange(1,15)];
print [fibonacciloop(n) for n in xrange(1,15)];
print [fibonaccievenloop(n) for n in xrange(1,15)];
print [fibonaccipower(n) for n in xrange(1,15)];

if __name__ == "__main__":
  import timeit;

  number = 20;
  n = 25;
  print;
  print "Timing n={0:d}.".format(n);
  for i in ["fibonaccirecursion", "fibonaccidualrecursion", "fibonacciloop", "fibonaccievenloop", "fibonaccipower"]:
    print "{0:s}: {1:f} us".format(i, 1e6*timeit.timeit("{0:s}({1:d})".format(i,n), setup="from __main__ import {0:s}".format(i), number=number));

  number = 20;
  n = 500;
  print;
  print "Timing n={0:d} (simple recursion would fill up stack).".format(n);
  for i in ["fibonaccidualrecursion", "fibonacciloop", "fibonaccievenloop", "fibonaccipower"]:
    print "{0:s}: {1:f} us".format(i, 1e6*timeit.timeit("{0:s}({1:d})".format(i,n), setup="from __main__ import {0:s}".format(i), number=number));

  number = 20;
  n = 1000;
  print;
  print "Timing n={0:d} (even dual recursion would fill up stack).".format(n);
  for i in ["fibonacciloop", "fibonaccievenloop", "fibonaccipower"]:
    print "{0:s}: {1:f} us".format(i, 1e6*timeit.timeit("{0:s}({1:d})".format(i,n), setup="from __main__ import {0:s}".format(i), number=number));
