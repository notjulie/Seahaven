
#ifndef CARDLOCATION_H
#define CARDLOCATION_H

enum class LinkID : uint8_t {
   FIRST_COLUMN_LINK = 0,
   FIRST_TOWER_LINK = 50,
   FIRST_ACE_LINK = 54,
   FIRST_THRONE_LINK = 58,
   LINK_COUNT = 62,
   NO_LINK = LINK_COUNT
};


class CardLocation {
public:
   inline int GetRowIndex(void) const { return ((uint8_t)linkID - (uint8_t)LinkID::FIRST_COLUMN_LINK) % 5; }
   inline LinkID GetLinkID(void) const { return linkID; }
   inline bool IsThrone(void) const { return (uint8_t)linkID >= (uint8_t)LinkID::FIRST_TOWER_LINK && (uint8_t)linkID - (uint8_t)LinkID::FIRST_TOWER_LINK <= 3; }
   inline bool IsTower(void) const { return (uint8_t)linkID >= (uint8_t)LinkID::FIRST_TOWER_LINK && (uint8_t)linkID - (uint8_t)LinkID::FIRST_TOWER_LINK <= 3; }
   inline void SetLinkID(LinkID linkID) { this->linkID = linkID; }

   int GetColumnIndex(void) const;

private:
   LinkID linkID;
};

#endif
