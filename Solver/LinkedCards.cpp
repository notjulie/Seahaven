/* 
 * File:   LinkedCards.cpp
 * Author: Randy
 * 
 * Created on April 3, 2014, 1:51 PM
 */

#include "SolverIncludes.h"

#include "ProblemCard.h"
#include "SolverException.h"

#include "LinkedCards.h"

// ==============================================================
//    class LinksArray
// ==============================================================

void LinksArray::Clear(void)
{
   LinkedCard  nullCard;
   for (int i = 0; i < LINK_COUNT; ++i)
      links[i] = nullCard;
}


// ==============================================================
//    class LinkedCards
// ==============================================================

LinkedCards::LinkedCards() {
   Clear();
}

void LinkedCards::Clear(void)
{
   links.Clear();
}

LinkedCard LinkedCards::GetAce(Suit suit) const
{
   return LinkedCard(links[(LinkID)(FIRST_ACE_LINK + suit)]);
}

int LinkedCards::GetEmptyTowers(void) const
{
   return
      4 -
      LinkedCard(links[FIRST_TOWER_LINK]).size -
      LinkedCard(links[(LinkID)(FIRST_TOWER_LINK + 1)]).size -
      LinkedCard(links[(LinkID)(FIRST_TOWER_LINK + 2)]).size -
      LinkedCard(links[(LinkID)(FIRST_TOWER_LINK + 3)]).size;
}

LinkedCard LinkedCards::GetTower(int i) const
{
   return LinkedCard(links[(LinkID)(FIRST_TOWER_LINK + i)]);
}

LinkedCard LinkedCards::GetThrone(int i) const
{
   return LinkedCard(links[(LinkID)(FIRST_THRONE_LINK + i)]);
}

LinkedCard LinkedCards::GetColumnCard(int column, int row) const
{
   return LinkedCard(links[GetColumnLinkID(column, row)]);
}

ProblemCard LinkedCards::GetCardDetails(LinkID link) const
{
   int   rank = 0;
   for (;;)
   {
      LinkedCard  card(links[link]);
      rank += card.size;
      
      if (card.toLower == NO_LINK)
         return ProblemCard(
            (Suit)(link - FIRST_ACE_LINK),
            rank
            );
      
      link = card.toLower;
   }
}

int LinkedCards::GetThroneOccupationMask(void) const
{
   return
      (LinkedCard(links[FIRST_THRONE_LINK]).size != 0 ? 1 : 0) +
      (LinkedCard(links[(LinkID)(FIRST_THRONE_LINK + 1)]).size != 0 ? 2 : 0) +
      (LinkedCard(links[(LinkID)(FIRST_THRONE_LINK + 2)]).size != 0 ? 4 : 0) +
      (LinkedCard(links[(LinkID)(FIRST_THRONE_LINK + 3)]).size != 0 ? 8 : 0);
}

bool LinkedCards::IsKing(LinkedCard card) const
{
   return IsTower(card.toHigher);
}

void LinkedCards::MoveToHigher(LinkID link)
{
   // get the card
   LinkedCard card(links[link]);
   
   // link the lower to the higher
   LinkedCard lower(links[card.toLower]);
   lower.toHigher = card.toHigher;
   links[card.toLower] = lower.AsUInt16();
   
   // link the higher to the lower and add the size of the card
   // being moved
   LinkedCard higher(links[card.toHigher]);
   higher.toLower = card.toLower;
   higher.size += card.size;
   links[card.toHigher] = higher.AsUInt16();
   
   // make the card being moved go away
   links[link] = LinkedCard().AsUInt16();
}

void LinkedCards::MoveColumnCardToHigher(LinkID link)
{
   // get the card in question
   LinkedCard card(links[link]);
   
   // if it is of size == 1 this is just a simple MoveToHigher
   if (card.size == 1)
   {
      MoveToHigher(link);
      return;
   }
   
   // else we just transfer one card to the higher and move
   // the rest to a tower

   // make the higher bigger by one
   LinkedCard higher(links[card.toHigher]);
   higher.size++;
   links[card.toHigher] = higher.AsUInt16();
   
   // make the card being moved smaller by one
   card.size--;
   links[link] = card.AsUInt16();
   
   // move the rest of the card to a tower
   MoveToOpenTower(link);
}

void LinkedCards::MoveToLower(LinkID link)
{
   // get the card
   LinkedCard card(links[link]);
   
   // link the higher to the lower
   if (card.toHigher != NO_LINK)
   {
      LinkedCard higher(links[card.toHigher]);
      higher.toLower = card.toLower;
      links[card.toHigher] = higher.AsUInt16();
   }
   
   // link the lower to the higher and add the size of the card
   // being moved
   LinkedCard lower(links[card.toLower]);
   lower.toHigher = card.toHigher;
   lower.size += card.size;
   links[card.toLower] = lower.AsUInt16();
   
   // make the card being moved go away
   links[link] = LinkedCard().AsUInt16();
}

void LinkedCards::MoveToOpenTower(LinkID link)
{
   // find an open tower... we assume we have one
   LinkID tower = FIRST_TOWER_LINK;
   for (;;)
   {
      LinkedCard t(links[tower]);
      if (t.size == 0)
         break;
      tower = (LinkID)(tower + 1);
   }
   
   // get the card
   LinkedCard card(links[link]);
   
   // link the lower to the tower
   LinkedCard lower(links[card.toLower]);
   lower.toHigher = tower;
   links[card.toLower] = lower.AsUInt16();
   
   // link the higher to the tower
   LinkedCard higher(links[card.toHigher]);
   higher.toLower = tower;
   links[card.toHigher] = higher.AsUInt16();
   
   // move the card to the tower
   links[tower] = links[link];
   links[link] = LinkedCard().AsUInt16();
}

void LinkedCards::SetAceSizes(void)
{
   // all we do is follow the chain of upper links; the cards on the
   // aces must be 13 - all other cards in the suit's chain
   for (int i=0; i<4; ++i)
   {
      LinkedCard  ace(links[(LinkID)(FIRST_ACE_LINK + i)]);
      
      int totalCards = 0;
      LinkedCard  card = ace;
      while (card.toHigher != NO_LINK)
      {
         card = LinkedCard(links[card.toHigher]);
         totalCards += card.size;
      }
      
      ace.size = 13 - totalCards;
      links[(LinkID)(FIRST_ACE_LINK + i)] = ace.AsUInt16();
   }
}


/// <summary>
/// Sets the given card's links and size; it also sets back links and verifies;
/// this is called only in setup, so we check and verify and throw exceptions
/// as we see fit
/// </summary>
void LinkedCards::SetCard(LinkID card, LinkID toLow, LinkID toHigh, uint8_t size)
{
   // update the card in question
   LinkedCard  linkedCard(links[card]);
   if (linkedCard.toLower!=NO_LINK && linkedCard.toLower!=toLow)
      throw SolverException("LinkedCards::SetCard: rewriting link to lower");
   if (linkedCard.toHigher!=NO_LINK && linkedCard.toHigher!=toHigh)
      throw SolverException("LinkedCards::SetCard: rewriting link to higher");
   linkedCard.toHigher = toHigh;
   linkedCard.toLower = toLow;
   linkedCard.size = size;
   links[card] = linkedCard.AsUInt16();

   // update the lower
   if (toLow != NO_LINK)
   {
      LinkedCard  lowerCard(links[toLow]);
      if (lowerCard.toHigher!=NO_LINK && lowerCard.toHigher!=card)
         throw SolverException("LinkedCards::SetCard: rewriting link from lower");
      lowerCard.toHigher = card;
      links[toLow] = lowerCard.AsUInt16();      
   }
   
   // update the higher
   if (toHigh != NO_LINK)
   {
      LinkedCard  higherCard(links[toHigh]);
      if (higherCard.toLower!=NO_LINK && higherCard.toLower!=card)
         throw SolverException("LinkedCards::SetCard: rewriting link from higher");
      higherCard.toLower = card;
      links[toHigh] = higherCard.AsUInt16();
   }   
}
