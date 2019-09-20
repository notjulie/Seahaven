
#include "SolverIncludes.h"
#include "SeahavenProblem.h"
#include "SolverState.h"


// This was part of a solution I ran into... from this point it tried to
// move the 7-6-5D on to the 8D, even though the 8 is covered by the 9.
static const ProblemDefinition testCardOverlapTestCase =
{
   // columns
   {
      { "2S", "TD" },
      { "AS", "8S", "7C", "2D", "7S", "6S" },
      { "TC", "3C", "2C" },
      { "QS", "JC", "5C", "KH", "QH", "JH" },
      { "9C", "4H", "3H", "KC" },
      { "AC", "2H", "3S", "JD", "7D", "6D", "5D" },
      { "4D", "3D", "TS", "5S", "KD", "QD" },
      { "8C", "AH", "6C", "TH", "8H" },
      { "KS", "5H", "JS", "8D", "9D" },
      { "7H", "4C", "9S", "QC", "6H" }
   },
   // towers
   {"9H", "4S"}
};


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

