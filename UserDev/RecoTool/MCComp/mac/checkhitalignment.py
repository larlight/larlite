import sys
if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)


from larlite import larlite as fmwk

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in range(len(sys.argv)-1):
    fname = sys.argv[x+1]
    my_proc.add_input_file(fname)
    
# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify analysis output root file name
my_proc.set_ana_output_file("");

# Specify data output root file name
my_proc.set_output_file("larlite_mcclusters.root")

hitcheck = fmwk.CheckHitAlignment()
hitcheck.setHitProducer('gaushit')

my_proc.add_process(hitcheck)

print()
print("Finished configuring ana_processor. Start event loop!")
print()

import matplotlib.pyplot as plt
import numpy as np
plt.ion()

def gauss(mu,sigma):
    timeticks = []
    amps = []
    numpoints = 200
    for i in range(numpoints):
        t = (mu-2*sigma) + (2*sigma) * (float(i)/ (numpoints/2.) )
        timeticks.append(t)
        adc = 1./np.sqrt( 2 * np.pi * sigma*sigma ) * np.exp ( - ( (t-mu) / sigma )**2 * 0.5 )
        amps.append( adc )
    return timeticks,amps

fig, ax = plt.subplots(1,1,figsize=(20,8))

while ( my_proc.process_event() ):
    
    # loop through all channels
    for chan in range(4800,8256):
        
        # make sure there is information stored at this channel
        if (hitcheck.hasHits(chan) == False):
            continue
            
        ax.cla()
        fig.gca()
        
        hits = hitcheck.getHits(chan)
        ides = hitcheck.getIDEs(chan)

        print('channel : %i'%chan)
        print('there are %i hits and %i ides'%(hits.size(), ides.size()))
        
        # skip channels with > 1 hit
        if (hits.size() > 1):
            print('skipping event w/ more than 1 hit...')
            continue

        maxhittick = 0

        hitIntegral = 0.
        
        for hit in hits:
            print('hit time : %i'%hit.PeakTime())
            maxhittick = hit.PeakTime()
            times,adcs = gauss(hit.PeakTime(),hit.RMS())
            hitIntegral = hit.Integral()
            print('Hit integral : %.02f'%hitIntegral)
            plt.plot(times,adcs,'ro',label='Hit Gauss Fit')
            plt.axvspan(hit.PeakTime()-3*hit.RMS(),hit.PeakTime()+3*hit.RMS(),color='r',alpha=0.5,label='Simch integration Region')
            plt.xlim([hit.PeakTime()-10*hit.RMS(),hit.PeakTime()+10*hit.RMS()])

        qtot = 0
        ideTime   = []
        ideCharge = []
        maxtick   = 0
        maxq      = 0
        for ide in ides:
            #print 'ide time : %i -> charge : %.02f'%(ide.first, ide.second)
            ideTime.append(ide.first)
            ideCharge.append(ide.second)
            if (ide.second > maxq):
                maxq    = ide.second
                maxtick = ide.first
            qtot += ide.second

        offset = maxtick - maxhittick
        print('offset (ticks) is ',offset)

        ideTime   = np.array(ideTime)-offset
        
        print('IDE Q = %.02f'%(np.sum(ideCharge)))
        ideCharge = np.array(ideCharge)/float(qtot)
        if (qtot == 0):
            continue
        print('Hit Q / # e- = %.02f 10^3'%(1000*hitIntegral/qtot))

        
        plt.plot(ideTime,ideCharge,'bo',label='Simch IDEs')

        plt.grid()
        plt.legend(fontsize=16)
        plt.xlabel('Tick Number [Hit Scale]',fontsize=16)
        plt.ylabel('Relative Amplitude',fontsize=16)

        fig.canvas
        fig.canvas.draw()
        
        usrinput = input("Hit Enter: next channel  || int: go to channel number ||  q: exit viewer\n")                        
        if ( usrinput == "q" ):                                                          
            sys.exit(0)
        elif ( usrinput.isdigit() == True):
            chan = int(usrinput)

sys.exit()

