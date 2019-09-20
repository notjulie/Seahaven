
#include "SolverIncludes.h"
#include "CardLocation.h"



int CardLocation::GetColumnIndex(void) const
{
   int column = ((uint8_t)linkID - (uint8_t)LinkID::FIRST_COLUMN_LINK) / 5;
   if (column >= 10)
      return -1;
   else
      return column;
}


Suit CardLocation::GetSuit(void) const
{
   int suitIndex = (uint8_t)linkID - (uint8_t)LinkID::FIRST_ACE_LINK;
   if (suitIndex<0 || suitIndex>=4)
      suitIndex = (uint8_t)linkID - (uint8_t)LinkID::FIRST_THRONE_LINK;
   return Suit::FromIndex(suitIndex);
}


CardLocation CardLocation::FromLinkID(LinkID linkID)
{
   CardLocation result;
   result.linkID = linkID;
   return result;
}


const CardLocation CardLocation::Aces[4] = {
   CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_ACE_LINK + 0)),
   CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_ACE_LINK + 1)),
   CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_ACE_LINK + 2)),
   CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_ACE_LINK + 3))
};

const CardLocation CardLocation::Thrones[4] = {
   CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_THRONE_LINK + 0)),
   CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_THRONE_LINK + 1)),
   CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_THRONE_LINK + 2)),
   CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_THRONE_LINK + 3))
};

const CardLocation CardLocation::Towers[4] = {
   CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_TOWER_LINK + 0)),
   CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_TOWER_LINK + 1)),
   CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_TOWER_LINK + 2)),
   CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_TOWER_LINK + 3))
};


const CardLocation CardLocation::Columns[10][5] = {
   {
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 0)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 1)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 2)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 3)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 4))
   },
   {
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 5)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 6)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 7)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 8)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 9))
   },
   {
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 10)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 11)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 12)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 13)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 14))
   },
   {
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 15)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 16)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 17)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 18)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 19))
   },
   {
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 20)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 21)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 22)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 23)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 24))
   },
   {
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 25)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 26)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 27)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 28)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 29))
   },
   {
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 30)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 31)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 32)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 33)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 34))
   },
   {
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 35)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 36)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 37)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 38)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 39))
   },
   {
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 40)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 41)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 42)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 43)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 44))
   },
   {
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 45)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 46)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 47)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 48)),
      CardLocation::FromLinkID((LinkID)((uint8_t)LinkID::FIRST_COLUMN_LINK + 49))
   }
};
static const CardLocation Towers[4];
