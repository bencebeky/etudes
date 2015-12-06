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
from matplotlib import gridspec
from matplotlib import patches
from matplotlib import pyplot
from matplotlib.backends.backend_pdf import PdfPages
from sys import argv

pyplot.interactive(False)

# Transform a line segment from the coordinate system of a single side to the
# coordinate system of the subplot.
def drawline(ax, voffset, data):
  data = numpy.array(data).T
  data[:,1] += 2 * voffset
  ax.lines.extend([patches.Polygon(0.5 + 0.5 * data,
    transform=ax.transAxes, facecolor="grey", edgecolor="black", linewidth=1.5)])

# Draw a single side at given vertical offset.
def drawshape(ax, side, voffset):
  angle = 0.0
  short = numpy.sqrt(0.5) / numpy.cos(numpy.pi/12)
  while (side > 0):
    assert(side % 3 != 0);
    angle1 = angle + numpy.pi / 6;
    angle2 = angle + numpy.pi / 3;
    if (side % 3 == 1):
      drawline(ax, voffset,
        [[0.0, short * numpy.sin(angle), short * numpy.sin(angle1)],
         [0.0, + short * numpy.cos(angle), short * numpy.cos(angle1)]])
      angle = angle1
    else:
      drawline(ax, voffset,
        [[0.0, short * numpy.sin(angle), numpy.sin(angle1), short * numpy.sin(angle2)],
         [0.0, short * numpy.cos(angle), numpy.cos(angle1), short * numpy.cos(angle2)]])
      angle = angle2
    side //= 3;

# Draw two sides in given subplot.
def drawshapes(ax, shapes):
  ax.set_axis_off()
  ax.set_aspect(1.0)
  drawshape(ax, shapes[0], 0.0)
  drawshape(ax, shapes[1], 1.0)
  ax.lines.extend([lines.Line2D([0.5, 0.5], [0.0, 2.0], transform=ax.transAxes, linewidth=1.5)])

# Read solution file and generate |firststep|.  For each shape, |firststep|
# contains the first step to take on one of the shortest paths to the root.
def read_firststep():
  firststep = {}
  with open("solution.dat") as f:
    for line in f:
      words = line.split();
      assert(len(words) == 6)
      firststep[(int(words[0]), int(words[1]))] = [
          (int(words[2]), int(words[3])),
          (int(words[4]), int(words[5]))]
  return firststep

# Generate maximal list of suffix-free paths.
def generate_suffixfree(firststep):
  # |paths| is the list of all paths from all shapes to one step before the root.
  paths = []

  for shape in firststep.keys():
    if (shape == (4100, 4100)):
      continue
    path = []
    while shape != (4100, 4100):
      rotated, shape = firststep[shape]
      path.append(rotated)
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

  return suffixfree

def plot(suffixfree):
  rows = 6
  columns = max([len(path) for path in suffixfree])
  pdf = PdfPages("solution.pdf")
  for pagenumber in range(int(numpy.ceil(len(suffixfree)/rows))):
    fig = pyplot.figure(figsize=(8.27,11.69))
    gs = gridspec.GridSpec(rows, columns, top=0.95, bottom=0.0, wspace=0.2, hspace=0.0)
    for row in range(min(rows, len(suffixfree) - rows*pagenumber)):
      path = suffixfree[rows*pagenumber + row]
      for column in range(len(path)):
        ax = fig.add_subplot(gs[row, column])
        drawshapes(ax, path[column])
    pdf.savefig()
    pyplot.close(fig)
  pdf.close()


firststep = read_firststep()
suffixfree = generate_suffixfree(firststep)
plot(suffixfree)
