from ROOT import ertool
k=ertool.ERAlgoToy()
j=ertool.Manager()
j.AddAlgo(k)
j.Initialize()
j.StorePSet("aho.txt")
