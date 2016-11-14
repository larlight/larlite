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

hitcheck = fmwk.CheckHitAlignment()
hitcheck.setHitProducer('gaushit')

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


ADC2e_Y = []
ADC2e_V = []
ADC2e_U = []

#offset = 7150
offset = 7298

while ( my_proc.process_event() ):
    
    # loop through all channels
    for chan in xrange(5000,8256):

        # make sure there is information stored at this channel
        if (hitcheck.hasHits(chan) == False):
            continue
            
        hits = hitcheck.getHits(chan)
        ides = hitcheck.getIDEs(chan)

        #print 'channel : %i'%chan
        print 'there are %i hits and %i ides'%(hits.size(), ides.size())
        
        # skip channels with > 1 hit
        #if (hits.size() > 1):
        #    print 'skipping event w/ more than 1 hit...'
        #    continue

        if (len(hits) != 1):
            continue
        
        for hit in hits:
            
            maxhittick = 0

            hitIntegral = 0.
            hitPlane = 0
            
            hitPlane = hit.WireID().Plane
            #print 'Hit plane : ',hitPlane
            #print 'hit time : %i'%hit.PeakTime()
            maxhittick = hit.PeakTime()
            times,adcs = gauss(hit.PeakTime(),hit.RMS())
            hitIntegral = hit.Integral()
            #print 'Hit integral : %.02f'%hitIntegral
            #plt.plot(times,adcs,'ro')

            qtot = 0
            qhit = 0 # charge in simch ides that is at the same time as the hit time
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
                if ( ( (ide.first-offset) > (hit.PeakTime() - 3*hit.RMS()) ) and ( (ide.first-offset) < (hit.PeakTime() + 3*hit.RMS()) ) ):
                    qhit += ide.second
            

            #offset = maxtick - maxhittick
            #print 'offset (ticks) is ',offset

            ideTime   = np.array(ideTime)-offset
            
            if (qtot == 0):
                continue
        
            #print 'IDE Q = %.02f'%(qtot) 
            #ideCharge = np.array(ideCharge)/float(qtot)
            #print 'Hit Q / # e- = %.02f 10^-3'%(1000*hitIntegral/qtot)

            print len(ADC2e_U)
            
            if (hitPlane == 0):
                ADC2e_U.append(hitIntegral/qtot)
            if (hitPlane == 1):
                ADC2e_V.append(hitIntegral/qtot)
            if (hitPlane == 2):
                ADC2e_Y.append(hitIntegral/qtot)
        
            #plt.plot(ideTime,ideCharge,'bo')

            #plt.grid()
            #plt.xlabel('Tick Number [Hit Scale]')
            #plt.ylabel('Relative Amplitude')
            
            #fig.canvas
            #fig.canvas.draw()
            
            #usrinput = raw_input("Hit Enter: next channel  || int: go to channel number ||  q: exit viewer\n")                        
            #if ( usrinput == "q" ):                                                          
            #    sys.exit(0)
            #elif ( usrinput.isdigit() == True):
            #    chan = int(usrinput)

fig = plt.figure(figsize=(8,8))
plt.hist(1000.*np.array(ADC2e_Y),bins=np.linspace(0,10,100),histtype='stepfilled')
plt.grid()
plt.xlabel('ADC / $e^-$ [$10^3$]')
plt.title('Y Plane')
plt.show()

fig = plt.figure(figsize=(8,8))
plt.hist(1000*np.array(ADC2e_V),bins=np.linspace(3,7,100),histtype='stepfilled')
plt.grid()
plt.title('V Plane')
plt.xlabel('ADC / $e^-$ [$10^3$]')
plt.show()

fig = plt.figure(figsize=(8,8))
plt.hist(1000*np.array(ADC2e_U),bins=np.linspace(3,7,100),histtype='stepfilled')
plt.grid()
plt.title('U Plane')
plt.xlabel('ADC / $e^-$ [$10^3$]')
plt.show()
        
sys.exit()

