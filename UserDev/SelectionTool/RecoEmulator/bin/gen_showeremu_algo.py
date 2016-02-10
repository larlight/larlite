import os,sys

rawname=sys.argv[1]
if not rawname:
    print 'Name (1st arg.) cannot be empty...'
    sys.exit(1)
name='ShowerEmu' + rawname
print
print 'Generating a source code for new ShowerEmulator algorithm',name
source_path = os.path.dirname(__file__)
source_path = os.getcwd() + "/" + source_path + "/"
target_path = source_path + '../'

print 'Target path:',target_path
print 'Source path:',source_path
print

res_h='%s/%s.h' % (target_path,name)
res_cxx='%s/%s.cxx' % (target_path,name)
res_link='%s/LinkDef.h' % target_path
if os.path.isfile(res_h) or os.path.isfile(res_cxx):
    print 'Error: source code %s.h and/or %s.cxx already exist..' % (name,name)
    sys.exit(1)
if not os.path.isfile(res_link):
    print 'Error: LinkDef.h not found...'
    sys.exit(1)
    
tmp_cxx=open(source_path + '/shower_tmp.cxx','r').read()
tmp_h=open(source_path + '/shower_tmp.h','r').read()

tmp_cxx = tmp_cxx.replace('CLASSNAME',rawname.upper())
tmp_cxx = tmp_cxx.replace('ClassName',rawname)
tmp_h = tmp_h.replace('CLASSNAME',rawname.upper())
tmp_h = tmp_h.replace('ClassName',rawname)
tmp_h = tmp_h.replace('USER',os.environ['USER'])
fout = open(res_h,'w')
fout.write(tmp_h)
fout.close()

fout = open(res_cxx,'w')
fout.write(tmp_cxx)
fout.close()

tmp_linkdef = open(res_link,'r').read().split('\n')
linkdef = ''
for line in tmp_linkdef:
    if line.find('ADD_NEW_CLASS') >= 0:
        linkdef += '//#pragma link C++ class recoemu::%s+;\n' % name
    linkdef += line + '\n'
fout=open(res_link,'w')
fout.write(linkdef)
fout.close()

print 'Generated source files:'
print '  ',res_h
print '  ',res_cxx
print 'Modified:'
print '  ',res_link
print


