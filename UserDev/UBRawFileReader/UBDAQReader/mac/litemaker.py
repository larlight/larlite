from ROOT import ubdaq

k=ubdaq.LiteStorageManager()
k.StoreLArLiteFile("out.root")
k.AddInputFile("/datalocal/uboonepro/rawdata/NoiseRun-0002026-00000.ubdaq")
k.AddInputFile("/datalocal/uboonepro/rawdata/NoiseRun-0002026-00001.ubdaq")
k.ReadRawDigit(False)
k.Initialize()

while k.ProcessEvent():

    print k.FIFO()

k.Reset()
