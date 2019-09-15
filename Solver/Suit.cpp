
#include "SolverIncludes.h"
#include "SolverException.h"
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

Suit Suit::FromMnemonic(char mnemonic)
{
   char        buffer[100];

   // NOTE: we don't use our global const values here because the unit test likes
   // to call this during C++ runtime initialization for the creation of test cases.
   // Since that's mostly what we're here for, we accommodate that.
   switch (mnemonic)
   {
   case 'C':
      return Suit::FromIndex(0);

   case 'D':
      return Suit::FromIndex(1);

   case 'H':
      return Suit::FromIndex(2);

   case 'S':
      return Suit::FromIndex(3);

   default:
      snprintf(buffer, sizeof(buffer), "Suit::FromMnemonic: invalid suit character: %c", mnemonic);
      throw SolverException(buffer);
   }
}
