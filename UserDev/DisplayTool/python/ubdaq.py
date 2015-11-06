#!/usr/bin/env python

import argparse
import sys
from PyQt4 import QtGui, QtCore
import evdmanager
import gui
import signal


def sigintHandler(*args):
    """Handler for the SIGINT signal."""
    sys.stderr.write('\r')
    sys.exit()

def main():

    parser = argparse.ArgumentParser(description='Python based event display.')
    parser.add_argument('file', nargs='*', help="Optional input file to use")

    args = parser.parse_args()

    app = QtGui.QApplication(sys.argv)


    geom = evdmanager.microboone()

    file = None    
    if len(args.file) > 0:
        file = args.file[0]
    

    # If a file was passed, give it to the manager:
    manager = evdmanager.ubdaq_manager(geom)
    manager.setInputFile(file)

    thisgui = gui.livegui(geom,manager)
    thisgui.initUI()

    # Start looking for new files if the is not a new file:
    if file is None:
        thisgui.fileUpdateButtonHandler()

    # manager.goToEvent(0)

    signal.signal(signal.SIGINT, sigintHandler)
    timer = QtCore.QTimer()
    timer.start(500)  # You may change this if you wish.
    timer.timeout.connect(lambda: None)  # Let the interpreter run each 500 ms.

    app.exec_()
    # sys.exit(app.exec_())

if __name__ == '__main__':
    main()
