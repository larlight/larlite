import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def sphere(origin,radius):
    '''Returns x,y,z points to draw as a sphere'''

    u,v = np.mgrid[0:2*np.pi:20j, 0:2*np.pi:20j]
    x = origin[0] + radius*np.cos(u)*np.sin(v)
    y = origin[1] + radius*np.sin(u)*np.sin(v)
    z = origin[2] + radius*np.cos(v)

    return x,y,z

