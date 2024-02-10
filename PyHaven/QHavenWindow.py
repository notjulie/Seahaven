
from PyQt5.QtWidgets import QWidget, QGraphicsView, QGraphicsScene, QGraphicsTextItem
from PyQt5.QtCore import QTimer
from QCard import QCard

class QHavenWindow(QGraphicsView):

    def __init__(self):
        QGraphicsView.__init__(self)

        self.scene = QGraphicsScene(0, 0, 100, 100)
        self.setScene(self.scene)
        self.card = QCard()
        self.card.cardMousePress = lambda event : self.bleem(self.card, event)
        self.scene.addItem(self.card)

        self.resize(320, 240)  # The resize() method resizes the widget.
        self.setWindowTitle("Hello, Couch!")  # Here we set the title for our window.

        #self.timer = QTimer(self)
        #self.timer.timeout.connect(self.processOneThing)
        #self.timer.start(100)

    def bleem(self, card, event):
        card.setX(card.x() + 1)



    #def processOneThing(self):
        #self.card.setX(self.card.x() + 1)

