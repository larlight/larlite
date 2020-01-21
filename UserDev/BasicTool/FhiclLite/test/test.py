import ROOT,os, sys
from ROOT import fcllite

def msg(s):
    print('\033[95m%s\033[00m' % s)

fs = open("aho.cfg","w")
fs.write('A:{\n A : 1\n B : 1\n}\n')
fs.close()

msg('Config file content')
os.system('cat aho.cfg')

msg('Touching PSet class')
fcllite.PSet
msg('Printing PSet class ptr')
print(str(fcllite.PSet))

msg('Touching CreatePSetFromFile')
fcllite.CreatePSetFromFile
msg('Printing CreatePsetFromFile function ptr')
print(fcllite.CreatePSetFromFile)

cmd = 'ps = fcllite.CreatePSetFromFile("aho.cfg","AHO")'
msg('Creating PSet by command "%s"' % cmd)
ps = None
exec(cmd)

msg('Printing created pset')
print(ps.dump())

msg('Exiting')
sys.exit(0)
