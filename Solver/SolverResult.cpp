
#include "SolverIncludes.h"
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

