/* 
 * File:   SolutionStep.cpp
 * Author: Randy
 * 
 * Created on April 8, 2014, 7:51 PM
 */

#include "SolverIncludes.h"

#include "SolutionStep.h"

SolutionStep::SolutionStep(const char *s)
   :
      card(s)
{
}

SolutionStep::SolutionStep(const ProblemCard &_card)
   :
   card(_card)
{
}


bool SolutionStep::operator==(const SolutionStep &that) const
{
   return card == that.card;
}

bool SolutionStep::operator!=(const SolutionStep &that) const
{
   return !operator==(that);
}

void SolutionStep::Dump(FILE *f) const
{
   card.Dump(f);
   fprintf(f, "\r\n");
}
