
from typing import Dict
from PyQt5.QtWidgets import QGraphicsView, QGraphicsScene, QGraphicsSceneMouseEvent

from PyHavenCard import PyHavenCard
from QCard import QCard
from QHavenView import QHavenView


class QHavenWindow(QGraphicsView):
    """ PyHaven main window """

    # region Constructor

    def __init__(self):
        """ initializer """

        # call the base class
        QGraphicsView.__init__(self)

        # clear
        self._game = None
        self._cardDictionary: Dict[PyHavenCard, QCard] = dict()

        # create our scene
        self._scene = QGraphicsScene(0, 0, 100, 100)
        self.setScene(self._scene)

        # add our QHavenView
        self._qhaven_view = QHavenView()
        self._scene.addItem(self._qhaven_view)

        # set our size and title
        self.resize(320, 240)  # The resize() method resizes the widget.
        self.setWindowTitle("PyHaven Towers")  # Here we set the title for our window.

    # endregion

    # region Public Methods

    def new_game(self):
        """ shuffles and deals a new game """
        self._qhaven_view.new_game()

    # endregion

    # region Event Handlers

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

    # endregion


