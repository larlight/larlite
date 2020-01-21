from ROOT import TChain

for x in range(40):

    v = (x+1)*5

    ch=TChain("flash_tree")

    ch.AddFile("x_offset_%04d.root" % v)

    print(v,ch.GetEntries())
