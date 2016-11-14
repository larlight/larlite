#!/usr/bin/env python
import ROOT
from ROOT import larlite
ROOT.PyConfig.IgnoreCommandLineOptions = True


from gui import larlitegui
import argparse
import sys
import signal
from pyqtgraph.Qt import QtGui, QtCore


from ROOT import evd

from evdmanager import geometry, larlite_manager

# This is to allow key commands to work when focus is on a box




def sigintHandler(*args):
    """Handler for the SIGINT signal."""
    sys.stderr.write('\r')
    sys.exit()


def main():

    parser = argparse.ArgumentParser(description='Python based event display.')
    geom = parser.add_mutually_exclusive_group()
    geom.add_argument('-A', '-a', '--argoneut',
                      action='store_true',
                      help="Run with the argoneut geometry")
    geom.add_argument('-U', '-u', '--uboone',
                      action='store_true',
                      help="Run with the microboone geometry")
    geom.add_argument('-T', '-t', '--uboonetruncated',
                      action='store_true',
                      help="Run with the truncated microboone geometry")
    geom.add_argument('-L', '-l', '--lariat',
                      action='store_true',
                      help="Run with the lariat geometry")
    parser.add_argument('file', nargs='*', help="Optional input file to use")

    args = parser.parse_args()

    app = QtGui.QApplication(sys.argv)

    if args.uboone:
        geom = geometry.microboone()
    elif args.uboonetruncated:
        geom = geometry.microboonetruncated()
    elif args.lariat:
        geom = geometry.lariat()
    else:
        geom = geometry.argoneut()

    # If a file was passed, give it to the manager:

    manager = larlite_manager(geom)
    manager.setInputFiles(args.file)


    thisgui = larlitegui(geom, manager)
    # manager.goToEvent(0)

    signal.signal(signal.SIGINT, sigintHandler)
    timer = QtCore.QTimer()
    timer.start(500)  # You may change this if you wish.
    timer.timeout.connect(lambda: None)  # Let the interpreter run each 500 ms.

    app.exec_()
    # sys.exit(app.exec_())


if __name__ == '__main__':
    main()
