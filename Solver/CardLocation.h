
//
// Seahaven Solver project
//
//    Author: Randy Rasmussen
//    Copyright: None
//    Warranty: At your own risk
//

#ifndef CARDLOCATION_H
#define CARDLOCATION_H

#include "LinkID.h"
#include "Suit.h"


/// <summary>
/// Structure representing the location of a card.  Formerly just a wrapper around a LinkID,
/// it is now expanded to be a friendlier face on LinkID.
/// </summary>
struct alignas(1) CardLocation {
public:
   constexpr CardLocation(void);

public:
   LinkID linkID;
   bool onColumn : 1;
   bool isThrone : 1;
   bool isTower : 1;
   bool isAce : 1;
   uint8_t row : 3;
   union {
      Suit suit;
      uint8_t column;
   };

public:
   // all we card about for equality is the link ID; if there are other differences it
   // is because someone is modifying the location object after creating it, which is stupid
   inline bool operator==(CardLocation card) const { return card.linkID == linkID; }
   inline bool operator!=(CardLocation card) const { return card.linkID != linkID; }

public:
   static constexpr CardLocation Null(void);
   static const CardLocation Aces[4];
   static const CardLocation Columns[10][5];
   static const CardLocation Thrones[4];
   static const CardLocation Towers[4];
   static const CardLocation Links[(uint8_t)LinkID::LINK_COUNT_INCLUDING_NO_LINK];

private:
   static constexpr CardLocation FromLinkID(LinkID linkID);
};
static_assert(sizeof(CardLocation)<=3, "CardLocation is getting chubby");


constexpr CardLocation CardLocation::FromLinkID(LinkID linkID)
{
   CardLocation result;
   result.linkID = linkID;

   if ((uint8_t)linkID >= (uint8_t)LinkID::FIRST_COLUMN_LINK && (uint8_t)linkID - (uint8_t)LinkID::FIRST_COLUMN_LINK < 50)
   {
      result.onColumn = true;
      result.row = ((uint8_t)linkID - (uint8_t)LinkID::FIRST_COLUMN_LINK) % 5;
      result.column = ((uint8_t)linkID - (uint8_t)LinkID::FIRST_COLUMN_LINK) / 5;
   }
   else if ((uint8_t)linkID >= (uint8_t)LinkID::FIRST_THRONE_LINK && (uint8_t)linkID - (uint8_t)LinkID::FIRST_THRONE_LINK <= 3)
   {
      result.isThrone = true;
      result.suit = Suit::FromIndex((uint8_t)linkID - (uint8_t)LinkID::FIRST_THRONE_LINK);
   }
   else if ((uint8_t)linkID >= (uint8_t)LinkID::FIRST_ACE_LINK && (uint8_t)linkID - (uint8_t)LinkID::FIRST_ACE_LINK <= 3)
   {
      result.isAce = true;
      result.suit = Suit::FromIndex((uint8_t)linkID - (uint8_t)LinkID::FIRST_ACE_LINK);
   }
   else if ((uint8_t)linkID >= (uint8_t)LinkID::FIRST_TOWER_LINK && (uint8_t)linkID - (uint8_t)LinkID::FIRST_TOWER_LINK <= 3)
   {
      result.isTower = true;
   }

   return result;
}

constexpr CardLocation CardLocation::Null(void)
{
   return CardLocation::FromLinkID(LinkID::NO_LINK);
}

#endif
