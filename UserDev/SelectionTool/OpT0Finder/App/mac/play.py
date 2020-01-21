import sys,ROOT
from ROOT import flashana,TH1D,TGraph

def pmt_pos():
    xv = ROOT.std.vector("double")()
    yv = ROOT.std.vector("double")()
    zv = ROOT.std.vector("double")()
    for line in open('opt_geo.txt','r').read().split('\n'):
        words = line.split()
        if not len(words) == 3: continue
        xv.push_back(float(words[0]))
        yv.push_back(float(words[1]))
        zv.push_back(float(words[2]))
    return (xv,yv,zv)

def get_alg():
    xv,yv,zv = pmt_pos()
    match_alg = flashana.QLLMatch.GetME()
    match_alg.SetOpDetPositions(xv,yv,zv)

    return match_alg

def hypothesis(pt_v):

    res=None
    alg = get_alg()

    q_v = flashana.QCluster_t()
    for pt in pt_v:
        q = flashana.QPoint_t()
        q.x = pt[0]
        q.y = pt[1]
        q.z = pt[2]
        q.q = pt[3]
        q_v.push_back(q)

    hypothesis = alg.ChargeHypothesis(q_v)
    g=TGraph(hypothesis.size())
    g.SetMarkerSize(1)
    g.SetMarkerStyle(22)
    g.SetMarkerColor(ROOT.kBlue)
    for x in range(hypothesis.size()):
        #h.SetBinContent(x+1,hypothesis[x])
        g.SetPoint(x,x,hypothesis[x])

    #return h
    return g
    
def make_graph(x,y,z,q=1):

    pt_v=flashana.QCluster_t()
    pt  =flashana.QPoint_t()
    pt.x = x
    pt.y = y
    pt.z = z
    pt.q = q
    pt_v.push_back(pt)

    hypothesis = get_alg().ChargeHypothesis(pt_v)

    #h=TH1D("h","",hypothesis.size(),-0.5,hypothesis.size()-0.5)
    g=TGraph(hypothesis.size())
    g.SetMarkerSize(1)
    g.SetMarkerStyle(22)
    g.SetMarkerColor(ROOT.kBlue)
    for x in range(hypothesis.size()):
        #h.SetBinContent(x+1,hypothesis[x])
        g.SetPoint(x,x,hypothesis[x])

    #return h
    return g
