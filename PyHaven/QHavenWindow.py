
from PyQt5.QtWidgets import QGraphicsView, QGraphicsScene
from QCard import QCard


class QHavenWindow(QGraphicsView):
    """ PyHaven main window """

    def __init__(self):
        """ initializer """
        QGraphicsView.__init__(self)

        self.scene = QGraphicsScene(0, 0, 100, 100)
        self.setScene(self.scene)
        self.card = QCard()
        self.card.cardMousePress = lambda event: self._card_mouse_press(self.card, event)
        self.scene.addItem(self.card)

        self.resize(320, 240)  # The resize() method resizes the widget.
        self.setWindowTitle("Hello, Couch!")  # Here we set the title for our window.

    def _card_mouse_press(self, card, event):
        """ handler for mouse press on card objects """
        card.setX(card.x() + 1)
