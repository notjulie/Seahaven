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


const CompressedLink CompressedLink::Null = CompressedLink::Create(LinkID::NO_LINK, LinkID::NO_LINK, 0);


// ==============================================================
//    class LinksArray
// ==============================================================

void LinksArray::Clear(void)
{
   for (int i = 0; i < (uint8_t)LinkID::LINK_COUNT; ++i)
      links[i] = CompressedLink::Null;
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
   return LinkedCard(links[(LinkID)((uint8_t)LinkID::FIRST_ACE_LINK + suit.GetIndex())]);
}

int LinkedCards::GetEmptyTowers(void) const
{
   return
      4 -
      LinkedCard(links[LinkID::FIRST_TOWER_LINK]).size -
      LinkedCard(links[(LinkID)((uint8_t)LinkID::FIRST_TOWER_LINK + 1)]).size -
      LinkedCard(links[(LinkID)((uint8_t)LinkID::FIRST_TOWER_LINK + 2)]).size -
      LinkedCard(links[(LinkID)((uint8_t)LinkID::FIRST_TOWER_LINK + 3)]).size;
}

LinkedCard LinkedCards::GetTower(int i) const
{
   return LinkedCard(links[(LinkID)((uint8_t)LinkID::FIRST_TOWER_LINK + i)]);
}

LinkedCard LinkedCards::GetThrone(Suit suit) const
{
   return LinkedCard(links[(LinkID)((uint8_t)LinkID::FIRST_THRONE_LINK + suit.GetIndex())]);
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
      
      if (card.toLower == LinkID::NO_LINK)
         return ProblemCard(
            Suit::All[((uint8_t)link - (uint8_t)LinkID::FIRST_ACE_LINK)],
            rank
            );
      
      link = card.toLower;
   }
}

int LinkedCards::GetThroneOccupationMask(void) const
{
   return
      (LinkedCard(links[LinkID::FIRST_THRONE_LINK]).size != 0 ? 1 : 0) +
      (LinkedCard(links[(LinkID)((uint8_t)LinkID::FIRST_THRONE_LINK + 1)]).size != 0 ? 2 : 0) +
      (LinkedCard(links[(LinkID)((uint8_t)LinkID::FIRST_THRONE_LINK + 2)]).size != 0 ? 4 : 0) +
      (LinkedCard(links[(LinkID)((uint8_t)LinkID::FIRST_THRONE_LINK + 3)]).size != 0 ? 8 : 0);
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
   links[card.toLower] = lower;
   
   // link the higher to the lower and add the size of the card
   // being moved
   LinkedCard higher(links[card.toHigher]);
   higher.toLower = card.toLower;
   higher.size += card.size;
   links[card.toHigher] = higher;
   
   // make the card being moved go away
   links[link] = CompressedLink::Null;
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
   links[card.toHigher] = higher;
   
   // make the card being moved smaller by one
   card.size--;
   links[link] = card;
   
   // move the rest of the card to a tower
   MoveToOpenTower(link);
}

void LinkedCards::MoveToLower(LinkID link)
{
   // get the card
   LinkedCard card(links[link]);
   
   // link the higher to the lower
   if (card.toHigher != LinkID::NO_LINK)
   {
      LinkedCard higher(links[card.toHigher]);
      higher.toLower = card.toLower;
      links[card.toHigher] = higher;
   }
   
   // link the lower to the higher and add the size of the card
   // being moved
   LinkedCard lower(links[card.toLower]);
   lower.toHigher = card.toHigher;
   lower.size += card.size;
   links[card.toLower] = lower;
   
   // make the card being moved go away
   links[link] = CompressedLink::Null;
}

void LinkedCards::MoveToOpenTower(LinkID link)
{
   // find an open tower... we assume we have one
   LinkID tower = LinkID::FIRST_TOWER_LINK;
   for (;;)
   {
      LinkedCard t(links[tower]);
      if (t.size == 0)
         break;
      tower = (LinkID)((uint8_t)tower + 1);
   }
   
   // get the card
   LinkedCard card(links[link]);
   
   // link the lower to the tower
   LinkedCard lower(links[card.toLower]);
   lower.toHigher = tower;
   links[card.toLower] = lower;
   
   // link the higher to the tower
   LinkedCard higher(links[card.toHigher]);
   higher.toLower = tower;
   links[card.toHigher] = higher;
   
   // move the card to the tower
   links[tower] = links[link];
   links[link] = CompressedLink::Null;
}

void LinkedCards::SetAceSizes(void)
{
   // all we do is follow the chain of upper links; the cards on the
   // aces must be 13 - all other cards in the suit's chain
   for (int i=0; i<4; ++i)
   {
      LinkedCard  ace(links[(LinkID)((uint8_t)LinkID::FIRST_ACE_LINK + i)]);
      
      int totalCards = 0;
      LinkedCard  card = ace;
      while (card.toHigher != LinkID::NO_LINK)
      {
         card = LinkedCard(links[card.toHigher]);
         totalCards += card.size;
      }
      
      ace.size = 13 - totalCards;
      links[(LinkID)((uint8_t)LinkID::FIRST_ACE_LINK + i)] = ace;
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
   if (linkedCard.toLower!= LinkID::NO_LINK && linkedCard.toLower!=toLow)
      throw SolverException("LinkedCards::SetCard: rewriting link to lower");
   if (linkedCard.toHigher!= LinkID::NO_LINK && linkedCard.toHigher!=toHigh)
      throw SolverException("LinkedCards::SetCard: rewriting link to higher");
   linkedCard.toHigher = toHigh;
   linkedCard.toLower = toLow;
   linkedCard.size = size;
   links[card] = linkedCard;

   // update the lower
   if (toLow != LinkID::NO_LINK)
   {
      LinkedCard  lowerCard(links[toLow]);
      if (lowerCard.toHigher!= LinkID::NO_LINK && lowerCard.toHigher!=card)
         throw SolverException("LinkedCards::SetCard: rewriting link from lower");
      lowerCard.toHigher = card;
      links[toLow] = lowerCard;      
   }
   
   // update the higher
   if (toHigh != LinkID::NO_LINK)
   {
      LinkedCard  higherCard(links[toHigh]);
      if (higherCard.toLower!= LinkID::NO_LINK && higherCard.toLower!=card)
         throw SolverException("LinkedCards::SetCard: rewriting link from higher");
      higherCard.toLower = card;
      links[toHigh] = higherCard;
   }   
}


