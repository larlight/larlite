from __future__ import print_function
import os,sys
cmd = ''
if not len(sys.argv) == 2 or not 'LARLITE_BASEDIR' in os.environ or not sys.argv[1] in os.environ:
    pass
else:
    lite = os.environ['LARLITE_BASEDIR']
    e = sys.argv[1]
    v = os.environ[e]
    start = v.find(lite)
    if start >= 0:
        end = v.find(':',start)

        replace = ''
        if end >=0:
            replace = v[start:end+1]
        else:
            replace = v[start:len(v)]

        cmd = 'export %s=%s' % (e,v.replace(replace,''))
        while cmd.find('::') >= 0:
            cmd = cmd.replace('::',':')
print(cmd)
