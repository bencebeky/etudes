#/usr/bin/env python
# coding=utf8
# Print linearly dependent lights out combinations.

"""
Copyright 2015 Bence Béky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

import numpy

n = 4

flatten = lambda i, j: n*i + j

# Fill left submatrix with switch patterns.
switch = numpy.zeros([n*n, n*n], dtype=int)
for i in range(n):
  for j in range(n):
    switch[flatten(i,j), flatten(i,j)] = 1
    if (i >= 1):
      switch[flatten(i,j), flatten(i-1,j)] = 1
    if (i <= n-2):
      switch[flatten(i,j), flatten(i+1,j)] = 1
    if (j >= 1):
      switch[flatten(i,j), flatten(i,j-1)] = 1
    if (j <= n-2):
      switch[flatten(i,j), flatten(i,j+1)] = 1
big = numpy.zeros([n*n, 2*n*n], dtype=int)
big[:, :n*n] = switch
# Fill right submatrix with identity.
big[:, n*n:] = numpy.eye(n*n, dtype=int)
# Find linear combination that gives zero vector, or find inverse.
for p in range(n*n):
  # Find row with non-zero pivot.
  search = p
  while ((search < n*n) and (big[search, p] % 2 == 0)):
    search += 1
  if (search == n*n):
    print("Singular switch matrix.")
    break
  if (search != p):
    # Switch rows.
    temp = numpy.copy(big[search,:])
    big[search,:] = big[p,:]
    big[p,:] = temp
  # Zero the pth element in other rows.
  for i in range(n*n):
    if (i == p):
      continue;
    if (big[i,p] % 2 == 1):
      big[i,:] += big[p,:]
if (search == n*n):
  for i in range(p,n*n):
    singular = numpy.mod(big[i, n*n:], 2)
    # This must be a positive number of switches.
    assert(0<numpy.sum(singular))
    # Lights must be out by this combination.
    assert((numpy.mod(numpy.matrix(singular)*numpy.matrix(switch),2) == numpy.matrix(numpy.zeros([1,n*n]))).all())
    print(singular.reshape([n,n]))
