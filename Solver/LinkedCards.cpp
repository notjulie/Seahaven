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


/// <summary>
/// Initializes a new instance of class LinkedCards
/// </summary>
LinkedCards::LinkedCards(void)
{
   Clear();
}

/// <summary>
/// Resets the object to an empty state
/// </summary>
void LinkedCards::Clear(void)
{
   for (int i = 0; i < (uint8_t)LinkID::LINK_COUNT; ++i)
      links[i] = LinkedCard::Null;
   for (int i = 0; i < 4; ++i)
      isThroneLocked[i] = false;
}

/// <summary>
/// Gets a count of the empty towers
/// </summary>
int LinkedCards::GetEmptyTowers(void) const
{
   return
      4 -
      LinkedCard(links[(int)LinkID::FIRST_TOWER_LINK]).size -
      LinkedCard(links[(int)(LinkID)((uint8_t)LinkID::FIRST_TOWER_LINK + 1)]).size -
      LinkedCard(links[(int)(LinkID)((uint8_t)LinkID::FIRST_TOWER_LINK + 2)]).size -
      LinkedCard(links[(int)(LinkID)((uint8_t)LinkID::FIRST_TOWER_LINK + 3)]).size;
}

/// <summary>
/// Converts the LinkedCard at the given location to an actual suit and
/// rank... this is not used during solving, but is necessary for converting
/// the solution back to something understandable.
/// </summary>
ProblemCard LinkedCards::GetCardDetails(CardLocation cardLocation) const
{
   int   rank = 0;
   for (;;)
   {
      LinkedCard  card(links[(int)cardLocation.linkID]);
      rank += card.size;
      
      if (card.toLower == CardLocation::Null)
         return ProblemCard(
            cardLocation.suit,
            rank
            );
      
      cardLocation = card.toLower;
   }
}

/// <summary>
/// Calculates a value that represents the state of the thrones.  This is used
/// by the solver's cache to represent each state of the game as a unique value.
/// </summary>
int LinkedCards::GetThroneHashValue(void) const
{
   int result = 0;
   for (int i = 0; i < 4; ++i)
   {
      result *= 3;

      // we calculate a state for each throne as:
      //  0: unoccupied
      //  1: occupied, unlocked
      //  2: occupied, locked
      if (links[(int)((uint8_t)LinkID::FIRST_THRONE_LINK + i)].size != 0)
      {
         if (isThroneLocked[i])
            result += 2;
         else
            result += 1;
      }
   }

   return result;
}

void LinkedCards::MoveToHigher(CardLocation cardLocation)
{
   // get the card
   LinkedCard card(links[(int)cardLocation.linkID]);
   
   // link the lower to the higher
   LinkedCard lower(links[(int)card.toLower.linkID]);
   lower.toHigher = card.toHigher;
   links[(int)card.toLower.linkID] = lower;
   
   // link the higher to the lower and add the size of the card
   // being moved
   LinkedCard higher(links[(int)card.toHigher.linkID]);
   higher.toLower = card.toLower;
   higher.size += card.size;
   links[(int)card.toHigher.linkID] = higher;
   
   // make the card being moved go away
   links[(int)cardLocation.linkID] = LinkedCard::Null;
}

void LinkedCards::MoveToLower(CardLocation link)
{
   // get the card
   LinkedCard card(links[(int)link.linkID]);
   
   // link the higher to the lower
   if (card.toHigher != CardLocation::Null)
   {
      LinkedCard higher(links[(int)card.toHigher.linkID]);
      higher.toLower = card.toLower;
      links[(int)card.toHigher.linkID] = higher;
   }
   
   // link the lower to the higher and add the size of the card
   // being moved
   LinkedCard lower(links[(int)card.toLower.linkID]);
   lower.toHigher = card.toHigher;
   lower.size += card.size;
   links[(int)card.toLower.linkID] = lower;
   
   // make the card being moved go away
   links[(int)link.linkID] = LinkedCard::Null;
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
   LinkedCard card(links[(int)cardLocation.linkID]);
   
   // link the lower to the tower
   LinkedCard lower(links[(int)card.toLower.linkID]);
   lower.toHigher = CardLocation::Links[(uint8_t)tower];
   links[(int)card.toLower.linkID] = lower;
   
   // link the higher to the tower
   LinkedCard higher(links[(int)card.toHigher.linkID]);
   higher.toLower = CardLocation::Links[(uint8_t)tower];
   links[(int)card.toHigher.linkID] = higher;
   
   // move the card to the tower
   links[(int)tower] = links[(int)cardLocation.linkID];
   links[(int)cardLocation.linkID] = LinkedCard::Null;
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
      while (card.toHigher != CardLocation::Null)
      {
         card = LinkedCard(links[(int)card.toHigher.linkID]);
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
   LinkedCard  linkedCard(links[(int)cardLocation.linkID]);
   if (linkedCard.toLower!=CardLocation::Null && linkedCard.toLower !=card.toLower)
      throw SolverException("LinkedCards::SetCard: rewriting link to lower");
   if (linkedCard.toHigher != CardLocation::Null && linkedCard.toHigher !=card.toHigher)
      throw SolverException("LinkedCards::SetCard: rewriting link to higher");
   links[(int)cardLocation.linkID] = card;

   // update the lower
   if (card.toLower != CardLocation::Null)
   {
      LinkedCard  lowerCard(links[(int)card.toLower.linkID]);
      if (lowerCard.toHigher!=CardLocation::Null && lowerCard.toHigher !=cardLocation)
         throw SolverException("LinkedCards::SetCard: rewriting link from lower");
      lowerCard.toHigher = cardLocation;
      links[(int)card.toLower.linkID] = lowerCard;
   }
   
   // update the higher
   if (card.toHigher != CardLocation::Null)
   {
      LinkedCard  higherCard(links[(int)card.toHigher.linkID]);
      if (higherCard.toLower != CardLocation::Null && higherCard.toLower !=cardLocation)
         throw SolverException("LinkedCards::SetCard: rewriting link from higher");
      higherCard.toLower = cardLocation;
      links[(int)card.toHigher.linkID] = higherCard;
   }   
}

int LinkedCards::CountKingsOnTowers(void) const
{
   int kingsOnTowers = 0;
   for (Suit suit : Suit::All)
   {
      LinkedCard throne = GetCard(CardLocation::Thrones[suit.GetIndex()]);
      if (throne.size == 0)
         if (throne.toLower.isTower)
            ++kingsOnTowers;
   }
   return kingsOnTowers;
}


