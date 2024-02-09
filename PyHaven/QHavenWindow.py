
from PyQt5.QtWidgets import QApplication, QWidget

class QHavenWindow(QWidget):
    def __init__(self):
        QWidget.__init__(self)
        self.resize(320, 240)  # The resize() method resizes the widget.
        self.setWindowTitle("Hello, Couch!")  # Here we set the title for our window.



