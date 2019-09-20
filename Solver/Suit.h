/* 
 * File:   Suit.h
 * Author: Randy
 *
 * Created on April 2, 2014, 3:39 PM
 */

#ifndef SUIT_H
#define	SUIT_H

#include "LinkID.h"

class Suit {
public:
   inline uint8_t GetIndex(void) const { return index; }

   inline bool operator<=(Suit suit) const { return index <= suit.index; }
   inline bool operator==(Suit suit) const { return index == suit.index; }
   inline bool operator!=(Suit suit) const { return index != suit.index; }
   inline Suit operator++() { ++index; return *this; }

public:
   char GetMnemonic(void) const;

public:
   static Suit FromIndex(int index);
   static Suit FromMnemonic(char mnemonic);

public:
   static const Suit Clubs;
   static const Suit Diamonds;
   static const Suit Hearts;
   static const Suit Spades;

   static const Suit First;
   static const Suit Last;

   static const Suit All[4];

private:
   uint8_t index;
};

#endif	/* SUIT_H */

