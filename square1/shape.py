#/usr/bin/env python3
# coding=utf8
# Generate figures for solving the Square 1 puzzle into a cube shape.

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
from matplotlib import lines
from matplotlib import patches
from matplotlib import pyplot
from matplotlib.backends.backend_pdf import PdfPages
from sys import argv

pyplot.interactive(False)
fig = pyplot.figure(figsize=[6,6], facecolor="white")

def drawline(fig, data):
  fig.lines.extend([patches.Polygon(0.5 + 0.5 * numpy.array(data).T,
    transform=fig.transFigure, figure=fig, facecolor="grey", edgecolor="black", linewidth=4)])

def drawshape(arg):
  angle = 0.0
  short = numpy.sqrt(0.5) / numpy.cos(numpy.pi/12)
  for shape in arg:
    angle1 = angle + numpy.pi / 6;
    angle2 = angle + numpy.pi / 3;
    if (shape == "1"):
      drawline(fig, [[0.0, short * numpy.sin(angle), short * numpy.sin(angle1)],
                     [0.0, short * numpy.cos(angle), short * numpy.cos(angle1)]])
      angle = angle1
      continue
    drawline(fig,
      [[0.0, short * numpy.sin(angle), numpy.sin(angle1), short * numpy.sin(angle2)],
       [0.0, short * numpy.cos(angle), numpy.cos(angle1), short * numpy.cos(angle2)]])
    angle = angle2
  fig.lines.extend([lines.Line2D([0.5, 0.5], [0.0, 1.0],
  transform=fig.transFigure, figure=fig, linewidth=4)])
  pyplot.savefig(arg+".png")

# For each shape, |firststep| contains the first step to take on one of the
# shortest paths to the root.
firststep = {}

with open("solution.dat") as f:
  for line in f:
    words = line.split();
    assert(len(words) == 6)
    firststep[(int(words[0]), int(words[1]))] = [
        (int(words[2]), int(words[3])),
        (int(words[4]), int(words[5]))]

# |paths| is the list of all paths from all shapes to one step before the root.
paths = []

for shape in firststep.keys():
  if (shape == (4100, 4100)):
    continue
  path = []
  while shape != (4100, 4100):
    path.append(shape)
    unused, shape = firststep[shape]
  paths.append(path)

# |suffixfree| is the suffix-free union of |paths|
suffixfree = []

for path in paths:
  is_a_suffix = False
  for otherpath in paths:
    if (len(otherpath) <= len(path)):
      continue
    if (otherpath[-len(path):] == path):
      is_a_suffix = True
      break
  if (not is_a_suffix):
    suffixfree.append(path)

# Sort in place.
suffixfree.sort(key=len)

rows = 7
columns = max([len(path) for path in suffixfree])
pdf = PdfPages('solution.pdf')
for pagenumber in range(len(suffixfree)//rows):
  pyplot.figure(figsize=(11.69,8.27))
  for row in range(min(rows, len(suffixfree) - rows*pagenumber)):
    path = suffixfree[rows*pagenumber + row]
    for column in range(len(path)):
      pyplot.subplot(rows, columns, columns * row + column + 1)
      pyplot.plot([0,1],[0,1])
  pdf.savefig(papertype="a4", orientation="portrait")
  pyplot.close()
pdf.close()
