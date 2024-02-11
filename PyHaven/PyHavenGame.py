
import random


class PyHavenGame:
    """
    class PyHavenGame
    -- maintains the locations of all the cards
    """

    def __init__(self):
        """ initializer; starts empty"""
        self._columns = None
        self._towers = [None, None, None, None]
        self._aces = [0, 0, 0, 0]

    def new_game(self):
        """ shuffles and deals """

        # create a new deck
        cards = []
        for i in range(52):
            cards.append(i)

        # shuffle
        for i in range(52):
            j = random.randint(i, 51)
            t = cards[i]
            cards[i] = cards[j]
            cards[j] = t

        # deal
        self._towers = [None, cards[50], cards[51], None]
        self._columns = []
        for column in range(10):
            self._columns.append(
                [
                    cards[column * 5 + 0],
                    cards[column * 5 + 1],
                    cards[column * 5 + 2],
                    cards[column * 5 + 3],
                    cards[column * 5 + 4],
                ]
            )
