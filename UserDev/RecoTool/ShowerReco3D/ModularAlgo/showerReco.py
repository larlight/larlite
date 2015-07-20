
from ROOT import showerreco


def getShowerRecoAlgModular():
  # This function returns the default shower reco module
  # If you want to extend, customize, or otherwise alter the default
  # reco (which you should!) it's recommended that you do one of two
  # things:
  #   1) Copy this function, and change the lines you need
  #   2) Use this function as is and replace the modules with the built-in functions

  alg = showerreco.ShowerRecoAlgModular()
  alg.AddShowerRecoModule(showerreco.Axis3DModule()        )
  alg.AddShowerRecoModule(showerreco.StartPoint2DModule()  )
  alg.AddShowerRecoModule(showerreco.StartPoint3DModule()  )
  alg.AddShowerRecoModule(showerreco.ShowerChargeModule()  )

  return alg
