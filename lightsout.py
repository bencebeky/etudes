#/usr/bin/env python
# coding=utf8
# Lights out solver using matrix algebra above modulo 2 ring.

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

import Tkinter
import numpy

n = 3
cellsize = 100
height = n*cellsize-1
width = n*cellsize-1
inputmatrix = numpy.zeros([n,n])

flatten = lambda i, j: n*i + j

# Matrix inversion over modulo 2 ring.
def solve():
  big = numpy.zeros([n*n, n*n+1], dtype=int)
  # Fill in switch patterns.
  for i in range(n):
    for j in range(n):
      big[flatten(i,j), flatten(i,j)] = 1
      if (i >= 1):
        big[flatten(i-1,j), flatten(i,j)] = 1
      if (i <= n-2):
        big[flatten(i+1,j), flatten(i,j)] = 1
      if (j >= 1):
        big[flatten(i,j-1), flatten(i,j)] = 1
      if (j <= n-2):
        big[flatten(i,j+1), flatten(i,j)] = 1
  # Fill in light state.
  big[:, n*n] = inputmatrix.flatten()
  # Invert by elimination.
  for p in range(n*n):
    # Find row with non-zero pivot.
    search = p
    while ((search < n*n) and (big[search, p] % 2 == 0)):
      search += 1
    if (search == n*n):
      print "Singular switch matrix."
      return
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
  return numpy.mod(big[:,-1].reshape([n,n]),2)

# Toggle cells in input grid.
def gridclick(event):
  i = event.x / cellsize;
  j = event.y / cellsize;
  if (i < 0 or i >= n or j < 0 or j >= n):
    return
  inputmatrix[i,j] = 1 - inputmatrix[i,j]
  fill = ("blue" if inputmatrix[i,j] == 1 else "black")
  inputgrid.itemconfig(inputcells[i,j], fill=fill)

# Solve on the push of the button.
def solveclick(event):
  outputmatrix = solve()
  for i in range(n):
    for j in range(n):
      fill = ("blue" if outputmatrix[i,j] == 1 else "black")
      outputgrid.itemconfig(outputcells[i,j], fill=fill)

# Build GUI.
top = Tkinter.Tk()

inputgrid = Tkinter.Canvas(top, height = height, width = width, bg="black")
inputcells = numpy.empty([n,n], dtype=int)
for i in range(n):
  for j in range(n):
    inputcells[i,j] = inputgrid.create_polygon(
      i*cellsize, j*cellsize,
      (i+1)*cellsize, j*cellsize,
      (i+1)*cellsize, (j+1)*cellsize, 
      i*cellsize, (j+1)*cellsize, 
      fill="black", outline="white")
inputgrid.bind("<Button-1>", gridclick)
inputgrid.pack()

button = Tkinter.Button(text="solve")
button.bind("<Button-1>", solveclick)
button.pack()

outputgrid = Tkinter.Canvas(top, height = height, width = width, bg="black")
outputcells = numpy.empty([n,n], dtype=int)
for i in range(n):
  for j in range(n):
    outputcells[i,j] = outputgrid.create_polygon(
      i*cellsize, j*cellsize,
      (i+1)*cellsize, j*cellsize,
      (i+1)*cellsize, (j+1)*cellsize, 
      i*cellsize, (j+1)*cellsize, 
      fill="black", outline="white")
outputgrid.pack()

# Listen to events.
top.mainloop()
