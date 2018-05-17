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
for x in xrange(len(sys.argv)-1):
    fname = sys.argv[x+1]
    my_proc.add_input_file(fname)
    
# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify analysis output root file name
my_proc.set_ana_output_file("");

# Specify data output root file name
my_proc.set_output_file("larlite_getgains.root")

offset = 7298

NSIGMA = 3


hitcheck = fmwk.CheckHitAlignment()
hitcheck.setHitProducer('gaushit')
hitcheck.setOffset(offset)
hitcheck.setNSigma(NSIGMA)
my_proc.add_process(hitcheck)

print
print  "Finished configuring ana_processor. Start event loop!"
print

import matplotlib.pyplot as plt
import numpy as np
#plt.ion()

plt.rcParams.update({'font.size': 16})

def gauss(mu,sigma):
    timeticks = []
    amps = []
    numpoints = 200
    for i in xrange(numpoints):
        t = (mu-2*sigma) + (2*sigma) * (float(i)/ (numpoints/2.) )
        timeticks.append(t)
        adc = 1./np.sqrt( 2 * np.pi * sigma*sigma ) * np.exp ( - ( (t-mu) / sigma )**2 * 0.5 )
        amps.append( adc )
    return timeticks,amps

#fig, ax = plt.subplots(1,1,figsize=(20,8))


wire_Y  = []
ADC2e_Y = []
ADC2e_V = []
ADC2e_U = []

#offset = 7150


XMIN = 170
XMAX = 230
NBINS = 100

while ( my_proc.process_event() ):
#for n in xrange( 3 ):

    #print 'event %i'%n

    #my_proc.process_event()
    
    # loop through all channels
    for chan in xrange(0,8256):

        # make sure there is information stored at this channel
        if (hitcheck.hasHits(chan) == False):
            continue
            
        hits = hitcheck.getHits(chan)
        ides = hitcheck.getIDEs(chan)

        #print 'channel : %i'%chan
        #print 'there are %i hits and %i ides'%(hits.size(), ides.size())
        
        # skip channels with > 1 hit
        #if (hits.size() > 1):
        #   print 'skipping event w/ more than 1 hit...'
        #    continue

        if (len(hits) != 1):
            continue
        
        for hit in hits:
            
            maxhittick = 0

            hitIntegral = 0.
            hitPlane = 0
            
            hitPlane = hit.WireID().Plane

            #if ( (hitPlane == 2) and (len(ADC2e_Y) > 3000)): continue
            #if ( (hitPlane == 1) and (len(ADC2e_V) > 1000)): continue
            #if ( (hitPlane == 0) and (len(ADC2e_U) > 1000)): continue

            #if (hitPlane != 2): continue
            
            #print 'Hit plane : ',hitPlane
            #print 'hit time : %i'%hit.PeakTime()
            maxhittick = hit.PeakTime()
            hitIntegral = hit.Integral()

            if (hitIntegral < 20): continue
            
            #print 'Hit integral : %.02f'%hitIntegral

            qtot = 0
            qhit = 0 # charge in simch ides that is at the same time as the hit time
            ideCharge = []
            for ide in ides:
                qtot += ide.second
                if ( ( (ide.first-offset) > (hit.PeakTime() - NSIGMA * hit.RMS()) ) and ( (ide.first-offset) < (hit.PeakTime() + NSIGMA * hit.RMS()) ) ):
                    qhit += ide.second
            

            #offset = maxtick - maxhittick
            #print 'offset (ticks) is ',offset

            if (qhit == 0):
                continue

            if (hitPlane == 0):
                ADC2e_U.append(qhit/hitIntegral)
            if (hitPlane == 1):
                ADC2e_V.append(qhit/hitIntegral)
            if (hitPlane == 2):
                ADC2e_Y.append(qhit/hitIntegral)
                wire_Y.append( hit.Channel() )

ADC2e_Y = np.array(ADC2e_Y)
ADC2e_V = np.array(ADC2e_V)
ADC2e_U = np.array(ADC2e_U)

#print ADC2e_Y

fig = plt.figure(figsize=(8,8))
plt.hist(ADC2e_Y,bins=np.linspace(XMIN,XMAX,NBINS),histtype='stepfilled')
plt.grid()
plt.xlabel('collected $e^-$ / ADC',fontsize=20,fontweight='bold')
plt.xlim([XMIN,XMAX])
plt.title('Y Plane',fontsize=20,fontweight='bold')
plt.show()

fig = plt.figure(figsize=(8,8))
plt.hist(1000./np.array(ADC2e_Y),bins=np.linspace(1000./XMAX,1000./XMIN,NBINS),histtype='stepfilled')
plt.grid()
plt.xlabel('ADC / 1000. $e^-$',fontsize=20,fontweight='bold')
plt.xlim([1000./XMAX,1000./XMIN])
plt.title('Y Plane',fontsize=20,fontweight='bold')
plt.show()

fig = plt.figure(figsize=(8,8))
plt.hist2d(wire_Y,1000./np.array(ADC2e_Y),bins=(np.linspace(4800,8000,100),np.linspace(1000./XMAX,1000./XMIN,NBINS)))#,histtype='stepfilled')
#plt.grid()
plt.xlabel('Channel Number')
plt.ylabel('ADC / 1000. $e^-$',fontsize=20,fontweight='bold')
#plt.xlim([1000./XMAX,1000./XMIN])
plt.ylim([1000./XMAX,1000./XMIN])
plt.title('Y Plane',fontsize=20,fontweight='bold')
plt.show()

#sys.exit()

fig = plt.figure(figsize=(8,8))
plt.hist(ADC2e_V,bins=np.linspace(XMIN,XMAX,NBINS),histtype='stepfilled')
plt.grid()
plt.title('V Plane',fontsize=20,fontweight='bold')
plt.xlabel('$e^-$ / ADC',fontsize=20,fontweight='bold')
plt.xlim([XMIN,XMAX])
plt.show()

fig = plt.figure(figsize=(8,8))
plt.hist(ADC2e_U,bins=np.linspace(XMIN,XMAX,NBINS),histtype='stepfilled')
plt.grid()
plt.title('U Plane',fontsize=20,fontweight='bold')
plt.xlabel('$e^-$ / ADC',fontsize=20,fontweight='bold')
plt.xlim([XMIN,XMAX])
plt.show()
        
sys.exit()

