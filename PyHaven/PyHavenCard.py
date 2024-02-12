import math
from abc import abstractmethod


class PyHavenCard:
    """ abstract definition of PyHavenCard; implementation is private """
    @abstractmethod
    def get_rank(self):
        """ gets the rank of the card """
        pass

    @abstractmethod
    def get_suit(self):
        """ gets the suit of the card """
        return math.floor(self._index / 13)


def get_pyhaven_card(i) -> PyHavenCard:
    """
    Returns the global instance of PyHavenCard associated with the
    given numeric index in the range 0..51.  This is implemented such
    that there are only 52 immutable PyHavenCard instances that can
    be compared using 'is'.
    """
    class PyHavenCardImplementation:
        """
        abstract representation of a card; scope is limited to the
        get_pyhaven_card function so that we control the instance creation
        """
        _cards = None

        def __init__(self, index):
            """ initializer """
            self._index = index

        @staticmethod
        def get_cards():
            """ gets (creating if necessary) the global list of cards """
            if PyHavenCardImplementation._cards is None:
                PyHavenCardImplementation._cards = []
                for cardIndex in range(52):
                    PyHavenCardImplementation._cards.append(PyHavenCardImplementation(cardIndex))
            return PyHavenCardImplementation._cards

        def get_rank(self):
            return self._index % 13

        def get_suit(self):
            return math.floor(self._index / 13)

    # return the card from the static list
    return PyHavenCardImplementation.get_cards()[i]
