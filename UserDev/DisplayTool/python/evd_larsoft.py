#!/usr/bin/env python
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

from gui import larsoftgui
import argparse
import sys
import signal
from pyqtgraph.Qt import QtGui, QtCore

from ROOT import evd

from evdmanager import geometry, larsoft_manager


def sigintHandler(*args):
    """Handler for the SIGINT signal."""
    sys.stderr.write('\r')
    sys.exit()


def main():

    parser = argparse.ArgumentParser(description='Python based event display for drawing larsoft products.')
    parser.add_argument('file', nargs='?', help="Optional input file to use")
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
    args = parser.parse_args()

    app = QtGui.QApplication(sys.argv)

    if args.argoneut:
        geom = geometry.argoneut()
    elif args.lariat:
        geom = geometry.lariat()
    else:
        geom = geometry.microboone()

    # If a file was passed, give it to the manager:

    manager = larsoft_manager(geom)
    manager.setInputFile(args.file)


    thisgui = larsoftgui(geom, manager)
    thisgui.initUI()
    thisgui.update()
    # manager.goToEvent(0)

    # thisgui.setWindowState(window.windowState() & ~QtCore.Qt.WindowMinimized | QtCore.Qt.WindowActive)
    thisgui.raise_()
    thisgui.activateWindow()

    signal.signal(signal.SIGINT, sigintHandler)
    timer = QtCore.QTimer()
    timer.start(500)  # You may change this if you wish.
    timer.timeout.connect(lambda: None)  # Let the interpreter run each 500 ms.

    app.exec_()
    # sys.exit(app.exec_())


if __name__ == '__main__':
    main()
