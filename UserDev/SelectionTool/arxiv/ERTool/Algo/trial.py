from ROOT import ertool
k=ertool.ERAlgoToy("k")
j=ertool.Manager()
j.AddCfgFile("aho.txt")
j.AddAlgo(k)
j.Initialize()
#j.StorePSet("aho.txt")
