#/usr/bin/env python
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

for arg in argv[1:]:
  drawshape(arg)
