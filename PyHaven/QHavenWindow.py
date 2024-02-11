
from PyQt5.QtWidgets import QGraphicsView, QGraphicsScene, QGraphicsSceneMouseEvent

from PyHavenGame import PyHavenGame
from QCard import QCard


class QHavenWindow(QGraphicsView):
    """ PyHaven main window """

    def __init__(self):
        """ initializer """
        QGraphicsView.__init__(self)

        self._game = None
        self._scene = QGraphicsScene(0, 0, 100, 100)
        self.setScene(self._scene)
        self._card = QCard()
        self._scene.addItem(self._card)

        self._card.cardMousePress = lambda event: self._card_mouse_press(self._card, event)
        self._card.cardMouseMove = lambda event: self._card_mouse_move(self._card, event)
        self._card.cardMouseRelease = lambda event: self._card_mouse_release(self._card, event)

        self.resize(320, 240)  # The resize() method resizes the widget.
        self.setWindowTitle("Hello, Couch!")  # Here we set the title for our window.

    def new_game(self):
        self._game = PyHavenGame()
        self._game.new_game()

    def _card_mouse_press(self, card, event: QGraphicsSceneMouseEvent):
        """ handler for mouse press on card objects """
        self._cardDragStartX = card.x()
        self._cardDragStartY = card.y()
        self._cardInMotion = card
        self._cardDragClickX = event.screenPos().x()
        self._cardDragClickY = event.screenPos().y()

    def _card_mouse_move(self, card, event: QGraphicsSceneMouseEvent):
        """ handler for mouse press on card objects """
        if self._cardInMotion is not None:
            if self._cardInMotion is card:
                card.setX(self._cardDragStartX + event.screenPos().x() - self._cardDragClickX)
                card.setY(self._cardDragStartY + event.screenPos().y() - self._cardDragClickY)

    def _card_mouse_release(self, _card, _event):
        """ handler for mouse press on card objects """
        self._cardInMotion = None
