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


const LinkedCard LinkedCard::Null = { CardLocation::Null, CardLocation::Null, 0 };


// ==============================================================
//    class LinkedCards
// ==============================================================

LinkedCards::LinkedCards() {
   Clear();
}

void LinkedCards::Clear(void)
{
   for (int i = 0; i < (uint8_t)LinkID::LINK_COUNT; ++i)
      links[i] = LinkedCard::Null;
}

int LinkedCards::GetEmptyTowers(void) const
{
   return
      4 -
      LinkedCard(links[(int)LinkID::FIRST_TOWER_LINK]).size -
      LinkedCard(links[(int)(LinkID)((uint8_t)LinkID::FIRST_TOWER_LINK + 1)]).size -
      LinkedCard(links[(int)(LinkID)((uint8_t)LinkID::FIRST_TOWER_LINK + 2)]).size -
      LinkedCard(links[(int)(LinkID)((uint8_t)LinkID::FIRST_TOWER_LINK + 3)]).size;
}

ProblemCard LinkedCards::GetCardDetails(LinkID link) const
{
   int   rank = 0;
   for (;;)
   {
      LinkedCard  card(links[(int)link]);
      rank += card.size;
      
      if (card.toLower.GetLinkID() == LinkID::NO_LINK)
         return ProblemCard(
            Suit::All[((uint8_t)link - (uint8_t)LinkID::FIRST_ACE_LINK)],
            rank
            );
      
      link = card.toLower.GetLinkID();
   }
}

int LinkedCards::GetThroneOccupationMask(void) const
{
   return
      (LinkedCard(links[(int)LinkID::FIRST_THRONE_LINK]).size != 0 ? 1 : 0) +
      (LinkedCard(links[(int)(LinkID)((uint8_t)LinkID::FIRST_THRONE_LINK + 1)]).size != 0 ? 2 : 0) +
      (LinkedCard(links[(int)(LinkID)((uint8_t)LinkID::FIRST_THRONE_LINK + 2)]).size != 0 ? 4 : 0) +
      (LinkedCard(links[(int)(LinkID)((uint8_t)LinkID::FIRST_THRONE_LINK + 3)]).size != 0 ? 8 : 0);
}

void LinkedCards::MoveToHigher(CardLocation cardLocation)
{
   // get the card
   LinkedCard card(links[(int)cardLocation.GetLinkID()]);
   
   // link the lower to the higher
   LinkedCard lower(links[(int)card.toLower.GetLinkID()]);
   lower.toHigher = card.toHigher;
   links[(int)card.toLower.GetLinkID()] = lower;
   
   // link the higher to the lower and add the size of the card
   // being moved
   LinkedCard higher(links[(int)card.toHigher.GetLinkID()]);
   higher.toLower = card.toLower;
   higher.size += card.size;
   links[(int)card.toHigher.GetLinkID()] = higher;
   
   // make the card being moved go away
   links[(int)cardLocation.GetLinkID()] = LinkedCard::Null;
}

void LinkedCards::MoveToLower(LinkID link)
{
   // get the card
   LinkedCard card(links[(int)link]);
   
   // link the higher to the lower
   if (card.toHigher.GetLinkID() != LinkID::NO_LINK)
   {
      LinkedCard higher(links[(int)card.toHigher.GetLinkID()]);
      higher.toLower = card.toLower;
      links[(int)card.toHigher.GetLinkID()] = higher;
   }
   
   // link the lower to the higher and add the size of the card
   // being moved
   LinkedCard lower(links[(int)card.toLower.GetLinkID()]);
   lower.toHigher = card.toHigher;
   lower.size += card.size;
   links[(int)card.toLower.GetLinkID()] = lower;
   
   // make the card being moved go away
   links[(int)link] = LinkedCard::Null;
}

void LinkedCards::MoveToOpenTower(CardLocation cardLocation)
{
   // find an open tower... we assume we have one
   LinkID tower = LinkID::FIRST_TOWER_LINK;
   for (;;)
   {
      LinkedCard t(links[(int)tower]);
      if (t.size == 0)
         break;
      tower = (LinkID)((uint8_t)tower + 1);
   }
   
   // get the card
   LinkedCard card(links[(int)cardLocation.GetLinkID()]);
   
   // link the lower to the tower
   LinkedCard lower(links[(int)card.toLower.GetLinkID()]);
   lower.toHigher = CardLocation::Links[(uint8_t)tower];
   links[(int)card.toLower.GetLinkID()] = lower;
   
   // link the higher to the tower
   LinkedCard higher(links[(int)card.toHigher.GetLinkID()]);
   higher.toLower = CardLocation::Links[(uint8_t)tower];
   links[(int)card.toHigher.GetLinkID()] = higher;
   
   // move the card to the tower
   links[(int)tower] = links[(int)cardLocation.GetLinkID()];
   links[(int)cardLocation.GetLinkID()] = LinkedCard::Null;
}

void LinkedCards::SetAceSizes(void)
{
   // all we do is follow the chain of upper links; the cards on the
   // aces must be 13 - all other cards in the suit's chain
   for (int i=0; i<4; ++i)
   {
      LinkedCard  ace(links[(int)(LinkID)((uint8_t)LinkID::FIRST_ACE_LINK + i)]);
      
      int totalCards = 0;
      LinkedCard  card = ace;
      while (card.toHigher.GetLinkID() != LinkID::NO_LINK)
      {
         card = LinkedCard(links[(int)card.toHigher.GetLinkID()]);
         totalCards += card.size;
      }
      
      ace.size = 13 - totalCards;
      links[(int)(LinkID)((uint8_t)LinkID::FIRST_ACE_LINK + i)] = ace;
   }
}


/// <summary>
/// Sets the given card's links and size; it also sets back links and verifies;
/// this is called only in setup, so we check and verify and throw exceptions
/// as we see fit
/// </summary>
void LinkedCards::SetCard(CardLocation cardLocation, LinkedCard card)
{
   // update the card in question
   LinkedCard  linkedCard(links[(int)cardLocation.GetLinkID()]);
   if (linkedCard.toLower.GetLinkID() != LinkID::NO_LINK && linkedCard.toLower !=card.toLower)
      throw SolverException("LinkedCards::SetCard: rewriting link to lower");
   if (linkedCard.toHigher.GetLinkID() != LinkID::NO_LINK && linkedCard.toHigher !=card.toHigher)
      throw SolverException("LinkedCards::SetCard: rewriting link to higher");
   links[(int)cardLocation.GetLinkID()] = card;

   // update the lower
   if (!card.toLower.IsNull())
   {
      LinkedCard  lowerCard(links[(int)card.toLower.GetLinkID()]);
      if (lowerCard.toHigher.GetLinkID() != LinkID::NO_LINK && lowerCard.toHigher !=cardLocation)
         throw SolverException("LinkedCards::SetCard: rewriting link from lower");
      lowerCard.toHigher = cardLocation;
      links[(int)card.toLower.GetLinkID()] = lowerCard;
   }
   
   // update the higher
   if (!card.toHigher.IsNull())
   {
      LinkedCard  higherCard(links[(int)card.toHigher.GetLinkID()]);
      if (higherCard.toLower.GetLinkID() != LinkID::NO_LINK && higherCard.toLower !=cardLocation)
         throw SolverException("LinkedCards::SetCard: rewriting link from higher");
      higherCard.toLower = cardLocation;
      links[(int)card.toHigher.GetLinkID()] = higherCard;
   }   
}

int LinkedCards::CountKingsOnTowers(void) const
{
   int kingsOnTowers = 0;
   for (Suit suit = Suit::First; suit <= Suit::Last; ++suit)
   {
      LinkedCard throne = GetCard(CardLocation::Thrones[suit.GetIndex()]);
      if (throne.size == 0)
         if (throne.toLower.IsTower())
            ++kingsOnTowers;
   }
   return kingsOnTowers;
}


