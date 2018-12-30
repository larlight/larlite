import os,sys

# check if LARLITE_BASEDIR is defined. If not, nothing to do.
if not 'LARLITE_BASEDIR' in os.environ:
    sys.exit(1)

# read in template GNUmakefile
makefile=open('%s/config/template/TopMakefile.tmp' % os.environ['LARLITE_BASEDIR'],'r').read()

fout=open('%s/GNUmakefile' % os.environ['LARLITE_BASEDIR'],'w')
fout.write(makefile)
fout.close()
