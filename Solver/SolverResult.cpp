
#include "SolverIncludes.h"
#include "SeahavenProblem.h"
#include "SolverResult.h"

Solution SolverResult::CreateSolution(void) const
{
   // if no solution was found...
   if (resultStack.IsEmpty())
      return Solution::Fail();

   // else put together a solution from the changing states
   Solution solution;
   for (int i = 1; i < resultStack.GetSize(); ++i)
   {
      SolverMove move = resultStack[i].GetMoveThatWasPerformed();
      switch (move.type)
      {
      case SolverMoveType::FromColumnToHigherCard:
      case SolverMoveType::FromColumnToTower:
         solution.AddStep(resultStack[i - 1].GetBottomColumnCardDetails(move.column));
         break;

      case SolverMoveType::FromTowerToEmptyThrone:
         solution.AddStep(ProblemCard(move.suit, 13));
         break;

      default:
         throw SolverException("Solver::Solve: unrecognized move type");
      }
   }

   return solution;
}


void SolverResult::DumpToFile(FILE* file) const
{
   // create a solution... that's really nothing more than a list showing the card that
   // was moved in each step
   Solution solution = CreateSolution();

   // loop through every state in the result and dump it to the file
   for (int i = 0; i < resultStack.GetSize(); ++i)
   {
      if (i != 0)
         fprintf(file, "======================================================\r\n");

      // create a problem from the state entry
      SeahavenProblem problem(resultStack[i]);

      // dump it
      problem.Dump(file);

      // dump out the next move
      if (i < solution.GetStepCount())
      {
         fprintf(file, "Next move: ");
         solution.GetStep(i).Dump(file);
      }
   }
}
