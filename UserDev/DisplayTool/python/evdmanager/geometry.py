
from larlite import larlite as fmwk
import ROOT
from larlite import larutil
import numpy as np


class geoBase(object):

    """docstring for geometry"""

    def __init__(self):
        super(geoBase, self).__init__()
        self._nViews = 2
        self._tRange = 1600
        self._wRange = [240, 240]
        self._aspectRatio = 4
        self._time2Cm = 0.1
        self._wire2Cm = 0.4
        self._levels = [(-15, 15), (-10, 30)]
        self._name = "null"
        self._offset = [0, 0]

    def nViews(self):
        return self._nViews

    def tRange(self):
        return self._tRange

    def wRange(self, plane):
        return self._wRange[plane]

    def getLevels(self, plane):
        return self._levels[plane]

    def aspectRatio(self):
        return self._aspectRatio

    def getBlankData(self, plane):
        return np.ones((self._wRange[plane], self._tRange))

    def wire2cm(self):
        return self._wire2Cm

    def time2cm(self):
        return self._time2Cm

    def name(self):
        return self._name

    def offset(self, plane):
        return self._offset[plane]


class geometry(geoBase):

    def __init__(self):
        super(geometry, self).__init__()
        self._nViews = larutil.Geometry.GetME().Nviews()
        self._time2Cm = larutil.GeometryUtilities.GetME().TimeToCm()
        self._wire2Cm = larutil.GeometryUtilities.GetME().WireToCm()
        self._aspectRatio = self._wire2Cm / self._time2Cm
        self._tRange = larutil.DetectorProperties.GetME().ReadOutWindowSize()
        self._wRange = []
        self._offset = []
        for v in range(0, self._nViews):
            self._wRange.append(larutil.Geometry.GetME().Nwires(v))
            # Set up the correct drift time offset.
            # Offset is returned in terms of centimeters.

            self._offset.append(
                larutil.DetectorProperties.GetME().TriggerOffset()
                * larutil.GeometryUtilities.GetME().TimeToCm()
                - larutil.Geometry.GetME().PlaneOriginVtx(v)[0])

        self._defaultColorScheme = []

    def colorMap(self, plane):
        return self._defaultColorScheme[plane]


class microboone(geometry):

    def __init__(self):
        # Try to get the values from the geometry file.  Configure for microboone
        # and then call the base class __init__
        larutil.LArUtilManager.Reconfigure(fmwk.geo.kMicroBooNE)
        super(microboone, self).__init__()
        self._levels = [(-100, 10), (-10, 100), (-10, 100)]
        # self._colorScheme =
        # self._time2Cm = 0.05515
        self._aspectRatio = self._wire2Cm / self._time2Cm
        self._pedestals = [2000, 2000, 440]
        self._name = "uboone"
        self._defaultColorScheme = [(
            {'ticks': [(1, (22, 30, 151, 255)),
                       (0.791, (0, 181, 226, 255)),
                       (0.645, (76, 140, 43, 255)),
                       (0.47, (0, 206, 24, 255)),
                       (0.33333, (254, 209, 65, 255)),
                       (0, (255, 0, 0, 255))],
             'mode': 'rgb'})]
        self._defaultColorScheme.append(
            {'ticks': [(0, (22, 30, 151, 255)),
                       (0.33333, (0, 181, 226, 255)),
                       (0.47, (76, 140, 43, 255)),
                       (0.645, (0, 206, 24, 255)),
                       (0.791, (254, 209, 65, 255)),
                       (1, (255, 0, 0, 255))],
             'mode': 'rgb'})
        # self._defaultColorScheme.append(
        #     {'ticks': [(0, (22, 30, 151, 255)),
        #                (0.33333, (0, 181, 226, 255)),
        #                (0.47, (76, 140, 43, 255)),
        #                (0.645, (0, 206, 24, 255)),
        #                (0.791, (254, 209, 65, 255)),
        #                (1, (255, 0, 0, 255))],
        #      'mode': 'rgb'})

        #   Using a package to generate the palette:
        self._levels[-1] = (-50,2000)
        import palettable
        from palettable.cubehelix import Cubehelix
        palette = Cubehelix.make(start=1,n=30,reverse=True,rotation=2,min_light=.2,max_light=0.8)
        # palette = palettable.cubehelix.perceptual_rainbow_16
        # palette = palettable.cubehelix.cubehelix3_16_r
        # palette = palettable.cubehelix.classic_16_r
        npoints = palette.number
        a = []
        power = 2
        a.append((0.0,(255,255,255)))
        for point in xrange(npoints):
            step = (1.0*(point+1)**power) / (npoints**power)
            # step = 1.0* (point+1) / npoints
            # print "point, step", point, step
            a.append((step, tuple(palette.colors[point])))

        # print a
        self._defaultColorScheme.append({'ticks': a, 'mode': 'rgb'})


        # # Default color scheme but fading into white at high Q
        # self._levels[-1] = (-10,2000)
        # self._defaultColorScheme.append(
        #     {'ticks': [(0000./2010, (255, 255, 255, 255)),
        #                # (0010./2010, (0, 15, 151, 205)),
        #                (0037./2010, (000, 181, 226, 200)),
        #                (0052./2010, (192, 065, 255, 200)),
        #                (0071./2010, (255, 050, 255, 200)),
        #                (087./2010,  (255,  38,  49, 200)),
        #                (0110./2010, (255,  94,  80, 255)),
        #                (0200./2010, (255, 197, 051, 255)),
        #                (0300./2010, (255, 223, 060, 255)),
        #                (0400./2010, (255, 247, 001, 255)),
        #                (0550./2010, (166, 255, 001, 255)),
        #                (0700./2010, ( 81, 023, 000, 255)),
        #                (0850./2010, (000, 153,  69, 255)),
        #                (1000./2010, (001, 135, 175, 255)),
        #                (1250./2010, (002, 154, 255, 255)),
        #                # (1500./2010, (255, 0, 0, 025)),
        #                (2010./2010, (021, 000, 255, 255))],
        #      'mode': 'rgb'})

        # # Default color scheme but fading into white at high Q
        # self._levels[-1] = (-10,2000)
        # self._defaultColorScheme.append(
        #     {'ticks': [(0000./2010, (255, 255, 255, 255)),
        #                # (0010./2010, (0, 15, 151, 205)),
        #                (0037./2010, (0, 181, 226, 255)),
        #                (0052./2010, (76, 140, 43, 255)),
        #                (0071./2010, (0, 206, 24, 255)),
        #                (087./2010, (254, 209, 65, 255)),
        #                (0110./2010, (255, 0, 0, 255)),
        #                (0200./2010, (255, 0, 0, 225)),
        #                (0300./2010, (255, 0, 0, 200)),
        #                (0400./2010, (255, 0, 0, 175)),
        #                (0550./2010, (255, 0, 0, 150)),
        #                (0700./2010, (255, 0, 0, 125)),
        #                (0850./2010, (255, 0, 0, 100)),
        #                (1000./2010, (255, 0, 0, 075)),
        #                (1250./2010, (255, 0, 0, 050)),
        #                (1500./2010, (255, 0, 0, 025)),
        #                (2010./2010, (255, 0, 0, 000))],
        #      'mode': 'rgb'})
        # # Grayscale to logrithmic
        # self._levels[-1] = (-10,2010)
        # self._defaultColorScheme.append(
        #     {'ticks': [(0000./2010, (000,000,000, 0)),
        #                (0010./2010, (000,000,000, 250)),
        #                # (0010./2010, (000,181,226, 250)),
        #                (0015./2010, (050,101,156, 250)),
        #                (0025./2010, (050,101,156, 225)),
        #                (0035./2010, (050,101,156, 200)),
        #                (0045./2010, (050,101,156, 175)),
        #                (0075./2010, (050,101,156, 150)),
        #                (0100./2010, (050,101,156, 150)),
        #                (0125./2010, (050,101,156, 125)),
        #                (0150./2010, (050,101,156, 100)),
        #                (0200./2010, (050,101,156, 075)),
        #                (0250./2010, (050,101,156, 050)),
        #                (0300./2010, (050,101,156, 000)),
        #                (0500./2010, (000,206,024, 200)),
        #                (1000./2010, (254,209,065, 225)),
        #                (1500./2010, (254,209,065, 255)),
        #                (2010./2010, (255,000,000, 255))],
        #      'mode': 'rgb'})

        # # Black background to monochomatic fade to white:
        # self._levels[-1] = (-10,2010)
        # self._defaultColorScheme.append(
        #     {'ticks': [(0000./2010, (000,000,150, 255)),
        #                (0005./2010, (000,000,150, 255)),
        #                (0020./2010, (000,000,150, 225)),
        #                (0030./2010, (000,000,150, 175)),
        #                (0050./2010, (000,000,150, 125)),
        #                (0100./2010, (000,000,150, 075)),
        #                (0200./2010, (000,000,150, 050)),
        #                (0300./2010, (000,000,150, 000)),
        #                (0500./2010, (000,000,150, 075)),
        #                (1000./2010, (000,000,150, 050)),
        #                (2000./2010, (000,000,150, 025))],
        #                # (0000/2010, (0,0,255, 100)),
        #                # (0000/2010, (0,0,255, 050)),
        #                # (0000/2010, (0,0,255, 000))],
        #      'mode': 'rgb'})


  # Schemes:
  # Grey through teal to white to g/y/r:
  #       self._levels[-1] = (-10,2010)
  #       self._defaultColorScheme.append(
  #            {'ticks': [(0000./2010, (060,060,060, 225)),
  #                      (0010./2010, (075,075,075, 225)),
  #                      # (0010./2010, (000,181,226, 250)),
  #                      (0015./2010, (031,177,229, 250)),
  #                      (0025./2010, (000,234,255, 225)),
  #                      (0035./2010, (50,101,156, 200)),
  #                      (0045./2010, (50,101,156, 175)),
  #                      (0075./2010, (50,101,156, 150)),
  #                      (0100./2010, (50,101,156, 150)),
  #                      (0150./2010, (50,101,156, 100)),
  #                      (0300./2010, (50,101,156, 000)),
  #                      (0500./2010, (000,206,024, 200)),
  #                      (1000./2010, (254,209,065, 225)),
  #                      (1500./2010, (254,209,065, 255)),
  #                      (2010./2010, (255,000,000, 255))],
  #            'mode': 'rgb'})

  # Reverse nominal display (U plane, levels from 10 to -100)
  # # 
  #             {'ticks': [(1, (22, 30, 151, 255)),
  #                      (0.791, (0, 181, 226, 255)),
  #                      (0.645, (76, 140, 43, 255)),
  #                      (0.47, (0, 206, 24, 255)),
  #                      (0.33333, (254, 209, 65, 255)),
  #                      (0, (255, 0, 0, 255))],
  #            'mode': 'rgb'})]

  # # Reverse based on orange:
  #   # Reverse nominal display (U plane, levels from 10 to -100)
  #       self._levels[-1] = (-10,100)
  #       self._defaultColorScheme.append(
  #             {'ticks': [(1, (22, 30, 151, 255)),
  #                      (0.791, (0, 181, 226, 255)),
  #                      (0.645, (76, 140, 43, 255)),
  #                      (0.47, (0, 206, 24, 255)),
  #                      (0.33333, (254, 209, 65, 255)),
  #                      (0, (209, 83, 0, 255))],
  #            'mode': 'rgb'})
             
  #            
class argoneut(geometry):

    def __init__(self):
        # Try to get the values from the geometry file.  Configure for microboone
        # and then call the base class __init__
        larutil.LArUtilManager.Reconfigure(fmwk.geo.kArgoNeuT)
        super(argoneut, self).__init__()
        self._levels = [(-15, 60), (-5, 80)]
        self._pedestals = [0, 0]
        self._name = "argoneut"
        self._offset = []
        for v in range(0, self._nViews):
            # Set up the correct drift time offset.
            # Offset is returned in terms of centimeters.

            self._offset.append(
                larutil.DetectorProperties.GetME().TriggerOffset()
                * larutil.GeometryUtilities.GetME().TimeToCm()
                - larutil.Geometry.GetME().PlaneOriginVtx(v)[0])

        self._defaultColorScheme = [
            {'ticks': [(0.0,  (30,  30, 255, 255)),
                       (0.15, (30,  30, 255, 255)),
                       (0.6,  (0,  255, 255, 255)),
                       (0.8,  (0,  255, 0,   255)),
                       (1,    (255,  0, 0,   255))],
             'mode': 'rgb'}]
        self._defaultColorScheme.append(
            {'ticks': [(0.0,  (30,  30, 255, 255)),
                       (0.15, (30,  30, 255, 255)),
                       (0.6,  (0,  255, 255, 255)),
                       (0.8,  (0,  255, 0,   255)),
                       (1,    (255,  0, 0,   255))],
             'mode': 'rgb'})
        # self._offset = [1.7226813611, 2.4226813611]


class lariat(geometry):

    def __init__(self):
        # Try to get the values from the geometry file.  Configure for microboone
        # and then call the base class __init__
        larutil.LArUtilManager.Reconfigure(fmwk.geo.kArgoNeuT)
        super(lariat, self).__init__()
        # lariat has a different number of time ticks
        # fix it directly:
        self._tRange = 3072
        self._levels = [(-20, 200), (-20, 200)]
        self._pedestals = [0, 0]
        self._name = "lariat"
        # Make default color schemes here:
        self._defaultColorScheme = [
            {'ticks': [(0.0,  (30,  30, 255, 255)),
                       (0.15, (30,  30, 255, 255)),
                       (0.6,  (0,  255, 255, 255)),
                       (0.8,  (0,  255, 0,   255)),
                       (1,    (255,  0, 0,   255))],
             'mode': 'rgb'}]
        self._defaultColorScheme.append(
            {'ticks': [(0.0,  (30,  30, 255, 255)),
                       (0.15, (30,  30, 255, 255)),
                       (0.6,  (0,  255, 255, 255)),
                       (0.8,  (0,  255, 0,   255)),
                       (1,    (255,  0, 0,   255))],
             'mode': 'rgb'})
