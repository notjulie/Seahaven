/* 
 * File:   SolutionStep.h
 * Author: Randy
 *
 * Created on April 8, 2014, 7:51 PM
 */

#ifndef SOLUTIONSTEP_H
#define	SOLUTIONSTEP_H

#include "ProblemCard.h"

class SolutionStep {
public:
   SolutionStep(const char *s = NULL);
   SolutionStep(const ProblemCard &card);

   void Dump(FILE *f) const;
   bool IsNull(void) const { return card.IsNull(); }
   
   bool operator==(const SolutionStep &that) const;
   bool operator!=(const SolutionStep &that) const;
   
private:
   ProblemCard card;
};

#endif	/* SOLUTIONSTEP_H */

