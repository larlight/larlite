"""
Usage: script [file1.root] [file2.root]
This function compares an old root file to a new root file, it checks the contents 
of the waveform data and the event header and outputs an error with information of
the entry number.  
The event header check uses the operator== of the class, which checks to ensure 
they are exactly the same.
The waveform data check uses EXOWaveformData::ContainsAllWaveformsIn() which checks
to ensure that the old waveform data contains all the waveforms in the new waveform data.
Alternatively one could call the operator== of the class in which case they must have 
the equal number of waveforms.
"""

import sys,ROOT,os

if len(sys.argv) != 3:
    print "Usage: script [file1.root] [file2.root]"
    sys.exit(1)

# Make sure we can see debug output
#ROOT.EXOErrorLogger.GetLogger().SetOutputThreshold(0)

old_file = ROOT.TFile(sys.argv[1])
new_file = ROOT.TFile(sys.argv[2])

differences = 0

for tree in ['flashmatch_tree', 'int_tree','track_tree','_eff_tree','_flash_tree']:
    old_tmp_tree = old_file.Get(tree)
    new_tmp_tree = new_file.Get(tree)
    
    if old_tmp_tree.GetEntries() != new_tmp_tree.GetEntries():
        print "Trees (%s) do not have equal numbers of entries" % tree
        sys.exit(1)

    if tree == 'flashmatch_tree':
        for branch in ['npe','fy','fz','tx','ty','tz','ft','mct','mcE','mcx','mcy','mcz','mc_dx','score','trk_shift']:
            for i in range(old_tmp_tree.GetEntries()):
                old_tmp_tree.GetEntry(i)
                new_tmp_tree.GetEntry(i)
                if not ( abs( getattr(old_tmp_tree, branch) - getattr(new_tmp_tree, branch) ) < 0.001  ):
                    print
                    print "tree:branch:Entry : %s:%s:%i Do not match" % (tree, branch, i)
                    print "old_tmp_tree", branch, getattr(old_tmp_tree, branch)
                    print "new_tmp_tree", branch, getattr(new_tmp_tree, branch)
                    differences = differences + 1

    if tree == 'int_tree':
        for branch in ['_t0','_n_pe','_int_e','_n_flash']:
            for i in range(old_tmp_tree.GetEntries()):
                old_tmp_tree.GetEntry(i)
                new_tmp_tree.GetEntry(i)
                if not ( abs( getattr(old_tmp_tree, branch) - getattr(new_tmp_tree, branch) ) < 0.001  ):
                    print
                    print "tree:branch:Entry : %s:%s:%i Do not match" % (tree, branch, i)
                    print "old_tmp_tree", branch, getattr(old_tmp_tree, branch)
                    print "new_tmp_tree", branch, getattr(new_tmp_tree, branch)
                    differences = differences + 1

    if tree == 'track_tree':
        for branch in ['trk_time','trk_max_x','trk_min_x','trk_shift','trk_x']:
            for i in range(old_tmp_tree.GetEntries()):
                old_tmp_tree.GetEntry(i)
                new_tmp_tree.GetEntry(i)
                if not ( abs( getattr(old_tmp_tree, branch) - getattr(new_tmp_tree, branch) ) < 0.001  ):
                    print
                    print "tree:branch:Entry : %s:%s:%i Do not match" % (tree, branch, i)
                    print "old_tmp_tree", branch, getattr(old_tmp_tree, branch)
                    print "new_tmp_tree", branch, getattr(new_tmp_tree, branch)
                    differences = differences + 1

    if tree == '_eff_tree':
        for branch in ['_matched','_mc_time','_mc_edep','_flash_time','_npe','_npts','trk_max_x','trk_min_x','trk_max_abs_x','trk_min_abs_x','trk_shift']:
            for i in range(old_tmp_tree.GetEntries()):
                old_tmp_tree.GetEntry(i)
                new_tmp_tree.GetEntry(i)
                if not ( abs( getattr(old_tmp_tree, branch) - getattr(new_tmp_tree, branch) ) < 0.001  ):
                    print
                    print "tree:branch:Entry : %s:%s:%i Do not match" % (tree, branch, i)
                    print "old_tmp_tree", branch, getattr(old_tmp_tree, branch)
                    print "new_tmp_tree", branch, getattr(new_tmp_tree, branch)
                    differences = differences + 1

    if tree == '_flash_tree':
        for i in range(old_tmp_tree.GetEntries()):
            old_tmp_tree.GetEntry(i)
            new_tmp_tree.GetEntry(i)
            if not ( abs( getattr(old_tmp_tree, '_npe') - getattr(new_tmp_tree, '_npe') ) < 0.001  ):
                    print
                    print "tree:branch:Entry : %s:_npe:%i Do not match" % (tree, i)
                    print "old_tmp_tree _npe", getattr(old_tmp_tree, '_npe')
                    print "new_tmp_tree _npe", getattr(new_tmp_tree, '_npe')
                    differences = differences + 1

print
print "Finished processing Root Files"
print
print "%i differences found" % differences
print
# _npe


