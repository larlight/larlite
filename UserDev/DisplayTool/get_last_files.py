#!/bin/env python
import sys,os,time
from dstream.ds_api import ds_reader
from dstream_online.ds_online_constants import *
import samweb_cli

def DAQFileListToRunMap(flist):

    res = {}
    for fname in flist:
        fname_wo_suffix = fname.replace('.ubdaq','')
        try:
            run,subrun = (int(fname_wo_suffix.split('-')[-2]),int(fname_wo_suffix.split('-')[-1]))
            res[(run,subrun)] = fname
        except Exception:
            continue
    return res

def GetAvailableFileList(NRUNS=500):

    MAIN_RUN_TABLE = 'MainRun'
    LIMIT_NRUNS    = NRUNS
    SAM_LIST_FORMAT_STAGED = "data_tier = raw and file_type = data and file_format = binaryraw-uncompressed and run_number = %d and availability: virtual"
    SAM_LIST_FORMAT_STORED = "data_tier = raw and file_type = data and file_format = binaryraw-uncompressed and run_number = %d and availability: physical"
    FNAME_SUFFIX_FORMAT = '%07d-%05d.ubdaq'
    STAGE_AREA = '/pnfs/uboone/scratch/uboonepro/dropbox/data/uboone/raw/'

    k=ds_reader()
    samweb = samweb_cli.SAMWebClient(experiment='uboone')

    last_run = k.get_last_run(MAIN_RUN_TABLE)

    run_to_file_map = {}

    for x in xrange(last_run):

        run = last_run - x

        virtual_flist = list(samweb.listFiles(SAM_LIST_FORMAT_STAGED % run))

        flist = []
        for f in virtual_flist:
            fname = '%s/%s' % (STAGE_AREA,f)
            if not os.path.isfile(fname): continue
            if time.time() - os.path.getmtime(fname) < 30: continue
            flist.append(fname)
            if len(flist) > NRUNS: break

        res = DAQFileListToRunMap(flist)

        run_to_file_map.update(res)

        if len(run_to_file_map) > NRUNS: break

        physical_flist = list(samweb.listFiles(SAM_LIST_FORMAT_STORED % run))

        flist = []

        for f in physical_flist:
            fname = f
            try:
                loc = samweb.locateFile(fname)
                loc = loc[0]
                if 'full_path' in loc:
                    loc = loc['full_path']
                    loc = loc[loc.find('/'):len(loc)]
                    fname = '%s/%s' % (loc,fname)
                else: continue
            except Exception:
                continue
            flist.append(fname)
            if len(flist) + len(run_to_file_map) > NRUNS: break

        res = DAQFileListToRunMap(flist)

        run_to_file_map.update(res)

        if len(run_to_file_map) > NRUNS: break

    runid_v = run_to_file_map.keys()
    runid_v.sort()
    runid_v.reverse()

    result_flist = {}
    ctr = 0
    for runid in runid_v:
        result_flist[runid] = run_to_file_map[runid]
        #print fname,loc
    print
    return result_flist

if __name__ == '__main__':

    nruns = 100
    if len(sys.argv)>1:
        nruns = int(sys.argv[1])

    fmap = GetAvailableFileList(nruns)

    runid_v = fmap.keys()
    runid_v.sort()
    runid_v.reverse()

    # Show latest => oldest runs
    for runid in runid_v:
        print runid, fmap[runid]

