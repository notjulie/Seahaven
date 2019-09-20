
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
/// Class representing the location of a card.
/// </summary>
class CardLocation {
public:
   inline int GetRowIndex(void) const { return ((uint8_t)linkID - (uint8_t)LinkID::FIRST_COLUMN_LINK) % 5; }
   inline LinkID GetLinkID(void) const { return linkID; }
   inline bool IsNull(void) const { return linkID == LinkID::NO_LINK; }
   inline bool IsOnColumn(void) const { return (uint8_t)linkID >= (uint8_t)LinkID::FIRST_COLUMN_LINK && (uint8_t)linkID - (uint8_t)LinkID::FIRST_COLUMN_LINK < 50; }
   inline bool IsThrone(void) const { return (uint8_t)linkID >= (uint8_t)LinkID::FIRST_THRONE_LINK && (uint8_t)linkID - (uint8_t)LinkID::FIRST_THRONE_LINK <= 3; }
   inline bool IsTower(void) const { return (uint8_t)linkID >= (uint8_t)LinkID::FIRST_TOWER_LINK && (uint8_t)linkID - (uint8_t)LinkID::FIRST_TOWER_LINK <= 3; }

   int GetColumnIndex(void) const;
   Suit GetSuit(void) const;

   inline bool operator==(CardLocation card) const { return card.linkID == linkID; }
   inline bool operator!=(CardLocation card) const { return card.linkID != linkID; }

public:
   static const CardLocation Null;
   static const CardLocation Aces[4];
   static const CardLocation Columns[10][5];
   static const CardLocation Thrones[4];
   static const CardLocation Towers[4];
   static const CardLocation Links[(uint8_t)LinkID::LINK_COUNT_INCLUDING_NO_LINK];

private:
   static CardLocation FromLinkID(LinkID linkID);

private:
   LinkID linkID;
};

#endif
