""" class QCard

Implements a graphical card using the Unicode characters for
playing cards.

This program is free software: you can redistribute it and/or modify it
as you wish.  This program is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
"""

import typing

from PyQt5.QtCore import QRectF, QPointF
from PyQt5.QtGui import QBrush, QColor, QFont, QPainter, QFontMetrics
from PyQt5.QtWidgets import QGraphicsItem, QWidget
from PyHavenCard import PyHavenCard


class QCard(QGraphicsItem):
    """ represents a card on the screen """

    # class variables
    _background = QBrush(QColor(255, 255, 255))
    _unicodeSuitBases = [0x1F0D0, 0x1F0C0, 0x1F0B0, 0x1F0A0]
    _unicodeRankOffsets = [None, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14]

    def __init__(self, card_id: PyHavenCard):
        """ initializer """

        # call the base class
        QGraphicsItem.__init__(self)

        # save parameters
        self._cardID = card_id

        # initialize our event handlers
        self.cardMousePress = lambda *args: None
        self.cardMouseMove = lambda *args: None
        self.cardMouseRelease = lambda *args: None

        # figure out what character we draw and what color we draw it
        suit = self._cardID.get_suit()
        self._text = chr(QCard._unicodeSuitBases[suit] + QCard._unicodeRankOffsets[self._cardID.get_rank()])
        if suit > 0 & suit < 3:
            self._cardColor = QColor(255, 0, 0)
        else:
            self._cardColor = QColor(0, 0, 0)

        # set our font and measure our text based on it
        self._font = QFont()
        self._font.setPixelSize(180)
        metrics = QFontMetrics(self._font)
        r = metrics.tightBoundingRect(self._text)
        self._boundingRect = QRectF(0, 0, r.width(), r.height())
        self._textOffset = QPointF(-r.left(), -r.top())

    def boundingRect(self) -> QRectF:
        return self._boundingRect

    def paint(self, painter: typing.Optional[QPainter],
              option: typing.Optional['QStyleOptionGraphicsItem'],
              widget: typing.Optional[QWidget] = ...) -> None:
        painter.fillRect(self._boundingRect, self._background)
        painter.setFont(self._font)

        painter.setPen(self._cardColor)
        painter.drawText(self._textOffset, self._text)
