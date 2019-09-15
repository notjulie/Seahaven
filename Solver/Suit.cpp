
#include "SolverIncludes.h"
#include "Suit.h"

const Suit Suit::Clubs = Suit::FromIndex(0);
const Suit Suit::Diamonds = Suit::FromIndex(1);
const Suit Suit::Hearts = Suit::FromIndex(2);
const Suit Suit::Spades = Suit::FromIndex(3);

const Suit Suit::First = Suit::FromIndex(0);
const Suit Suit::Last = Suit::FromIndex(3);

const Suit Suit::All[4] = {
    Suit::FromIndex(0),
    Suit::FromIndex(1),
    Suit::FromIndex(2),
    Suit::FromIndex(3)
};


char Suit::GetMnemonic(void) const
{
   static const char mnemonics[4] = { 'C','D','H','S' };
   return mnemonics[index];
}

Suit Suit::FromIndex(int index)
{
   Suit result;
   result.index = index;
   return result;
}
