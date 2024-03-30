
from typing import Dict
from PyQt5.QtWidgets import QGraphicsView, QGraphicsScene, QGraphicsSceneMouseEvent

from PyHavenCard import PyHavenCard
from PyHavenGame import PyHavenGame
from QCard import QCard


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

        # set our size and title
        self.resize(320, 240)  # The resize() method resizes the widget.
        self.setWindowTitle("Hello, Couch!")  # Here we set the title for our window.

    # endregion

    # region Public Methods

    def new_game(self):
        """ shuffles and deals a new game """
        self._game = PyHavenGame()
        self._game.new_game()
        self._draw_game()

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

    # region Private Methods

    def _draw_game(self):
        """ redraws the current game """

        # start with a dumbed down version of drawing the towers
        for tower_index in range(4):
            tower_card_id = self._game.get_tower_card(tower_index)
            if tower_card_id is not None:
                screen_card = self._get_card(tower_card_id)
                screen_card.setX(20 * tower_index)
                screen_card.setY(20)
                screen_card.setVisible(True)

    def _get_card(self, card_id: PyHavenCard) -> QCard:
        """ gets the card associated with the given ID; creates it if needed """
        result = self._cardDictionary.get(card_id)
        if result is None:
            self._create_card(card_id)
            result = self._cardDictionary[card_id]
        return result

    def _create_card(self, card_id: PyHavenCard):
        """ creates a QCard instance for the given card_id """
        card = QCard(card_id)

        # hook in our event handlers
        card.mousePressEvent = lambda event: self._card_mouse_press(card, event)
        card.mouseMoveEvent = lambda event: self._card_mouse_move(card, event)
        card.mouseReleaseEvent = lambda event: self._card_mouse_release(card, event)

        # hide it before we add it to our scene
        card.setVisible(False)
        self._scene.addItem(card)

        # and drop it into our card dictionary
        self._cardDictionary[card_id] = card

    # endregion

