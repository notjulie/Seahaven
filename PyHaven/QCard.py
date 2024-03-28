import typing

from PyQt5 import QtCore
from PyQt5.QtCore import QRectF, Qt
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

        # add our rounded rectangle background
        # self.painterPath = QPainterPath()
        # self.painterPath.addRoundedRect(QRectF(0, 0, 50, 50), 5, 5)
        # self.path = QGraphicsPathItem(self.painterPath)
        # self.path.setBrush(self._background)
        # self.addToGroup(self.path)
        # self.setOpacity(1.0)
        # self.setScale(10)

        self._font = QFont()
        self._font.setPixelSize(180)

        # add our text item
        # self._textItem = QGraphicsTextItem(chr(0x0001F0B1 + 5))
        # self._textItem.setScale(10)
        # self._textItem.setFont(self._font)
        # self._textItem.setOpacity(1.0)
        # self._textItem.setX(0)
        # self._textItem.setY(0)
        # self.addToGroup(self._textItem)

        # self.setScale(10)
        # self.setDefaultTextColor(QColor(255, 0, 0))
        # self.addToGroup(self._textItem)
        # self.setHtml("<div style='background:rgba(255, 255, 255, 100%);'>" + chr(0x0001F0B1 + 5) + "</div>")

        # initialize our event handlers
        self.cardMousePress = lambda *args: None
        self.cardMouseMove = lambda *args: None
        self.cardMouseRelease = lambda *args: None

    def boundingRect(self) -> QRectF:
        r = QFontMetrics(self._font).tightBoundingRect(chr(0x0001F0B1 + 5))
        return QRectF(0, 0, r.width(), r.height())
        # size = QFontMetrics(self._font).size(0, chr(0x0001F0B1 + 5))
        # return QRectF(0, 0, size.width(), size.height())
        # return QRectF(0, 0, 50, 50)

    def paint(self, painter: typing.Optional[QPainter], option: typing.Optional['QStyleOptionGraphicsItem'], widget: typing.Optional[QWidget] = ...) -> None:
        painter.setClipRect(self.boundingRect())
        # painter.setBrush(self._background)
        painter.fillRect(self.boundingRect(), self._background)
        painter.setFont(self._font)
        r = self.boundingRect()
        bigR = QFontMetrics(self._font).boundingRect(chr(0x0001F0B1 + 5))
        painter.drawText(r.left(), r.top() - (bigR.height() - r.height()), r.width(), r.height(), Qt.AlignHCenter | Qt.AlignVCenter | Qt.TextDontClip, chr(0x0001F0B1 + 5))

    def mousePressEvent(self, event: typing.Optional['QGraphicsSceneMouseEvent']):
        """ mouse press event; dispatches to our cardMousePress handler """
        self.cardMousePress(event)

    def mouseMoveEvent(self, event: typing.Optional['QGraphicsSceneMouseEvent']):
        """ mouse press event; dispatches to our cardMouseMove handler """
        self.cardMouseMove(event)

    def mouseReleaseEvent(self, event: typing.Optional['QGraphicsSceneMouseEvent']):
        """ mouse press event; dispatches to our cardMouseRelease handler """
        self.cardMouseRelease(event)
