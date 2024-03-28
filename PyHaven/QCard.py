import typing

from PyQt5 import QtCore
from PyQt5.QtCore import QRectF, Qt, QPointF
from PyQt5.QtGui import QPainterPath, QBrush, QColor, QFont, QPainter, QFontMetrics
from PyQt5.QtWidgets import QGraphicsItem, QGraphicsItemGroup, QGraphicsTextItem, QWidget

from PyHavenCard import PyHavenCard


class QCard(QGraphicsItem):
    """ represents a card on the screen """

    # class variables
    _background = QBrush(QColor(255, 0, 255))

    def __init__(self, card_id: PyHavenCard):
        """ initializer """

        # call the base class
        QGraphicsItem.__init__(self)

        # save parameters
        self._cardID = card_id

        self._font = QFont()
        self._font.setPixelSize(180)

        # initialize our event handlers
        self.cardMousePress = lambda *args: None
        self.cardMouseMove = lambda *args: None
        self.cardMouseRelease = lambda *args: None

    def boundingRect(self) -> QRectF:
        r = QFontMetrics(self._font).tightBoundingRect(chr(0x0001F0B1 + 5))
        return QRectF(0, 0, r.width(), r.height())

    def paint(self, painter: typing.Optional[QPainter], option: typing.Optional['QStyleOptionGraphicsItem'], widget: typing.Optional[QWidget] = ...) -> None:
        painter.setClipRect(self.boundingRect())
        painter.fillRect(self.boundingRect(), self._background)
        painter.setFont(self._font)

        r = QFontMetrics(self._font).tightBoundingRect(chr(0x0001F0B1 + 5))
        p = QPointF(-r.left(), -r.top())
        painter.drawText(p, chr(0x0001F0B1 + 5))

    def mousePressEvent(self, event: typing.Optional['QGraphicsSceneMouseEvent']):
        """ mouse press event; dispatches to our cardMousePress handler """
        self.cardMousePress(event)

    def mouseMoveEvent(self, event: typing.Optional['QGraphicsSceneMouseEvent']):
        """ mouse press event; dispatches to our cardMouseMove handler """
        self.cardMouseMove(event)

    def mouseReleaseEvent(self, event: typing.Optional['QGraphicsSceneMouseEvent']):
        """ mouse press event; dispatches to our cardMouseRelease handler """
        self.cardMouseRelease(event)
