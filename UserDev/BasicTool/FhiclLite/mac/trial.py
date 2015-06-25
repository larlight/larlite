from ROOT import *
import sys
#k=CfgFileReader()
k=fclite.ConfigManager()

k.AddCfgFile(sys.argv[1])

d=k.Config()

keys = k.SubConfig("a").value_keys()
for x in xrange(keys.size()):
    print keys[x]

a=d.get_pset("a")
print a.get("g")
