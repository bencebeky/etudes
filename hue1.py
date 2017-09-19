import numpy
import cv2
from com.dtmilano.android.adb.adbclient import AdbClient
from scipy.ndimage import label
from matplotlib import pyplot

pyplot.interactive(True)
screen = AdbClient(serialno='.*').takeSnapshot()
#screen = cv2.imread('screen.png')
rgb = numpy.array(screen)[::4,::4,:3]
rgb2 = numpy.array(rgb[:,:,0], dtype=numpy.uint32)
rgb2 = rgb2 + 256 * rgb[:,:,1] + 256 * 256 * rgb[:,:,2]
colors = numpy.unique(rgb2.flatten())
labels = numpy.zeros(rgb.shape[:2])
label = 1
thresholdmin = numpy.prod(rgb.shape[:2])/500
thresholdmax = 10 * thresholdmin
tilecolors = []
for color in colors:
  mask = rgb2 == color
  if (numpy.sum(mask) < thresholdmin or numpy.sum(mask) > thresholdmax):
    continue
  labels[mask] = label
  label += 1
  tilecolors.append(rgb[mask][0])
label -= 1
tilecolors = numpy.array(tilecolors)
figure, axarr = pyplot.subplots(1, 2)
spaceplot = axarr[0]
spaceplot.set_xlim(0, rgb.shape[1])
spaceplot.set_ylim(-rgb.shape[0], 0)
xarray, yarray = numpy.meshgrid(numpy.arange(rgb.shape[1]), numpy.arange(rgb.shape[0]))
for i in range(label):
  mask = labels == i+1
  spaceplot.text(numpy.mean(xarray[mask]), -numpy.mean(yarray[mask]), str(i+1))
colorplot = axarr[1]
xcolor = tilecolors[:,0]
ycolor = -tilecolors[:,2]
colorplot.set_xlim(numpy.min(xcolor)-10, numpy.max(xcolor)+10)
colorplot.set_ylim(numpy.max(ycolor)+10, numpy.min(ycolor)-10)
for i in range(label):
  colorplot.text(xcolor[i], ycolor[i], str(i+1))
