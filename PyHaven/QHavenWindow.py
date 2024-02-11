
from PyQt5.QtWidgets import QGraphicsView, QGraphicsScene, QGraphicsSceneMouseEvent
from QCard import QCard


class QHavenWindow(QGraphicsView):
    """ PyHaven main window """

    def __init__(self):
        """ initializer """
        QGraphicsView.__init__(self)

        self.scene = QGraphicsScene(0, 0, 100, 100)
        self.setScene(self.scene)
        self.card = QCard()
        self.scene.addItem(self.card)

        self.card.cardMousePress = lambda event: self._card_mouse_press(self.card, event)
        self.card.cardMouseMove = lambda event: self._card_mouse_move(self.card, event)
        self.card.cardMouseRelease = lambda event: self._card_mouse_release(self.card, event)

        self.resize(320, 240)  # The resize() method resizes the widget.
        self.setWindowTitle("Hello, Couch!")  # Here we set the title for our window.

    def _card_mouse_press(self, card, event: QGraphicsSceneMouseEvent):
        """ handler for mouse press on card objects """
        self.cardDragStartX = card.x()
        self.cardDragStartY = card.y()
        self.cardInMotion = card
        self.cardDragClickX = event.screenPos().x()
        self.cardDragClickY = event.screenPos().y()

    def _card_mouse_move(self, card, event: QGraphicsSceneMouseEvent):
        """ handler for mouse press on card objects """
        if self.cardInMotion is not None:
            if self.cardInMotion is card:
                card.setX(self.cardDragStartX + event.screenPos().x() - self.cardDragClickX)
                card.setY(self.cardDragStartY + event.screenPos().y() - self.cardDragClickY)

    def _card_mouse_release(self, _card, _event):
        """ handler for mouse press on card objects """
        self.cardInMotion = None
