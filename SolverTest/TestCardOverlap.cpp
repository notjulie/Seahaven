
#include "SolverIncludes.h"
#include "SeahavenProblem.h"
#include "SolverState.h"


extern SeahavenProblem testCardOverlapTestCase;


/// <summary>
/// This test case helped me spot an error in GetBottomColumnCard.  Might as well
/// keep it around as a regression test.
/// </summary>
void TestCardOverlap(void)
{
   // create a state from the problem
   SolverState state(testCardOverlapTestCase);

   LinkedCard sevenOfDiamonds = state.GetColumnBottomCard(5);
   if (sevenOfDiamonds.size != 3)
      throw SolverException("TestCardOverlap: failed to get seven of diamonds");

   // check out our pointer to the 8 to see if it's the bottom card
   int column = sevenOfDiamonds.toHigher.GetColumnIndex();
   if (column != 8)
      throw SolverException("TestCardOverlap: eight of diamonds column is wrong");
   if (sevenOfDiamonds.toHigher.GetRowIndex() != 3)
      throw SolverException("TestCardOverlap: eight of diamonds row is wrong");
   if (state.GetColumnCardCount(column) != 5)
      throw SolverException("TestCardOverlap: eight of diamonds cards in column is wrong");
}

// This was part of a solution I ran into... from this point it tried to
// move the 7-6-5D on to the 8D, even though the 8 is covered by the 9.
SeahavenProblem testCardOverlapTestCase(
   ProblemCards("2S", "TD"),