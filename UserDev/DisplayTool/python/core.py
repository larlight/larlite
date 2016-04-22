#!/usr/bin/env python
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
import argparse
import sys
from pyqtgraph.Qt import QtGui, QtCore
import evdmanager
import gui
import signal


def sigintHandler(*args):
    """Handler for the SIGINT signal."""
    sys.stderr.write('\r')
    sys.exit()

def main():

    parser = argparse.ArgumentParser(description='Python based event display core, doesn\'t draw data.')
    geom = parser.add_mutually_exclusive_group()
    geom.add_argument('-A', '-a', '--argoneut',
                      action='store_true',
                      help="Run with the argoneut geometry")
    geom.add_argument('-U', '-u', '--uboone',
                      action='store_true',
                      help="Run with the microboone geometry")
    geom.add_argument('-L', '-l', '--lariat',
                      action='store_true',
                      help="Run with the lariat geometry")
    parser.add_argument('file', nargs='*', help="Optional input file to use")

    args = parser.parse_args()

    app = QtGui.QApplication(sys.argv)

    if args.uboone:
        geom = evdmanager.microboone()
    elif args.lariat:
        geom = evdmanager.lariat()
    else:
        geom = evdmanager.argoneut()

    # If a file was passed, give it to the manager:


    thisgui = gui.gui(geom)
    thisgui.initManager()
    thisgui.initUI()

    # manager = larlite_manager(geom)
    # manager.setInputFiles(args.file)


    # manager.goToEvent(0)

    signal.signal(signal.SIGINT, sigintHandler)
    timer = QtCore.QTimer()
    timer.start(500)  # You may change this if you wish.
    timer.timeout.connect(lambda: None)  # Let the interpreter run each 500 ms.

    app.exec_()
    # sys.exit(app.exec_())

if __name__ == '__main__':
    main()