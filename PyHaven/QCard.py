from PyQt5.QtCore import QRectF
from PyQt5.QtGui import QPainterPath, QBrush, QColor
from PyQt5.QtWidgets import QWidget, QGraphicsView, QGraphicsPathItem, QGraphicsItemGroup, QGraphicsTextItem


# class QCard; our basic graphic card
class QCard(QGraphicsItemGroup):
    def __init__(self):
        QGraphicsItemGroup.__init__(self)

        self.painterPath = QPainterPath()
        self.painterPath.addRoundedRect(QRectF(0, 0, 50, 50), 5, 5)

        self.path = QGraphicsPathItem(self.painterPath)
        self.path.setBrush(QBrush(QColor(255, 0, 0)))
        self.addToGroup(self.path)

        self.addToGroup(QGraphicsTextItem("bleem"))
