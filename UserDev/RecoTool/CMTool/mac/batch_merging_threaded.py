import sys, os
import threading
from ROOT import TFile

if len(sys.argv) != 2:
    print 'Usage: python %s batchname'%sys.argv[0]
    quit()

max_files_to_process = 50

numberOfWorkers = 10

batchname = sys.argv[1]
batchnamepath = '/Users/davidkaleko/Data/larlite/mcc6/larlite_reco/'+batchname+'/'
run_script = '/Users/davidkaleko/larlite/UserDev/RecoTool/CMTool/mac/run_merger.py'

#files is a list of (good) files (full path) to process
files = []

#build the list
#also save a txt file listing the input files that were used in this run
txtfile = open('%s_inputfiles.txt'%batchname,'w')
for folder in os.listdir(batchnamepath):
    myfile = folder + '/larlite_reco2d.root'
    #check if a file is a zombie
    f = TFile(batchnamepath+myfile,'READ')
    if not f.IsZombie():
        files.append(batchnamepath+myfile)
        max_files_to_process -= 1
    f.Close()
    txtfile.write(batchname+'/'+myfile+'\n')
    if not max_files_to_process:
        break

txtfile.close()

#Now we can do the threading stuff 
class Worker(threading.Thread):
    requestID = 0
    def __init__(self, requestsQueue, resultsQueue, **kwds):
        threading.Thread.__init__(self,**kwds)
        self.setDaemon(1)
        self.workRequestQueue = requestsQueue
        self.resultQueue = resultsQueue
        self.start()
    def performWork(self, callable, *args, **kwds):
        "called by the main thread as callable would be, but w/o return"
        #This function has the worker put the request into the requests queue
        Worker.requestID += 1
        self.workRequestQueue.put((Worker.requestID, callable, args, kwds))
        return Worker.requestID
    def run(self):
        #This function keeps looping and getting things off of the
        #request queue... then, it executes the request (callable)
        #and puts the result into the result queue
        while 1:
            requestID, callable, args, kwds = self.workRequestQueue.get()
            print "Worker that is executing request %d is: %s" \
                %(requestID,self.getName())
            self.resultQueue.put((requestID, callable(*args, **kwds)))

##The main thread creates the two queues, then instantiates worker
##threads as follows:
import Queue
requestsQueue = Queue.Queue()
resultsQueue = Queue.Queue()
for i in range(numberOfWorkers):
    worker = Worker(requestsQueue, resultsQueue)
    print worker

##Simulate work by evaluating random expressions, with random delays,
##on several worker threads.
import time,subprocess
def evaluate(commandString):
    #print "called: evaluate(%s)"%commandString
    return subprocess.check_output(commandString, shell=True)

workRequests = {}
def showResults():
    while 1:
        try: id, results = resultsQueue.get_nowait()
        except Queue.Empty: return
        print 'Result %d: %s has is successfully completed.' % (id, workRequests[id])
        del workRequests[id]

counter = 0
for myfile in files:
    commandString = 'python %s %s %s_merged_%d.root'%(run_script,myfile,batchname,counter)
    counter += 1

    #the last created worker is always being called here
    #which means thie last created worker is the only one putting
    #stuff into the work request queue
    id = worker.performWork(evaluate, commandString)
    workRequests[id] = commandString
    print 'Submitted request %d.\n' % id
    time.sleep(1)
    showResults()
while workRequests:
    time.sleep(1)
    showResults()
