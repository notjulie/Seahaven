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
const LinkedCard LinkedCard::Null = { CardLocation::Null, CardLocation::Null, 0 };


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

ProblemCard LinkedCards::GetCardDetails(LinkID link) const
{
   int   rank = 0;
   for (;;)
   {
      LinkedCard  card(links[link]);
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
      (LinkedCard(links[LinkID::FIRST_THRONE_LINK]).size != 0 ? 1 : 0) +
      (LinkedCard(links[(LinkID)((uint8_t)LinkID::FIRST_THRONE_LINK + 1)]).size != 0 ? 2 : 0) +
      (LinkedCard(links[(LinkID)((uint8_t)LinkID::FIRST_THRONE_LINK + 2)]).size != 0 ? 4 : 0) +
      (LinkedCard(links[(LinkID)((uint8_t)LinkID::FIRST_THRONE_LINK + 3)]).size != 0 ? 8 : 0);
}

bool LinkedCards::IsKing(LinkedCard card) const
{
   return card.toHigher.IsThrone();
}

void LinkedCards::MoveToHigher(CardLocation cardLocation)
{
   // get the card
   LinkedCard card(links[cardLocation.GetLinkID()]);
   
   // link the lower to the higher
   LinkedCard lower(links[card.toLower.GetLinkID()]);
   lower.toHigher = card.toHigher;
   links[card.toLower.GetLinkID()] = lower;
   
   // link the higher to the lower and add the size of the card
   // being moved
   LinkedCard higher(links[card.toHigher.GetLinkID()]);
   higher.toLower = card.toLower;
   higher.size += card.size;
   links[card.toHigher.GetLinkID()] = higher;
   
   // make the card being moved go away
   links[cardLocation.GetLinkID()] = CompressedLink::Null;
}

void LinkedCards::MoveToLower(LinkID link)
{
   // get the card
   LinkedCard card(links[link]);
   
   // link the higher to the lower
   if (card.toHigher.GetLinkID() != LinkID::NO_LINK)
   {
      LinkedCard higher(links[card.toHigher.GetLinkID()]);
      higher.toLower = card.toLower;
      links[card.toHigher.GetLinkID()] = higher;
   }
   
   // link the lower to the higher and add the size of the card
   // being moved
   LinkedCard lower(links[card.toLower.GetLinkID()]);
   lower.toHigher = card.toHigher;
   lower.size += card.size;
   links[card.toLower.GetLinkID()] = lower;
   
   // make the card being moved go away
   links[link] = CompressedLink::Null;
}

void LinkedCards::MoveToOpenTower(CardLocation cardLocation)
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
   LinkedCard card(links[cardLocation.GetLinkID()]);
   
   // link the lower to the tower
   LinkedCard lower(links[card.toLower.GetLinkID()]);
   lower.toHigher = CardLocation::Links[(uint8_t)tower];
   links[card.toLower.GetLinkID()] = lower;
   
   // link the higher to the tower
   LinkedCard higher(links[card.toHigher.GetLinkID()]);
   higher.toLower = CardLocation::Links[(uint8_t)tower];
   links[card.toHigher.GetLinkID()] = higher;
   
   // move the card to the tower
   links[tower] = links[cardLocation.GetLinkID()];
   links[cardLocation.GetLinkID()] = CompressedLink::Null;
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
      while (card.toHigher.GetLinkID() != LinkID::NO_LINK)
      {
         card = LinkedCard(links[card.toHigher.GetLinkID()]);
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
void LinkedCards::SetCard(CardLocation cardLocation, LinkedCard card)
{
   // update the card in question
   LinkedCard  linkedCard(links[cardLocation.GetLinkID()]);
   if (linkedCard.toLower.GetLinkID() != LinkID::NO_LINK && linkedCard.toLower !=card.toLower)
      throw SolverException("LinkedCards::SetCard: rewriting link to lower");
   if (linkedCard.toHigher.GetLinkID() != LinkID::NO_LINK && linkedCard.toHigher !=card.toHigher)
      throw SolverException("LinkedCards::SetCard: rewriting link to higher");
   links[cardLocation.GetLinkID()] = card;

   // update the lower
   if (!card.toLower.IsNull())
   {
      LinkedCard  lowerCard(links[card.toLower.GetLinkID()]);
      if (lowerCard.toHigher.GetLinkID() != LinkID::NO_LINK && lowerCard.toHigher !=cardLocation)
         throw SolverException("LinkedCards::SetCard: rewriting link from lower");
      lowerCard.toHigher = cardLocation;
      links[card.toLower.GetLinkID()] = lowerCard;
   }
   
   // update the higher
   if (!card.toHigher.IsNull())
   {
      LinkedCard  higherCard(links[card.toHigher.GetLinkID()]);
      if (higherCard.toLower.GetLinkID() != LinkID::NO_LINK && higherCard.toLower !=cardLocation)
         throw SolverException("LinkedCards::SetCard: rewriting link from higher");
      higherCard.toLower = cardLocation;
      links[card.toHigher.GetLinkID()] = higherCard;
   }   
}

int LinkedCards::CountKingsOnTowers(void) const
{
   int kingsOnTowers = 0;
   for (Suit suit = Suit::First; suit <= Suit::Last; ++suit)
   {
      LinkedCard throne = GetThrone(suit);
      if (throne.size == 0)
         if (throne.toLower.IsTower())
            ++kingsOnTowers;
   }
   return kingsOnTowers;
}


