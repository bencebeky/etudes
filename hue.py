import Tkinter as tk
import numpy
import cv2
from PIL import Image, ImageTk
from com.dtmilano.android.adb.adbclient import AdbClient

class Hue(object):
  def __init__(self, window):
    self.window = window
    self.window.title("Stretch hue")
    self.window.config(background="#ffffff")

    rgb = numpy.zeros([640, 360, 3], dtype=numpy.uint8)
    self.hsv = cv2.cvtColor(rgb, cv2.COLOR_RGB2HSV)
    self.huearray = self.hsv[:,:,0]
    self.hueimg = Image.fromarray(self.huearray)
    self.huetk = ImageTk.PhotoImage(image=self.hueimg)
    self.satarray = self.hsv[:,:,1]
    self.satimg = Image.fromarray(self.satarray)
    self.sattk = ImageTk.PhotoImage(image=self.satimg)
    self.valarray = self.hsv[:,:,2]
    self.valimg = Image.fromarray(self.valarray)
    self.valtk = ImageTk.PhotoImage(image=self.valimg)

    self.hueframe = tk.Frame()
    self.huelabel = tk.Label(self.hueframe, image=self.huetk)
    self.huelabel.pack()
    self.huescale0 = tk.Scale(self.hueframe, from_=0, to=179, orient=tk.HORIZONTAL, command=self.huescalecommand)
    self.huescale0.pack(side=tk.LEFT)
    self.huescale1 = tk.Scale(self.hueframe, from_=0, to=179, orient=tk.HORIZONTAL, command=self.huescalecommand)
    self.huescale1.set(255)
    self.huescale1.pack()
    self.huescale2 = tk.Scale(self.hueframe, from_=0, to=179, orient=tk.HORIZONTAL, command=self.huescalecommand)
    self.huescale2.pack(side=tk.TOP)
    self.hueframe.pack(side=tk.LEFT)

    self.satframe = tk.Frame()
    self.satframe.pack(side=tk.LEFT)
    self.satlabel = tk.Label(self.satframe, image=self.sattk)
    self.satlabel.pack()
    self.satscale0 = tk.Scale(self.satframe, from_=0, to=255, orient=tk.HORIZONTAL,
    command=self.satscalecommand)
    self.satscale0.pack(side=tk.LEFT)
    self.satscale1 = tk.Scale(self.satframe, from_=0, to=255, orient=tk.HORIZONTAL,
    command=self.satscalecommand)
    self.satscale1.set(255)
    self.satscale1.pack()

    self.valframe = tk.Frame()
    self.valframe.pack(side=tk.LEFT)
    self.vallabel = tk.Label(self.valframe, image=self.valtk)
    self.vallabel.pack()
    self.valscale0 = tk.Scale(self.valframe, from_=0, to=255, orient=tk.HORIZONTAL, command=self.valscalecommand)
    self.valscale0.pack(side=tk.LEFT)
    self.valscale1 = tk.Scale(self.valframe, from_=0, to=255, orient=tk.HORIZONTAL, command=self.valscalecommand)
    self.valscale1.set(255)
    self.valscale1.pack()

    self.imgframe = tk.Frame()
    self.imgframe.pack(side=tk.LEFT)
    self.combinedimg = Image.fromarray(rgb)
    self.combinedtk = ImageTk.PhotoImage(image=self.combinedimg)
    self.imglabel = tk.Label(self.imgframe, image=self.combinedtk)
    self.imglabel.pack(side=tk.TOP)

    self.timercommand()

  def capture(self):
    screen = AdbClient(serialno='.*').takeSnapshot()
    rgb = numpy.array(screen)[::3,::3,:3]
    self.hsv = cv2.cvtColor(rgb, cv2.COLOR_RGB2HSV)

  def refreshhue(self):
    huemin = self.huescale0.get()
    huemax = self.huescale1.get()
    if (huemin == huemax):
      return
    huearray = numpy.array(self.hsv[:,:,0], dtype=numpy.float64)
    huearray -= huemin
    huearray[huearray < 0] += 180
    huemax -= huemin
    if (huemax <= 0):
      huemax += 180
    huearray = huearray * 179 / huemax
    mask = huearray > 179
    huearray[mask] = 0
    huearray += self.huescale2.get()
    huearray[huearray > 179] -= 180
    hueimage = numpy.empty([huearray.shape[0], huearray.shape[1], 3], dtype=numpy.uint8)
    hueimage[:,:,0] = huearray * 255 / 179
    hueimage[:,:,1] = huearray * 255 / 179
    hueimage[:,:,2] = huearray * 255 / 179
    hueimage[mask, 0] = 255
    hueimage[mask, 1] = 0
    hueimage[mask, 2] = 0
    self.huearray = numpy.array(huearray, dtype=numpy.uint8)
    self.hueimg = Image.fromarray(hueimage)
    self.huetk = ImageTk.PhotoImage(image=self.hueimg)
    self.huelabel.config(image=self.huetk)

  def refreshsat(self):
    satmin = self.satscale0.get()
    satmax = self.satscale1.get()
    if (satmin >= satmax):
      return
    satarray = numpy.array(self.hsv[:,:,1], dtype=numpy.float64)
    satarray = (satarray - satmin) * 255 / (satmax - satmin)
    satimage = numpy.empty([satarray.shape[0], satarray.shape[1], 3], dtype=numpy.uint8)
    satimage[:,:,0] = satarray
    satimage[:,:,1] = satarray
    satimage[:,:,2] = satarray
    mask = satarray < 0
    satimage[mask, 0] = 255
    satimage[mask, 1] = 0
    satimage[mask, 2] = 0
    satarray[mask] = 0
    mask = satarray > 255
    satimage[mask, 0] = 255
    satimage[mask, 1] = 0
    satimage[mask, 2] = 0
    satarray[mask] = 255
    self.satarray = numpy.array(satarray, dtype=numpy.uint8)
    self.satimg = Image.fromarray(satimage)
    self.sattk = ImageTk.PhotoImage(image=self.satimg)
    self.satlabel.config(image=self.sattk)

  def refreshval(self):
    valmin = self.valscale0.get()
    valmax = self.valscale1.get()
    if (valmin >= valmax):
      return
    valarray = numpy.array(self.hsv[:,:,2], dtype=numpy.float64)
    valarray = (valarray - valmin) * 255 / (valmax - valmin)
    valimage = numpy.empty([valarray.shape[0], valarray.shape[1], 3], dtype=numpy.uint8)
    valimage[:,:,0] = valarray
    valimage[:,:,1] = valarray
    valimage[:,:,2] = valarray
    mask = valarray < 0
    valimage[mask, 0] = 255
    valimage[mask, 1] = 0
    valimage[mask, 2] = 0
    valarray[mask] = 0
    mask = valarray > 255
    valimage[mask, 0] = 255
    valimage[mask, 1] = 0
    valimage[mask, 2] = 0
    valarray[mask] = 255
    self.valarray = numpy.array(valarray, dtype=numpy.uint8)
    self.valimg = Image.fromarray(valimage)
    self.valtk = ImageTk.PhotoImage(image=self.valimg)
    self.vallabel.config(image=self.valtk)

  def combine(self):
    hsv = numpy.empty([self.huearray.shape[0], self.huearray.shape[1], 3], dtype=numpy.uint8)
    hsv[:,:,0] = self.huearray
    hsv[:,:,1] = self.satarray
    hsv[:,:,2] = self.valarray
    combined = cv2.cvtColor(hsv, cv2.COLOR_HSV2RGB)
    self.combinedimg = Image.fromarray(combined)
    self.combinedtk = ImageTk.PhotoImage(image=self.combinedimg)
    self.imglabel.config(image = self.combinedtk)

  def huescalecommand(self, scale):
    self.refreshhue()
    self.combine()
    self.window.update_idletasks()

  def satscalecommand(self, scale):
    self.refreshsat()
    self.combine()
    self.window.update_idletasks()

  def valscalecommand(self, scale):
    self.refreshval()
    self.combine()
    self.window.update_idletasks()

  def timercommand(self):
    self.capture()
    self.refreshhue()
    self.refreshsat()
    self.refreshval()
    self.combine()
    self.window.update_idletasks()
    self.window.after(1000, self.timercommand)

window = tk.Tk()
hue = Hue(window)
window.mainloop()
