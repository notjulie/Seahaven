
#include "SolverIncludes.h"
#include "CardLocation.h"


constexpr CardLocation::CardLocation(void)
   :
      linkID(LinkID::NO_LINK),
      onColumn(false),
      isThrone(false),
      isTower(false),
      isAce(false),
      row(0),
      suit(Suit::Clubs)
{
}


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

constexpr CardLocation CardLocation::Null = CardLocation::FromLinkID(LinkID::NO_LINK);

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

const CardLocation CardLocation::Links[(uint8_t)LinkID::LINK_COUNT_INCLUDING_NO_LINK] = {
      CardLocation::FromLinkID((LinkID)0),
      CardLocation::FromLinkID((LinkID)1),
      CardLocation::FromLinkID((LinkID)2),
      CardLocation::FromLinkID((LinkID)3),
      CardLocation::FromLinkID((LinkID)4),
      CardLocation::FromLinkID((LinkID)5),
      CardLocation::FromLinkID((LinkID)6),
      CardLocation::FromLinkID((LinkID)7),
      CardLocation::FromLinkID((LinkID)8),
      CardLocation::FromLinkID((LinkID)9),
      CardLocation::FromLinkID((LinkID)10),
      CardLocation::FromLinkID((LinkID)11),
      CardLocation::FromLinkID((LinkID)12),
      CardLocation::FromLinkID((LinkID)13),
      CardLocation::FromLinkID((LinkID)14),
      CardLocation::FromLinkID((LinkID)15),
      CardLocation::FromLinkID((LinkID)16),
      CardLocation::FromLinkID((LinkID)17),
      CardLocation::FromLinkID((LinkID)18),
      CardLocation::FromLinkID((LinkID)19),
      CardLocation::FromLinkID((LinkID)20),
      CardLocation::FromLinkID((LinkID)21),
      CardLocation::FromLinkID((LinkID)22),
      CardLocation::FromLinkID((LinkID)23),
      CardLocation::FromLinkID((LinkID)24),
      CardLocation::FromLinkID((LinkID)25),
      CardLocation::FromLinkID((LinkID)26),
      CardLocation::FromLinkID((LinkID)27),
      CardLocation::FromLinkID((LinkID)28),
      CardLocation::FromLinkID((LinkID)29),
      CardLocation::FromLinkID((LinkID)30),
      CardLocation::FromLinkID((LinkID)31),
      CardLocation::FromLinkID((LinkID)32),
      CardLocation::FromLinkID((LinkID)33),
      CardLocation::FromLinkID((LinkID)34),
      CardLocation::FromLinkID((LinkID)35),
      CardLocation::FromLinkID((LinkID)36),
      CardLocation::FromLinkID((LinkID)37),
      CardLocation::FromLinkID((LinkID)38),
      CardLocation::FromLinkID((LinkID)39),
      CardLocation::FromLinkID((LinkID)40),
      CardLocation::FromLinkID((LinkID)41),
      CardLocation::FromLinkID((LinkID)42),
      CardLocation::FromLinkID((LinkID)43),
      CardLocation::FromLinkID((LinkID)44),
      CardLocation::FromLinkID((LinkID)45),
      CardLocation::FromLinkID((LinkID)46),
      CardLocation::FromLinkID((LinkID)47),
      CardLocation::FromLinkID((LinkID)48),
      CardLocation::FromLinkID((LinkID)49),
      CardLocation::FromLinkID((LinkID)50),
      CardLocation::FromLinkID((LinkID)51),
      CardLocation::FromLinkID((LinkID)52),
      CardLocation::FromLinkID((LinkID)53),
      CardLocation::FromLinkID((LinkID)54),
      CardLocation::FromLinkID((LinkID)55),
      CardLocation::FromLinkID((LinkID)56),
      CardLocation::FromLinkID((LinkID)57),
      CardLocation::FromLinkID((LinkID)58),
      CardLocation::FromLinkID((LinkID)59),
      CardLocation::FromLinkID((LinkID)60),
      CardLocation::FromLinkID((LinkID)61),
      CardLocation::FromLinkID((LinkID)62)
};
