
from PyQt5.QtWidgets import QWidget
from PyQt5.QtCore import QTimer

class QHavenWindow(QWidget):
    def __init__(self):
        QWidget.__init__(self)
        self.resize(320, 240)  # The resize() method resizes the widget.
        self.setWindowTitle("Hello, Couch!")  # Here we set the title for our window.

        self.timer = QTimer(self)
        self.timer.timeout.connect(self.processOneThing)
        self.timer.start(100)


    def processOneThing(self):
        self.setWindowTitle("Hello, table!")  # Here we set the title for our window.

