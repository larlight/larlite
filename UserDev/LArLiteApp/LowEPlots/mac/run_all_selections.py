import os, datetime

_128kV = False
_use_reco = True

if _128kV: input_base = '/pnfs/uboone/persistent/users/oscillations_group/LowEExcess/128kV_MCC6Era/'
else: input_base = '/pnfs/uboone/persistent/users/oscillations_group/LowEExcess/70kV/'

if _128kV:
	cosmics_files = input_base + 'ben_cosmics/larlite_mcinfo_merge.root'
	dirt_files = input_base + 'prodgenie_bnb_dirt_nu_uboone/larlite_mcinfo.root'
	bnb_files = input_base + 'prodgenie_bnb_nu_uboone/larlite_mcinfo_merge.root'
else:
	cosmics_files = input_base + 'osc_cosmics_70kv/osc_cosmics_70kv_all_reco3d_fuzzyshower.root '
	cosmics_files += input_base + 'osc_cosmics_70kv/osc_cosmics_70kv_all_reco3d_kalmanhitcc.root '
	cosmics_files += input_base + 'osc_cosmics_70kv/osc_cosmics_70kv_all_mcinfo.root'
	dirt_files = None
	bnb_files = input_base + 'osc_bnb_70kv/osc_bnb_70kv_all_reco3d_fuzzyshower.root '
	bnb_files += input_base + 'osc_bnb_70kv/osc_bnb_70kv_all_reco3d_kalmanhitcc.root '
	bnb_files += input_base + 'osc_bnb_70kv/osc_bnb_70kv_all_mcinfo.root'


starttime = datetime.datetime.now()
print "run_all_selections start time is",starttime
if cosmics_files: os.system('python singleE_cosmic_selection.py %s %s'%('reco' if _use_reco else 'mc',cosmics_files))
if dirt_files: os.system('python singleE_dirt_selection.py %s %s'%('reco' if _use_reco else 'mc',dirt_files))
if bnb_files: os.system('python singleE_nc_selection.py %s %s'%('reco' if _use_reco else 'mc',bnb_files))
if bnb_files: os.system('python singleE_nue_selection.py %s %s'%('reco' if _use_reco else 'mc',bnb_files))
if bnb_files: os.system('python singleE_numu_selection.py %s %s'%('reco' if _use_reco else 'mc',bnb_files))
print "run_all_selections total time duration is",datetime.datetime.now()-starttime