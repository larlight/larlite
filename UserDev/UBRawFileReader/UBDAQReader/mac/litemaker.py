from ROOT import ubdaq

k=ubdaq.LiteStorageManager()
k.StoreLArLiteFile("out.root")
k.AddInputFile("/data/kterao/PhysicsRun-2015_10_15_5_45_18-0003414-02382.ubdaq")
#k.AddInputFile("/datalocal/uboonepro/rawdata/NoiseRun-0002026-00001.ubdaq")
#k.ReadRawDigit(False)
k.ReadRawDigit(True)
k.Initialize()

while k.ProcessEvent():

    print(k.FIFO())

k.Reset()
