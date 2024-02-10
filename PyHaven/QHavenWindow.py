
from PyQt5.QtWidgets import QWidget, QGraphicsView, QGraphicsScene
from PyQt5.QtCore import QTimer

class QHavenWindow(QGraphicsView):

    def __init__(self):
        QGraphicsView.__init__(self)

        self.scene = QGraphicsScene(0, 0, 100, 100)
        self.setScene(self.scene)
        self.text = self.scene.addText("bleem")
        self.text.setX(10)
        self.text.setY(0)

        self.moreText = self.scene.addText("blerg")

        self.resize(320, 240)  # The resize() method resizes the widget.
        self.setWindowTitle("Hello, Couch!")  # Here we set the title for our window.

        self.timer = QTimer(self)
        self.timer.timeout.connect(self.processOneThing)
        self.timer.start(100)



    def processOneThing(self):
        #self.setWindowTitle("Hello, table!")  # Here we set the title for our window.
        self.text.setX(self.text.x() + 1)

