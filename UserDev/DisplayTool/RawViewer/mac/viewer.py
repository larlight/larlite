
# Window Management
from Tkinter import *
import Tkinter as Tk
import ttk

# Larlite 
from larlite import larlite as fmwk
from ROOT import example
from ROOT import *

# numpy
import numpy as np

class DrawHandle():
  """docstring for DrawHandle"""
  def __init__(self):
    gSystem.Load("libLArLite_LArUtil")
    gSystem.Load("libargoneut_electrons_EventViewer.so")
    gSystem.Load("libExample_PyPackage.so")   
    self.init_geom()
    self.init_converter()
    self.init_proc()
    self.event=1
    self.nplanes=2

  def init_geom(self):
    larutil.LArUtilManager.Reconfigure(fmwk.geo.kArgoNeuT)
    self.aspectRatio = larutil.GeometryUtilities.GetME().TimeToCm() / larutil.GeometryUtilities.GetME().WireToCm()
  
  def init_converter(self):
    self.c2p = example.PyExample()   

  def init_proc(self):
    self.my_proc = fmwk.ana_processor()
    # Set input root file
    for x in xrange(len(sys.argv)-1):
      self.my_proc.add_input_file(sys.argv[x+1])
    # Specify IO mode
    self.my_proc.set_io_mode(fmwk.storage_manager.kREAD)
    self.my_proc.add_process(fmwk.DrawRaw())

  def proc_event(self, axes):
    self.my_proc.process_event(self.event)
    # for p in xrange(0,self.nplanes):
    d0 = np.array(self.c2p.Convert(self.my_proc.get_process(0).getDataByPlane(0)))
    d1 = np.array(self.c2p.Convert(self.my_proc.get_process(0).getDataByPlane(1)))
    axes[0].imshow(d0.T,aspect=self.aspectRatio,cmap="jet")
    axes[1].imshow(d1.T,aspect=self.aspectRatio,cmap="jet")
    axes[0].set_frame_on(True)

# matplotlib stuff
import matplotlib
matplotlib.use('TkAgg')
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg
# implement the default mpl key bindings
from matplotlib.backend_bases import key_press_handler
import matplotlib.pyplot as plt
from matplotlib.figure import Figure

root = Tk.Tk()
root.wm_title("Embedding in TK")

# Set up the main figure
fig, axes = plt.subplots(2,1,figsize=(10,10))
fig.tight_layout()


dh = DrawHandle()
dh.proc_event(axes)

# from numpy import arange, sin, pi
# t = arange(0.0,3.0,0.01)
# s = sin(2*pi*t)

# axes[0].plot(t,s)
# axes[1].plot(t,s**2)

# a tk.DrawingArea
canvas = FigureCanvasTkAgg(fig, master=root)
canvas.show()
canvas.get_tk_widget().pack(side=Tk.TOP, fill=Tk.BOTH, expand=1)

# toolbar = NavigationToolbar2TkAgg( canvas, root )
# toolbar.update()
# canvas._tkcanvas.pack(side=Tk.TOP, fill=Tk.BOTH, expand=1)


def on_key_event(event):
    print('you pressed %s'%event.key)
    key_press_handler(event, canvas, toolbar)

canvas.mpl_connect('key_press_event', on_key_event)

def _quit():
    root.quit()     # stops mainloop
    root.destroy()  # this is necessary on Windows to prevent
                    # Fatal Python Error: PyEval_RestoreThread: NULL tstate


button = Tk.Button(master=root, text='Quit', command=_quit)
button.pack(side=Tk.BOTTOM)

def _next_event():
  dh.event = dh.event+1
  dh.proc_event(axes)
  canvas.draw()

def _prev_event():
  dh.event = dh.event-1
  dh.proc_event(axes)
  canvas.draw()

_prev_button = Tk.Button(master=root,text='Prev',command=_prev_event)
_prev_button.pack(side=Tk.BOTTOM)

_next_button = Tk.Button(master=root,text='Next',command=_next_event)
_next_button.pack(side=Tk.BOTTOM)

Tk.mainloop()