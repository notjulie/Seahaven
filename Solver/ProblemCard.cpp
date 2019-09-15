/* 
 * File:   ProblemCard.cpp
 * Author: Randy
 * 
 * Created on April 2, 2014, 2:12 PM
 */

#include "SolverIncludes.h"

#include "SolverException.h"

#include "ProblemCard.h"

ProblemCard::ProblemCard(const char *_s)
{
    if (_s == NULL)
    {
        rank = 0;
        suit = Suit::Clubs;
        return;
    }
    
    if (strlen(_s) != 2)
        throw SolverException("ProblemCard string must consist of two characters");
    
    switch (_s[0])
    {
        case 'A':
            rank = 1;
            break;
            
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            rank = _s[0] - '0';
            break;

        case 'T':
            rank = 10;
            break;
            
        case 'J':
            rank = 11;
            break;
            
        case 'Q':
            rank = 12;
            break;
            
        case 'K':
            rank = 13;
            break;
            
        default:
        {
            char        buffer[100];
            snprintf(buffer, sizeof(buffer), "ProblemCard: invalid rank character: %c", _s[0]);
            throw SolverException(buffer);
        }
    }

    switch (_s[1])
    {
        case 'C':
            suit = Suit::Clubs;
            break;
            
        case 'D':
            suit = Suit::Diamonds;
            break;
            
        case 'H':
            suit = Suit::Hearts;
            break;
            
        case 'S':
            suit = Suit::Spades;
            break;
            
        default:
        {
            char        buffer[100];
            snprintf(buffer, sizeof(buffer), "ProblemCard: invalid suit character: %c", _s[1]);
            throw SolverException(buffer);
        }
    }
}

ProblemCard::ProblemCard(Suit _suit, int _rank)
{
   suit = _suit;
   rank = _rank;
}

bool ProblemCard::operator==(const ProblemCard &that) const
{
   return
      rank == that.rank &&
      suit == that.suit;
}

void ProblemCard::DumpToConsole(void) const
{
   static const char ranks[] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J','Q', 'K'};
   
   char s[3];
   s[0] = ranks[rank - 1];
   s[1] = suit.GetMnemonic();
   s[2] = 0;
   printf("%s\r\n", s);
}
