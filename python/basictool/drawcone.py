import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def rot(XYZ,theta,phi):
    '''Returns the points XYZ rotated by the given euler angles'''

    ERotY = np.array([[np.cos(theta) , 0, np.sin(theta)],
                      [0,              1,             0],
                      [-np.sin(theta), 0, np.cos(theta)]])

    ERotZ = np.array([[np.cos(phi), -np.sin(phi), 0],
                      [np.sin(phi), np.cos(phi),  0],
                      [0,          0,             1]])


    x,y,z = ERotY.dot(XYZ)

    XYZ = np.vstack([x,y,z])

    return ERotZ.dot(XYZ)


