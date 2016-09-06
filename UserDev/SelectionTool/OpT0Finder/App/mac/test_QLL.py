import sys,ROOT
import os
#ROOT.gSystem.Load("libOpFlashAna_OpT0FinderApp")
from larlite import larlite as fmwk
from ROOT import flashana,TChain,larutil

PRODUCER_NAME="opFlash"

infile = sys.argv[1]

indir = infile.split('/')[-2]

nphoton,x,y,z = ( int(indir.split('_')[1]),
                  float(indir.split('_')[-3])/10.,
                  float(indir.split('_')[-2])/10.,
                  float(indir.split('_')[-1])/10.)

print x,y,z

geo = larutil.Geometry.GetME()

match_alg = flashana.QLLMatch.GetME()
match_mgr = flashana.FlashMatchManager()
match_mgr.SetAlgo(match_alg)
match_mgr.SetAlgo(flashana.PhotonLibHypothesis())
match_mgr.Configure( "%s/SelectionTool/OpT0Finder/App/mac/flashmatch.fcl" % os.environ['LARLITE_USERDEVDIR'])

tch = TChain("opflash_%s_tree" % PRODUCER_NAME)
tch.AddFile(infile)

for entry in xrange(tch.GetEntries()):

    tch.GetEntry(entry)

    br = None
    exec('br = tch.opflash_%s_branch' % PRODUCER_NAME)

    # Use highest OpFlash for now
    flash = None
    for flash_index in xrange(br.size()):

        if not flash or flash.TotalPE() < br[flash_index].TotalPE():

            flash = br[flash_index]

    reco_flash = flashana.Flash_t()
    reco_flash.pe_v.resize(geo.NOpDets())

    for ch in xrange(32):

        pmt_index = geo.OpDetFromOpChannel(ch)

        if pmt_index >= 32: continue

        reco_flash.pe_v[pmt_index] = flash.PE(ch)

    mc_flash = flashana.QCluster_t()
    mc_flash.push_back(flashana.QPoint_t(x,y,z,nphoton))

    print "QLL:",match_alg.CallMinuit(mc_flash,reco_flash)
    print "Look at a history..."
    xval_history = match_alg.HistoryX()
    qll_history  = match_alg.HistoryQLL()

    for history_index in xrange(xval_history.size()):
        print qll_history[history_index],'at x=',xval_history[history_index]

    break

    
