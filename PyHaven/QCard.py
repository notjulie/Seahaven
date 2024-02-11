import typing

from PyQt5.QtCore import QRectF
from PyQt5.QtGui import QPainterPath, QBrush, QColor
from PyQt5.QtWidgets import QGraphicsPathItem, QGraphicsItemGroup, QGraphicsTextItem


class QCard(QGraphicsItemGroup):
    """ represents a card on the screen """

    # class variables
    _background = QBrush(QColor(255, 0, 0))

    def __init__(self):
        """ initializer """
        QGraphicsItemGroup.__init__(self)

        # add our rounded rectangle background
        self.painterPath = QPainterPath()
        self.painterPath.addRoundedRect(QRectF(0, 0, 50, 50), 5, 5)
        self.path = QGraphicsPathItem(self.painterPath)
        self.path.setBrush(self._background)
        self.addToGroup(self.path)

        # add our text item
        self.addToGroup(QGraphicsTextItem("bleem"))

        # initialize our event handlers
        self.cardMousePress = lambda *args: None
        self.cardMouseMove = lambda *args: None
        self.cardMouseRelease = lambda *args: None

    def mousePressEvent(self, event: typing.Optional['QGraphicsSceneMouseEvent']):
        """ mouse press event; dispatches to our cardMousePress handler """
        self.cardMousePress(event)

    def mouseMoveEvent(self, event: typing.Optional['QGraphicsSceneMouseEvent']):
        """ mouse press event; dispatches to our cardMouseMove handler """
        self.cardMouseMove(event)

    def mouseReleaseEvent(self, event: typing.Optional['QGraphicsSceneMouseEvent']):
        """ mouse press event; dispatches to our cardMouseRelease handler """
        self.cardMouseRelease(event)
